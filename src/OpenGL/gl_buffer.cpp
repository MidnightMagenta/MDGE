#include "../include/OpenGL/gl_buffer.hpp"

void mdge::gl::Buffer::Create() { glCreateBuffers(1, &m_buffer); }

void mdge::gl::Buffer::Create(GLenum target, GLsizeiptr size, const void *pData) {
	Create();
	m_target = target;
	m_usage = GL_STATIC_DRAW;
	m_size = size;
	glBufferData(target, size, pData, GL_STATIC_DRAW);
}

void mdge::gl::Buffer::Create(GLenum target, GLsizeiptr size, const void *pData, GLenum usage) {
	Create();
	m_target = target;
	m_usage = usage;
	m_size = size;
	glBufferData(target, size, pData, usage);
}

void mdge::gl::Buffer::Destroy() { glDeleteBuffers(1, &m_buffer); }

void mdge::gl::Buffer::RecreateBuffer(GLenum target, GLsizeiptr size, const void *pData) {
	m_target = target;
	m_usage = GL_STATIC_DRAW;
	m_size = size;
	glBufferData(target, size, pData, GL_STATIC_DRAW);
}

void mdge::gl::Buffer::RecreateBuffer(GLenum target, GLsizeiptr size, const void *pData, GLenum usage) {
	m_target = target;
	m_usage = usage;
	m_size = size;
	glBufferData(target, size, pData, usage);
}

void mdge::gl::Buffer::RewriteBuffer(GLsizeiptr size, const void *pData) {
	m_size = size;
	glBufferData(m_target, size, pData, m_usage);
}

void mdge::gl::Buffer::WriteData(GLsizeiptr size, const void *pData) const {
	assert(size <= m_size);
	glBufferSubData(m_target, 0, size, pData);
}

void mdge::gl::Buffer::WriteDataAtLocation(GLsizeiptr size, GLintptr offset, const void *pData) const {
	assert(size <= m_size);
	glBufferSubData(m_target, offset, size, pData);
}
