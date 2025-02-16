#ifndef MDGE_OPENGL_PROGRAM_H
#define MDGE_OPENGL_PROGRAM_H

#include "../include/file.hpp"
#include <../dependencies/OpenGL/include/OpenGL.hpp>
#include <string>
#include <utility>
#include <vector>

namespace mdge::gl {
class Shader {
public:
	Shader() {}
	Shader(std::string shaderPath, GLuint shaderUsage) { Create(shaderPath, shaderUsage); }
	~Shader() { Delete(); }

	void Create(std::string shaderPath, GLenum shaderUsage);
	void Delete() {
		if (m_shader != GL_NULL_HANDLE) { glDeleteShader(m_shader); }
		m_shader = GL_NULL_HANDLE;
	}

	GLuint Handle() const { return m_shader; }

private:
	GLuint m_shader = GL_NULL_HANDLE;
};

class Program {
public:
	struct CreateInfo {
		std::vector<std::pair<std::string, GLenum>> shaderInfos;
	};

	Program() {}
	~Program() { Delete(); }

	void Create(const CreateInfo *pCreateInfo);
	void Delete() {
		if (m_program) { glDeleteProgram(m_program); }
		m_program = GL_NULL_HANDLE;
	}

protected:
	GLuint m_program = GL_NULL_HANDLE;
};
}// namespace mdge::gl

#endif// !MDGE_OPENGL_PROGRAM_H
