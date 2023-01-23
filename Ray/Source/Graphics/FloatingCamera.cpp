#include "Ray.h"

FloatingCamera::FloatingCamera(const CameraDesc& cdesc, const FPSCameraDesc& fpsdesc)
	: FPSCamera(cdesc, fpsdesc)
{

}

void FloatingCamera::MoveUp(float amount)
{
	Translate(m_Up * amount);
}

void FloatingCamera::Think(glm::vec2* mousevel, HWND handle, float delta)
{
	if (GetForegroundWindow() != handle || !UI::Instance().SceneActive())
		return;

	if (GetAsyncKeyState(VK_SPACE))
		MoveUp(delta);
	if (GetAsyncKeyState(VK_SHIFT))
		MoveUp(-delta);

	FPSCamera::Think(mousevel, handle, delta);
}