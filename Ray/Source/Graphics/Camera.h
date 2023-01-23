#pragma once

struct CameraDesc
{
	float m_Fov;
	float m_Speed;
	float m_Near;
	float m_Far;

	glm::vec3 m_StartPosition;
};

class RAY_API Camera
{
public:
	Camera(const CameraDesc& desc);
	void ManageSpeed();

	glm::vec3 Position() { return m_Position; }
	glm::mat4 GetViewProj() { return m_ViewProj; }
	void UpdateProjection(glm::vec2 newsize);
	void SetPosition(glm::vec3 position);

	virtual void Think(glm::vec2* mousevel, HWND handle, float delta);
	virtual void Translate(glm::vec3 v);
	virtual void Update();

protected:
	glm::vec3 m_Position;

	glm::mat4 m_Projection;
	glm::mat4 m_View;
	glm::mat4 m_ViewProj;

	CameraDesc m_Desc;

protected:
	float m_ActualSpeed;
};