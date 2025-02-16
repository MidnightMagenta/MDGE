#ifndef MDGE_OPENGL_WINDOW_H
#define MDGE_OPENGL_WINDOW_H

#include "../include/windows/win32_window.hpp"
#include <../dependencies/OpenGL/include/OpenGL.hpp>

namespace mdge::gl {
namespace proc {
inline PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;
inline PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = nullptr;
inline PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = nullptr;
}// namespace proc

class OpenGL_Window : public mdge::win32_window {
public:
	struct CreateInfo {
		win32_window::CreateInfo windowCreateInfo;
		uint32_t OpenGL_major = 4;
		uint32_t OpenGL_minor = 6;
		uint32_t flags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		uint8_t pixelType = PFD_TYPE_RGBA;
		uint8_t colorBits = 32;
		uint8_t depthBits = 24;
		uint8_t stencilBits = 8;
		uint8_t msaaSamples = 8;
	};

	OpenGL_Window() {}
	OpenGL_Window(const CreateInfo &createInfo) { Create(&createInfo); }
	~OpenGL_Window() {
		wglMakeCurrent(m_deviceContext, nullptr);
		wglDeleteContext(m_context);
		ReleaseDC(mdge::win32_window::m_window, m_deviceContext);
	}

	void Create(const CreateInfo *pCreateInfo);

	void EnableVSync(bool enable);
	bool MakeCurrent() const { return wglMakeCurrent(m_deviceContext, m_context); }

private:
	static bool LoadContextCreationProcedures();
	static bool LoadExtraProcedures();

	HDC m_deviceContext = nullptr;
	HGLRC m_context = nullptr;
};
}// namespace mdge::gl

#endif// !MDGE_OPENGL_WINDOW_H
