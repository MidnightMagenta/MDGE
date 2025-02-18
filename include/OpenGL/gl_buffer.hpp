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

	void Resize(GLsizeiptr size);
	void RecreateBuffer(GLenum target, GLsizeiptr size, const void *pData);
	void RecreateBuffer(GLenum target, GLsizeiptr size, const void *pData, GLenum usage);
	void RewriteBuffer(GLsizeiptr size, const void *pData);

	void Overwrite(GLsizeiptr size, const void *pData) const;
	void Overwrite(GLsizeiptr size, GLintptr offset, const void *pData) const;

	void Bind() const;
	inline void Unbind() const { glBindBuffer(m_target, GL_NULL_HANDLE); }

	inline void GetData(GLsizeiptr size, void *pData) const { glGetBufferSubData(m_target, 0, size, pData); }

	static GLuint GetBoundBuffer(GLenum target);
	static GLsizeiptr ElementSize(GLenum type);

protected:
	GLenum m_target;
	GLenum m_usage;
	GLsizeiptr m_size;

	GLuint m_buffer = GL_NULL_HANDLE;
};
}// namespace mdge::gl

#endif// !MDGE_OPENGL_BUFFER_H
