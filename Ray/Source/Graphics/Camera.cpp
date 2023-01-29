#include "Ray.h"

Camera::Camera()
{
	HWND desktop = GetDesktopWindow();
	RECT desktoprect; GetWindowRect(desktop, &desktoprect);

	CameraDesc desc = 
	{
				90.f,							// FOV
				3.f,							// SPEED
				0.1f,							// NEAR
				1000.f,							// FAR
				glm::vec3(0.f, 2.f, 8.f)		// START POS
	};

	m_Projection = glm::mat4(1.f);
	m_Projection = glm::perspective(desc.m_Fov / 2.f, float(desktoprect.right - desktoprect.left) / float(desktoprect.bottom - desktoprect.top), desc.m_Near, desc.m_Far);
	m_View = glm::mat4(1.f);
	m_Position = desc.m_StartPosition;

	SetPosition(m_Position);

	m_Handle = GetForegroundWindow();
	m_ActualSpeed = desc.m_Speed;
	m_Desc = desc;
	Update();
}

void Camera::Think(glm::vec2* mousevel, float delta)
{
	ManageSpeed();

	if (GetForegroundWindow() != m_Handle || !UI::Instance().SceneActive())
		return;

	// input system to walk around
	if (GetAsyncKeyState(0x57)) // W
		Translate(glm::vec3(0.f,0.f,-1.f) * delta);
	if (GetAsyncKeyState(0x41)) // A
		Translate(glm::vec3(-1.f, 0.f, 0.f) * delta);
	if (GetAsyncKeyState(0x53)) // S
		Translate(glm::vec3(0.f, 0.f, 1.f) * delta);
	if (GetAsyncKeyState(0x44)) // D
		Translate(glm::vec3(1.f, 0.f, 0.f) * delta);

	RECT clipr = UI::Instance().SceneRect();

	UpdateProjection(glm::vec2(clipr.right - clipr.left, clipr.bottom - clipr.top));

	Update();
}

void Camera::UpdateProjection(glm::vec2 newsize)
{
	m_Projection = glm::perspective(m_Desc.m_Fov / 2.f, newsize.x / newsize.y, m_Desc.m_Near, m_Desc.m_Far);
	Update();
}

void Camera::ManageSpeed()
{
	if (GetAsyncKeyState(VK_CONTROL))
	{
		m_ActualSpeed = m_Desc.m_Speed * 2.f;
	}
	else
	{
		m_ActualSpeed = m_Desc.m_Speed;
	}
}

void Camera::Translate(glm::vec3 v)
{
	m_Position += v * m_ActualSpeed;
	m_View = glm::translate(m_View, v * -1.f * m_ActualSpeed);
}

void Camera::SetPosition(glm::vec3 pos)
{
	m_Position = pos;
	m_View = glm::translate(m_View, pos * -1.f);
}

void Camera::Update()
{
	m_ViewProj = m_Projection * m_View;
}