#include "Ray.h"

Camera::Camera(const CameraDesc& desc)
{
	m_Projection = glm::mat4(1.f);
	m_Projection = glm::perspective(desc.m_Fov / 2.f, desc.m_Width / desc.m_Height, 0.1f, 1000.f);
	m_View = glm::mat4(1.f);
	m_Position = glm::vec3(0.f);

	m_ActualSpeed = desc.m_Speed;

	m_Desc = desc;
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

void Camera::Update()
{
	m_ViewProj = m_Projection * m_View;
}