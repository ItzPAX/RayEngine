#pragma once

class RAY_API Model
{
public:
    Model() = default;
    Model(std::string path, const ShaderProgramDesc& shader);
    void Draw();
    
protected:
    glm::mat4 m_Model;

private:
    std::vector<TexturePtr> m_TexturesLoaded;

    UniformBufferPtr m_UniformBuffer;
    ShaderProgramPtr m_Shader;

    // model data
    std::vector<Mesh> m_Meshes;
    std::string m_Directory;

    void LoadModel(std::string path);
    void ProcessNode(aiNode* node, const aiScene* scene);
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<TexturePtr> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};