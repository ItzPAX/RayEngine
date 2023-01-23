#include "Ray.h"

FPSCamera::FPSCamera(const CameraDesc& cdesc, const FPSCameraDesc& fpsdesc)
	: Camera(cdesc), m_Sensitivity(fpsdesc.m_Sensitivity)
{
	m_Up = glm::vec3(0.f, 1.f, 0.f);
	m_Yaw = -90.f;
	m_Pitch = 0.f;

	// calc lookat vector
	glm::vec2 v(0.f);
	OnMouseMoved(&v);
	Update();
}

void FPSCamera::Think(glm::vec2* mousevel, float delta)
{
	RECT clipr = UI::Instance().SceneRect();
	bool sceneactive = UI::Instance().SceneActive();

	if (GetForegroundWindow() != m_Handle || !sceneactive)
		return;

	ManageSpeed();
	UpdateProjection(glm::vec2(clipr.right - clipr.left, clipr.bottom - clipr.top));

	// input system to walk around
	if (GetAsyncKeyState(0x57)) // W
		MoveFront(delta);
	if (GetAsyncKeyState(0x41)) // A
		MoveSideways(-delta);
	if (GetAsyncKeyState(0x53)) // S
		MoveFront(-delta);
	if (GetAsyncKeyState(0x44)) // D
		MoveSideways(delta);


	if (sceneactive && GetAsyncKeyState(VK_RBUTTON))
	{
		if (!m_ChangedCursorVis)
		{
			ShowCursor(false);
			m_ChangedCursorVis = true;
		}

		ClipCursor(&clipr);
		OnMouseMoved(mousevel);
	}
	else
	{
		if (m_ChangedCursorVis)
		{
			ShowCursor(true);
			m_ChangedCursorVis = false;
		}
		ClipCursor(NULL);
	}

	Update();
}

void FPSCamera::OnMouseMoved(glm::vec2* v)
{
	m_Yaw += v->x * m_Sensitivity;
	m_Pitch -= v->y * m_Sensitivity;

	// reset our speed vecs
	v->x = 0; v->y = 0;

	NormalizePitch();
	NormalizeYaw();

	glm::vec3 front;
	front.x = cos(glm::radians(m_Pitch)) * cos(glm::radians(m_Yaw));
	front.y = sin(glm::radians(m_Pitch));
	front.z = cos(glm::radians(m_Pitch)) * sin(glm::radians(m_Yaw));

	m_LookAt = glm::normalize(front);
	Update();
}

void FPSCamera::Update()
{
	m_View = glm::lookAt(m_Position, m_Position + m_LookAt, m_Up);
	m_ViewProj = m_Projection * m_View;
}

void FPSCamera::NormalizePitch()
{
	if (m_Pitch > 89.f)
		m_Pitch = 89.f;
	if (m_Pitch < -89.f)
		m_Pitch = -89.f;
}

void FPSCamera::NormalizeYaw()
{
	m_Yaw = fmodf(m_Yaw, 360.f);
	if (m_Yaw < 0.f)
		m_Yaw += 360.f;
}

void FPSCamera::MoveFront(float amount)
{
	Translate(glm::normalize(glm::vec3(1.f, 0.f, 1.f) * m_LookAt) * amount); // so we cant go up and down by just looking at stuff
	Update();
}

void FPSCamera::MoveSideways(float amount)
{
	Translate(glm::normalize(glm::cross(m_LookAt, m_Up)) * amount);
	Update();
}