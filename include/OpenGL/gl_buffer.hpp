#ifndef MDGE_OPENGL_BUFFER_H
#define MDGE_OPENGL_BUFFER_H

#include <OpenGL/include/OpenGL.hpp>
#include <cassert>

namespace mdge::gl {
class Buffer {
public:
	Buffer() { Create(); }
	~Buffer() { Destroy(); }

	void Create();
	void Create(GLenum target, GLsizeiptr size, const void *pData);
	void Create(GLenum target, GLsizeiptr size, const void *pData, GLenum usage);
	void Destroy();

	void RecreateBuffer(GLenum target, GLsizeiptr size, const void *pData);
	void RecreateBuffer(GLenum target, GLsizeiptr size, const void *pData, GLenum usage);
	void RewriteBuffer(GLsizeiptr size, const void *pData);

	void WriteData(GLsizeiptr size, const void *pData) const;
	void WriteDataAtLocation(GLsizeiptr size, GLintptr offset, const void *pData) const;
	
	void Bind() const { glBindBuffer(m_target, m_buffer); }
	void Unbind() const { glBindBuffer(m_target, GL_NULL_HANDLE); }

protected:
	GLenum m_target;
	GLenum m_usage;
	GLsizeiptr m_size;

	GLuint m_buffer = GL_NULL_HANDLE;
};
}// namespace mdge::gl

#endif// !MDGE_OPENGL_BUFFER_H
