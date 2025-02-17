#include "../include/OpenGL/gl_program.hpp"

void mdge::gl::Shader::Create(std::string shaderPath, GLenum shaderUsage) {
	std::string shaderCode;
	mdge::file::LoadPlainText(shaderPath, &shaderCode);

	m_shader = glCreateShader(shaderUsage);
	char const *sourcePointer = shaderCode.c_str();
	glShaderSource(m_shader, 1, &sourcePointer, nullptr);
	glCompileShader(m_shader);

	GLint status = GL_FALSE;
	glGetShaderiv(m_shader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		GLsizei logLength = 0;
		char message[1024];
		glGetShaderInfoLog(m_shader, 1024, &logLength, message);
		std::string error(message);
		throw std::runtime_error(error);
	}
}

void mdge::gl::Program::Create(const CreateInfo *pCreateInfo) {
	std::vector<Shader> shaders(pCreateInfo->shaderInfos.size());
	for (size_t i = 0; i < shaders.size(); i++) {
		shaders[i].Create(pCreateInfo->shaderInfos[i].first, pCreateInfo->shaderInfos[i].second);
	}

	m_program = glCreateProgram();

	for (const auto &shader : shaders) { glAttachShader(m_program, shader.Handle()); }
	glLinkProgram(m_program);

	GLint status = GL_FALSE;
	glGetProgramiv(m_program, GL_LINK_STATUS, &status);
	if (status != GL_TRUE) {
		GLsizei logLength = 0;
		char message[1024];
		glGetProgramInfoLog(m_program, 1024, &logLength, message);
		std::string error(message);
		throw std::runtime_error(error);
	}

	for (auto &shader : shaders) { shader.Delete(); }

	for (const auto &uniformVariable : pCreateInfo->uniformVariables) {
		GLint uniformIndex = GL_NULL_HANDLE;
		switch (uniformVariable.second) {
			case mdge::gl::UniformVariableType::MDGE_GL_UNIFORM_VARIABLE:
				uniformIndex = glGetUniformLocation(m_program, uniformVariable.first.c_str());
				if (uniformIndex >= 0) {
					m_uniforms.insert(std::make_pair(
							uniformVariable.first,
							std::make_pair(uniformIndex, mdge::gl::UniformVariableType::MDGE_GL_UNIFORM_VARIABLE)));
				}
				break;
			case mdge::gl::UniformVariableType::MDGE_GL_UNIFORM_BLOCK:
				uniformIndex = glGetUniformBlockIndex(m_program, uniformVariable.first.c_str());
				if (uniformIndex >= 0) {
					m_uniforms.insert(std::make_pair(
							uniformVariable.first,
							std::make_pair(uniformIndex, mdge::gl::UniformVariableType::MDGE_GL_UNIFORM_BLOCK)));
				}
				break;
			default:
				break;
		}
	}
}
