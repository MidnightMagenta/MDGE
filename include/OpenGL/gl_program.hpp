#ifndef MDGE_OPENGL_PROGRAM_H
#define MDGE_OPENGL_PROGRAM_H

#include "../include/file.hpp"
#include <../dependencies/OpenGL/include/OpenGL.hpp>
#include <map>
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
		glDeleteShader(m_shader);
		m_shader = GL_NULL_HANDLE;
	}

	GLuint Handle() const { return m_shader; }

private:
	GLuint m_shader = GL_NULL_HANDLE;
};

enum class UniformVariableType { MDGE_GL_UNIFORM_VARIABLE, MDGE_GL_UNIFORM_BLOCK };

class Program {
public:
	struct CreateInfo {
		std::vector<std::pair<std::string, GLenum>> shaderInfos;
		std::vector<std::pair<std::string, UniformVariableType>> uniformVariables;
	};

	Program() {}
	~Program() { Delete(); }

	void Create(const CreateInfo *pCreateInfo);
	void Delete() {
		glDeleteProgram(m_program);
		m_program = GL_NULL_HANDLE;
	}

	void Bind() const {
		GLint id = 0;
		glGetIntegerv(GL_CURRENT_PROGRAM, &id);
		if (m_program != GLuint(id)) { glUseProgram(m_program); }
	}
	void Unbind() const { glUseProgram(0); }

	std::map<std::string, std::pair<GLint, UniformVariableType>> m_uniforms;

protected:
	GLuint m_program = GL_NULL_HANDLE;
};
}// namespace mdge::gl

#endif// !MDGE_OPENGL_PROGRAM_H
