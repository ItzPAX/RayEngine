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
	static Camera& GetCam()
	{
		static Camera instance; // Guaranteed to be destroyed.
		// Instantiated on first use.
		return instance;
	}

public:
	Camera(Camera const&) = delete;
	void operator=(Camera const&) = delete;

public:
	Camera();
	void ManageSpeed();

	glm::vec3 Position() { return m_Position; }
	glm::mat4 GetViewProj() { return m_ViewProj; }
	void UpdateProjection(glm::vec2 newsize);
	void SetPosition(glm::vec3 position);

	virtual void Think(glm::vec2* mousevel, float delta);
	virtual void Translate(glm::vec3 v);
	virtual void Update();

public:
	CameraDesc m_Desc;
	glm::mat4 m_View;

protected:
	glm::vec3 m_Position;

	glm::mat4 m_Projection;
	glm::mat4 m_ViewProj;

	float m_ActualSpeed;

	HWND m_Handle;
};