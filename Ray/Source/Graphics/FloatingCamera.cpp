#include "Ray.h"

FloatingCamera::FloatingCamera(const CameraDesc& cdesc, const FPSCameraDesc& fpsdesc)
	: FPSCamera(cdesc, fpsdesc)
{

}

void FloatingCamera::MoveUp(float amount)
{
	Translate(m_Up * amount);
}

void FloatingCamera::Think(glm::vec2* mousevel, float delta)
{
	if (GetForegroundWindow() != m_Handle || !UI::Instance().SceneActive())
		return;

	if (GetAsyncKeyState(VK_SPACE))
		MoveUp(delta);
	if (GetAsyncKeyState(VK_SHIFT))
		MoveUp(-delta);

	FPSCamera::Think(mousevel, delta);
}