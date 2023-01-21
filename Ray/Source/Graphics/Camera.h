#pragma once

struct CameraDesc
{
	float m_Fov;
	float m_Width;
	float m_Height;
	float m_Speed;
};

class RAY_API Camera
{
public:
	Camera(const CameraDesc& desc);
	void ManageSpeed();

	glm::mat4 GetViewProj() { return m_ViewProj; }

	virtual void Think(glm::vec2* mousevel, RECT clipr, bool sceneactive, HWND handle, float delta) = 0;
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