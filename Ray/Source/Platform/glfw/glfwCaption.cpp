#include "Ray.h"
#include "glfwCaption.h"

namespace glfw {

	VOID Caption::AddCaptionButton(CaptionButton* button)
	{
		m_CaptionButtons.push_back(button);
	}
}