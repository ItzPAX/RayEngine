#pragma once
#include <vector>

struct RAY_API Vertex
{
    glm::vec3 m_Position;
    glm::vec2 m_TexCoords;
    glm::vec3 m_Normal;
    glm::vec3 m_Tangent;
    glm::vec3 m_Bitangent;
};

class RAY_API Mesh
{
public:
    // mesh data
    std::vector<Vertex> m_Vertices;
    std::vector<unsigned int> m_Indices;
    std::vector<TexturePtr> m_Textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<TexturePtr> textures);
    void Draw(ShaderProgramPtr shader);
private:
    //  render data
    unsigned int m_VAO, m_VBO, m_EBO;

    void SetupMesh();
};