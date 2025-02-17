#include <iostream>
#include <limits>
#include <OpenGL/gl_program.hpp>
#include <OpenGL/gl_window.hpp>
#include <windows/win32_console.hpp>

int run() {
	mdge::gl::OpenGL_Window window;
	mdge::gl::OpenGL_Window::CreateInfo createInfo{};
	createInfo.windowCreateInfo.size = {800, 800};
	createInfo.windowCreateInfo.name = "Test window";
	createInfo.windowCreateInfo.style = WS_OVERLAPPEDWINDOW;
	createInfo.windowCreateInfo.color = {255, 255, 255};
	window.Create(&createInfo);
	window.MakeCurrent();

	mdge::gl::Program::CreateInfo programCreateInfo{};
	programCreateInfo.shaderInfos = {{"../resources/shaders/sample_shader.frag", GL_FRAGMENT_SHADER},
									 {"../resources/shaders/sample_shader.vert", GL_VERTEX_SHADER}};
	programCreateInfo.uniformVariables = {{"uniformVar", mdge::gl::UniformVariableType::MDGE_GL_UNIFORM_VARIABLE},
										  {"uniformVar2", mdge::gl::UniformVariableType::MDGE_GL_UNIFORM_VARIABLE},
										  {"uniformBlock", mdge::gl::UniformVariableType::MDGE_GL_UNIFORM_BLOCK}};

	mdge::gl::Program program;
	program.Create(&programCreateInfo);

	while (window.IsOpen()) { window.Update(); }
	return 0;
}

int main() {
	try {
		return run();
	} catch (const std::exception &e) {
		mdge::setConsoleColor(CONSOLE_RED);
		std::cerr << "Application error: " << e.what() << "\n";
		mdge::setConsoleColor(CONSOLE_WHITE);
		return -1;
	}
}