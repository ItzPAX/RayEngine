#pragma once

// has all features of the FPS camera but can also fly up and down (like minecraft creative mode)
class RAY_API FloatingCamera : public FPSCamera
{
public:
	static  FloatingCamera& GetFloatingCam()
	{
		static FloatingCamera instance; // Guaranteed to be destroyed.
		// Instantiated on first use.
		return instance;
	}

public:
	FloatingCamera(FloatingCamera const&) = delete;
	void operator=(FloatingCamera const&) = delete;

public:
	FloatingCamera();

	void MoveUp(float amount);
	void Think(glm::vec2* mousevel, float delta) override;
};