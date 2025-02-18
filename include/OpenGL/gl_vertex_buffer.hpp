#ifndef MDGE_OPENGL_VERTEX_BUFFER_H
#define MDGE_OPENGL_VERTEX_BUFFER_H

#include "../include/OpenGL/gl_buffer.hpp"
#include <OpenGL/include/OpenGL.hpp>
#include <set>
#include <stdexcept>
#include <vector>

namespace mdge::gl {
struct VertexAttribute {
	GLuint location;
	GLint valueCount;
	GLenum dataType;
	size_t offset;
};

class VertexBuffer {
public:
	struct CreateInfo {
		std::vector<VertexAttribute> attributes;
		size_t vertexSize = 0;
		size_t arraySize = 0;
		const void *data = nullptr;
		GLenum usage = GL_STATIC_DRAW;
	};

	VertexBuffer() {};
	VertexBuffer(const CreateInfo &createInfo) { Create(createInfo); }
	~VertexBuffer() { Destroy(); };

	void Create(const CreateInfo &createInfo);
	void Destroy();

	void Bind();
	void Unbind();

	void Enable();
	void Disable();

protected:
	Buffer m_buffer;
	GLuint m_array = GL_NULL_HANDLE;
	std::set<GLuint> m_attribLocations;
};
}// namespace mdge::gl

#endif// !MDGE_OPENGL_VERTEX_BUFFER_H
