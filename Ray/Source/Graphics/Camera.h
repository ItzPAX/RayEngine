#pragma once
#define FOV_KEEP -1

struct CameraDesc
{
	float m_Fov;
	float m_Width;
	float m_Height;
};

class RAY_API Camera
{
public:
	Camera(const CameraDesc& desc);

	glm::mat4 GetViewProj() { return m_ViewProj; }
	void UpdateProjection(float height, float width, float fov = FOV_KEEP);
	void Translate(glm::vec3 v);
	void Update();

private:
	glm::mat4 m_Projection;
	glm::mat4 m_View;
	glm::mat4 m_ViewProj;

	CameraDesc m_Desc;
};