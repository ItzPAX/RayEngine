#pragma once

namespace glfw {

	class RAY_API Window : public glfw::Caption, public glfw::SubObject {

	public:
		Window(STRING title, HICON icon, WindowType type = RESIZEABLE);
		~Window();

		virtual VOID Initialize() override;

	protected:
		static VOID FramebufferSizeCallback(GLFWwindow* window, int width, int height);

		VOID Update() {
			glfwMakeContextCurrent(Handle());
			while (!glfwWindowShouldClose(Handle()))
			{
				glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT);
			
				// Swap buffer
				glfwSwapBuffers(Handle());
			
				// queue events
				glfwPollEvents();
			}
			glfwTerminate();
		}

	protected:
		SIZE		m_Size;
		WindowType	m_Type;

		BOOL		m_Active;

		// Getters
	public:
		SIZE		Size() { return m_Size; }
		BOOL		Active() { return m_Active; }

		// Setters
	public:
		VOID		Size(SIZE size) { m_Size = size; }
		VOID		Size(INT cx, INT cy) { m_Size.cx = cx; m_Size.cy = cy; }
		VOID		Active(BOOL active) { m_Active = active; }
	};
}