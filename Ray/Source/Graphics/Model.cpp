#include "Ray.h"

Model::Model(std::string path, const ShaderProgramDesc& shader)
{
	m_Model = glm::mat4(1.f);
	m_UniformBuffer = Graphics::Instance()->CreateUniformBuffer({ sizeof(VertexData) });
	m_Shader = Graphics::Instance()->CreateShaderProgram(shader);
	m_Shader->SetUniformBufferSlot("VertexData", 0);

	LoadModel(path);
}

void Model::Draw()
{
	VertexData vdata = { FloatingCamera::GetFloatingCam().GetViewProj() * m_Model, m_Model };
	m_UniformBuffer->SetData(&vdata);

	Graphics::Instance()->SetShaderProgram(m_Shader);
	Graphics::Instance()->SetUniformBuffer(m_UniformBuffer, 0);

	LightingManager::Instance().ManageBasicLighting(m_Shader, glm::vec3(1.f), Materials::debug);

	for (auto& mesh : m_Meshes)
	{
		mesh.Draw(m_Shader);
	}
}

void Model::LoadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene * scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::wstringstream ss;
		ss << L"Assimp: " << importer.GetErrorString();
		Logger::Instance()->PrintOGL3DError(ss.str().c_str());
		return;
	}
	m_Directory = path.substr(0, path.find_last_of('\\'));

	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_Meshes.push_back(ProcessMesh(mesh, scene));
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<TexturePtr> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.m_Position = vector;

		if (mesh->HasNormals())
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.m_Normal = vector;
		}

		if (mesh->mTextureCoords[0])
		{
			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.m_TexCoords = vec;

			if (mesh->HasTangentsAndBitangents())
			{
				vector.x = mesh->mTangents[i].x;
				vector.y = mesh->mTangents[i].y;
				vector.z = mesh->mTangents[i].z;
				vertex.m_Tangent = vector;

				vector.x = mesh->mBitangents[i].x;
				vector.y = mesh->mBitangents[i].y;
				vector.z = mesh->mBitangents[i].z;
				vertex.m_Bitangent = vector;
			}
		}
		else
		{
			vertex.m_TexCoords = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}
	// process indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<TexturePtr> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<TexturePtr> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		std::vector<TexturePtr> normalMaps = LoadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

		std::vector<TexturePtr> roughnessMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE_ROUGHNESS, "texture_roughness");
		textures.insert(textures.end(), roughnessMaps.begin(), roughnessMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

std::vector<TexturePtr> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<TexturePtr> textures;
	
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;

		std::string material_path(m_Directory);
		material_path.push_back('\\');
		material_path.append(str.C_Str());

		for (auto& loaded_texture : m_TexturesLoaded)
		{
			if (std::strcmp(loaded_texture->m_Path.c_str(), material_path.c_str()) == 0)
			{
				textures.push_back(loaded_texture);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			TexturePtr tex = Graphics::Instance()->CreateTexture(material_path.c_str());
			tex->m_Type = typeName;
			textures.push_back(tex);
			m_TexturesLoaded.push_back(tex);
		}
	}
	return textures;
}