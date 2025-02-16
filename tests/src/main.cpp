#include <OpenGL/gl_program.hpp>
#include <OpenGL/gl_window.hpp>
#include <iostream>
#include <limits>

int run() {
	mdge::gl::OpenGL_Window window;
	mdge::gl::OpenGL_Window::CreateInfo createInfo{};
	createInfo.windowCreateInfo.size = {800, 800};
	createInfo.windowCreateInfo.name = "Test window";
	createInfo.windowCreateInfo.style = WS_OVERLAPPEDWINDOW;
	createInfo.windowCreateInfo.color = {255, 255, 255};
	window.Create(&createInfo);
	window.MakeCurrent();

	mdge::gl::Shader testShader;
	testShader.Create("../resources/shaders/sample_shader.frag", GL_FRAGMENT_SHADER);

	mdge::gl::Program::CreateInfo programCreateInfo{};
	programCreateInfo.shaderInfos = {{"../resources/shaders/sample_shader.frag", GL_FRAGMENT_SHADER},
									 {"../resources/shaders/sample_shader.vert", GL_VERTEX_SHADER}};

	mdge::gl::Program program;
	program.Create(&programCreateInfo);

	while (window.IsOpen()) { window.Update(); }
	return 0;
}

int main() {
	try {
		return run();
	} catch (const std::exception &e) {
		std::cerr << e.what() << "\n";
		return -1;
	}
}