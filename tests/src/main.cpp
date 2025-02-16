#include <iostream>
#include <limits>
#include <OpenGL/OpenGL_window.hpp>

int run() {
	mdge::gl::OpenGL_Window window;
	mdge::gl::OpenGL_Window::CreateInfo createInfo{};
	createInfo.windowCreateInfo.size = {800, 800};
	createInfo.windowCreateInfo.name = "Test window";
	createInfo.windowCreateInfo.style = WS_OVERLAPPEDWINDOW;
	createInfo.windowCreateInfo.color = {255, 255, 255};
	window.Create(&createInfo);
	window.MakeCurrent();

	while (window.IsOpen()) { window.Update(); }
	return 0;
}

int main() {
	try {
		return run();
	} catch (const std::exception &e) {
		MessageBox(nullptr, e.what(), "Error", MB_ICONERROR | MB_OK);
		return -1;
	}
}