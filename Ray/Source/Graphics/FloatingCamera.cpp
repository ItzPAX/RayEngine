#include "Ray.h"

FloatingCamera::FloatingCamera(const CameraDesc& cdesc, const FPSCameraDesc& fpsdesc)
	: FPSCamera(cdesc, fpsdesc)
{

}

void FloatingCamera::MoveUp(float amount)
{
	Translate(m_Up * amount);
}

void FloatingCamera::Think(glm::vec2* mousevel, RECT clipr, bool sceneactive, HWND handle, float delta)
{
	if (GetAsyncKeyState(VK_SPACE))
		MoveUp(delta);
	if (GetAsyncKeyState(VK_SHIFT))
		MoveUp(-delta);

	FPSCamera::Think(mousevel, clipr, sceneactive, handle, delta);
}