#include "Ray.h"
#include "glfwSubObject.h"

namespace glfw {

	SubObject::SubObject(STRING classTitle, HICON icon)
		: m_Title(classTitle), m_Icon(icon)
	{

	}

	SubObject::~SubObject()
	{
	}
}