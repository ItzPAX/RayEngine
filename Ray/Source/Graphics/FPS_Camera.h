#pragma once

struct FPSCameraDesc
{
	float m_Sensitivity;
};

class RAY_API FPSCamera : public Camera
{
public:
	static FPSCamera& GetFPSCam()
	{
		static FPSCamera instance; // Guaranteed to be destroyed.
		// Instantiated on first use.
		return instance;
	}

public:
	FPSCamera(FPSCamera const&) = delete;
	void operator=(FPSCamera const&) = delete;

public:
	FPSCamera();

	void Think(glm::vec2* mousevel, float delta) override;

	void OnMouseMoved(glm::vec2* v);
	void Update() override;

	void MoveFront(float amount);
	void MoveSideways(float amount);

	// getters
public:
	float Pitch() { return m_Pitch; }
	float Yaw() { return m_Yaw; }
	glm::vec2 View() { return glm::vec2(m_Pitch, m_Yaw); }
	glm::vec3 Front() { return m_LookAt; }

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