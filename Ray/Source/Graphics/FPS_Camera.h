#pragma once

struct FPSCameraDesc
{
	float m_Sensitivity;
};

class RAY_API FPSCamera : public Camera
{
public:
	FPSCamera(const CameraDesc& cdesc, const FPSCameraDesc& fpsdesc);

	void Think(glm::vec2* mousevel, float delta) override;

	void OnMouseMoved(glm::vec2* v);
	void Update() override;

	void MoveFront(float amount);
	void MoveSideways(float amount);

	// getters
public:
	float Pitch() { return m_Pitch; }
	float Yaw() { return m_Yaw; }

private:
	void NormalizePitch();
	void NormalizeYaw();

protected:
	float m_Yaw;
	float m_Pitch;
	float m_Sensitivity;

	glm::vec3 m_LookAt;
	glm::vec3 m_Up;

	bool m_ChangedCursorVis = false;
};