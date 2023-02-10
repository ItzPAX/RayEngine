#include "Ray.h"

FloatingCamera::FloatingCamera()
	: FPSCamera()
{

}

void FloatingCamera::MoveUp(float amount)
{
	Translate(m_Up * amount);
}

void FloatingCamera::Think(glm::vec2* mousevel, float delta)
{
	if (!UI::Instance().SceneActive() && Engine::GetMode() == EDITOR)
		return;

	if (GetForegroundWindow() != m_Handle)
		return;

	if (GetAsyncKeyState(VK_SPACE))
		MoveUp(delta);
	if (GetAsyncKeyState(VK_SHIFT))
		MoveUp(-delta);

	FPSCamera::Think(mousevel, delta);
}