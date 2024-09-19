#pragma once

class RAY_API Model
{
public:
    friend class ModelContainer;
    friend class UI;

    Model() = default;
    Model(std::string path, const ShaderProgramDesc& shader);
    void Draw(float dt);

public:
    glm::vec3 m_Position;
    glm::vec3 m_Rotation;
    
    float m_Scale = 1.f;

protected:
    glm::mat4 m_Model;
    int m_Index;

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
    void UpdateModel(float dt);
};

class RAY_API ModelContainer
{
public:
    void Render(float dt);
    ModelPtr Add(const char* path, const ShaderProgramDesc& shader);
    void Delete(ModelPtr model);

public:
    std::unordered_map<int, ModelPtr> m_Models;
};