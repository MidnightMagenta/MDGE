#include <iostream>
#include <limits>
#include <OpenGL/gl_program.hpp>
#include <OpenGL/gl_window.hpp>
#include <windows/win32_console.hpp>
#include <OpenGL/gl_vertex_buffer.hpp>

std::vector<float> triangleVertices = {
		0.0f,  0.5f,  0.0f,
		-0.5f, -0.5f, 0.0f,
		0.5f,  -0.5f, 0.0f
};

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

	mdge::gl::Program program;
	program.Create(&programCreateInfo);

	mdge::gl::VertexBuffer buffer;
	mdge::gl::VertexBuffer::CreateInfo bufferCreateInfo{};
	bufferCreateInfo.arraySize = 9 * sizeof(float);
	bufferCreateInfo.vertexSize = 3 * sizeof(float);
	bufferCreateInfo.attributes = {{0, 3, GL_FLOAT, 0}};
	bufferCreateInfo.data = triangleVertices.data();
	buffer.Create(bufferCreateInfo);

	window.SetClearColor({255, 0, 128});
	window.EnableVSync(false);

	while (window.IsOpen()) { 
		window.Update(); 
		window.ClearColorBuffer();
		program.Bind();
		buffer.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 3);
		window.Present();
	}
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