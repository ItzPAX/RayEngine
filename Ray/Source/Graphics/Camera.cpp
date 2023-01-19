#include "Ray.h"

Camera::Camera(const CameraDesc& desc)
{
	m_Projection = glm::mat4(1.f);
	m_Projection = glm::perspective(desc.m_Fov / 2.f, desc.m_Width / desc.m_Height, 0.1f, 1000.f);
	m_View = glm::mat4(1.f);

	m_Desc = desc;

	Update();
}

void Camera::Translate(glm::vec3 v)
{
	m_View = glm::translate(m_View, v * -1.f);
}

void Camera::Update()
{
	m_ViewProj = m_Projection * m_View;
}

void Camera::UpdateProjection(float height, float width, float fov)
{
	float f;
	f = fov == FOV_KEEP ? m_Desc.m_Fov : fov;

	m_Projection = glm::mat4(1.f);
	m_Projection = glm::perspective(f / 2.f, width / height, 0.1f, 1000.f);

	Update();
}