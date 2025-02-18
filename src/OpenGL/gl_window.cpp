#include "../include/OpenGL/gl_window.hpp"

LRESULT CALLBACK DummyProcess(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	return DefWindowProc(hWnd, message, wParam, lParam);
}

void mdge::gl::OpenGL_Window::Create(const CreateInfo *pCreateInfo) {
	WNDCLASSEX wc{};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpszClassName = "DUMMY_WINDOW";
	wc.lpfnWndProc = DummyProcess;
	RegisterClassEx(&wc);

	HWND dummyWindow = CreateWindowEx(0, "DUMMY_WINDOW", "DUMMY_WINDOW", 0, 0, 0, 0, 0, 0, 0, GetModuleHandleA(0), 0);
	if (!dummyWindow) {
		throw std::runtime_error("Failed to create OpenGL context.\nFailed to create dummy window");
		//TODO: Error handling
	}
	HDC dummyDevice = GetDC(dummyWindow);
	PIXELFORMATDESCRIPTOR pfd{};
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = pCreateInfo->flags;
	pfd.iPixelType = pCreateInfo->pixelType;
	pfd.cColorBits = pCreateInfo->colorBits;
	pfd.cDepthBits = pCreateInfo->depthBits;
	pfd.cStencilBits = pCreateInfo->stencilBits;

	int pixelFormat = ChoosePixelFormat(dummyDevice, &pfd);
	SetPixelFormat(dummyDevice, pixelFormat, &pfd);
	HGLRC dummyContext = wglCreateContext(dummyDevice);
	if (!dummyContext) {
		throw std::runtime_error("Failed to create OpenGL context.\nFailed to create dummy context.");
		//TODO: Error handling
	}
	if (!wglMakeCurrent(dummyDevice, dummyContext)) {
		throw std::runtime_error("Failed to create OpenGL context.\nFailed to make dummy context current.");
		//TODO: Error handling
	}

	if (!LoadContextCreationProcedures()) {
		throw std::runtime_error("Failed to create OpenGL context.\nFailed to load context creation procedures.");
		//TODO: Error handling
	}

	const int contextAttribs[] = {WGL_CONTEXT_MAJOR_VERSION_ARB, pCreateInfo->OpenGL_major,
								  //
								  WGL_CONTEXT_MINOR_VERSION_ARB, pCreateInfo->OpenGL_minor,
								  //
								  WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
								  //
								  0};
	const int pixelFormatAttribs[] = {
			WGL_DRAW_TO_WINDOW_ARB, (pCreateInfo->flags & PFD_DRAW_TO_WINDOW) ? GL_TRUE : GL_FALSE,
			//
			WGL_SUPPORT_OPENGL_ARB, (pCreateInfo->flags & PFD_SUPPORT_OPENGL) ? GL_TRUE : GL_FALSE,
			//
			WGL_DOUBLE_BUFFER_ARB, (pCreateInfo->flags & PFD_DOUBLEBUFFER) ? GL_TRUE : GL_FALSE,
			//
			WGL_COLOR_BITS_ARB, pCreateInfo->colorBits,
			//
			WGL_DEPTH_BITS_ARB, pCreateInfo->depthBits,
			//
			WGL_STENCIL_BITS_ARB, pCreateInfo->stencilBits,
			//
			WGL_SAMPLE_BUFFERS_ARB, 1,
			//
			WGL_SAMPLES_ARB, pCreateInfo->msaaSamples,
			//
			0};

	mdge::win32_window::Create(pCreateInfo->windowCreateInfo);

	int msPixelFormat;
	UINT formatCount;
	m_deviceContext = GetDC(mdge::win32_window::m_window);

	if (!proc::wglChoosePixelFormatARB(m_deviceContext, pixelFormatAttribs, nullptr, 1, &msPixelFormat, &formatCount)) {
		throw std::runtime_error("Failed to create OpenGL context.\nFailed to choose pixel format.");
		//TODO: Error handling
	}

	SetPixelFormat(m_deviceContext, msPixelFormat, &pfd);
	m_context = proc::wglCreateContextAttribsARB(m_deviceContext, nullptr, contextAttribs);
	if (!m_context) {
		throw std::runtime_error("Failed to create OpenGL context.");
		//TODO: Error handling
	}

	wglMakeCurrent(dummyDevice, nullptr);
	wglDeleteContext(dummyContext);
	ReleaseDC(dummyWindow, dummyDevice);
	DestroyWindow(dummyWindow);

	if (!wglMakeCurrent(m_deviceContext, m_context)) {
		throw std::runtime_error("Failed to create OpenGL context.\nFailed to make context current.");
		//TODO: Error handling
	}
	if (!gladLoadGL()) { 
		throw std::runtime_error("Failed to create OpenGL context.\nFailed to load procedures."); 
		//TODO: Error handling
	}

	LoadExtraProcedures();
}

void mdge::gl::OpenGL_Window::EnableVSync(bool enable) {
	if (!proc::wglSwapIntervalEXT) { return; }
	if (enable) {
		proc::wglSwapIntervalEXT(1);
	} else {
		proc::wglSwapIntervalEXT(0);
	}
}

LRESULT mdge::gl::OpenGL_Window::Process(UINT message, WPARAM wParam, LPARAM lParam) { 
	switch (message) {
		case WM_SIZE:
			if (glViewport != nullptr) { 
				mdm::uvec2_s size = GetClientSize();
				glViewport(0, 0, size.w(), size.h()); 
			}
			break;
		default:
			break;
	}
	return DefWindowProc(m_window, message, wParam, lParam);
}

bool mdge::gl::OpenGL_Window::LoadContextCreationProcedures() {
	proc::wglCreateContextAttribsARB =
			(PFNWGLCREATECONTEXTATTRIBSARBPROC) wglGetProcAddress("wglCreateContextAttribsARB");
	proc::wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC) wglGetProcAddress("wglChoosePixelFormatARB");
	if (!proc::wglCreateContextAttribsARB || !proc::wglChoosePixelFormatARB) { return false; }
	return true;
}

bool mdge::gl::OpenGL_Window::LoadExtraProcedures() {
	proc::wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC) wglGetProcAddress("wglSwapIntervalEXT");
	if (!proc::wglSwapIntervalEXT) { return false; }
	return true;
}
