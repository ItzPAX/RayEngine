#pragma once

// has all features of the FPS camera but can also fly up and down (like minecraft creative mode)
class RAY_API FloatingCamera : public FPSCamera
{
public:
	FloatingCamera(const CameraDesc& cdesc, const FPSCameraDesc& fpsdesc);

	void MoveUp(float amount);
	void Think(glm::vec2* mousevel, float delta) override;
};