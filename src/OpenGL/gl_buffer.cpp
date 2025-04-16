#include "../include/OpenGL/gl_buffer.hpp"

void mdge::gl::Buffer::Create() { glCreateBuffers(1, &m_buffer); }

void mdge::gl::Buffer::Create(GLenum target, GLsizeiptr size, const void *pData) {
	Create();
	m_target = target;
	m_usage = GL_STATIC_DRAW;
	m_size = size;
	Bind();
	glBufferData(target, size, pData, GL_STATIC_DRAW);
}

void mdge::gl::Buffer::Create(GLenum target, GLsizeiptr size, const void *pData, GLenum usage) {
	Create();
	m_target = target;
	m_usage = usage;
	m_size = size;
	Bind();
	glBufferData(target, size, pData, usage);
}

void mdge::gl::Buffer::Destroy() { 
	glDeleteBuffers(1, &m_buffer); 
	m_buffer = GL_NULL_HANDLE;
}

void mdge::gl::Buffer::Resize(GLsizeiptr size) {
	void *data = new char[m_size];
	Bind();
	glGetBufferSubData(m_target, 0, m_size, data);
	RecreateBuffer(m_target, size, data, m_usage);
	delete[] data;
}

void mdge::gl::Buffer::RecreateBuffer(GLenum target, GLsizeiptr size, const void *pData) {
	m_target = target;
	m_usage = GL_STATIC_DRAW;
	m_size = size;
	Bind();
	glBufferData(target, size, pData, GL_STATIC_DRAW);
}

void mdge::gl::Buffer::RecreateBuffer(GLenum target, GLsizeiptr size, const void *pData, GLenum usage) {
	m_target = target;
	m_usage = usage;
	m_size = size;
	Bind();
	glBufferData(target, size, pData, usage);
}

void mdge::gl::Buffer::RewriteBuffer(GLsizeiptr size, const void *pData) {
	m_size = size;
	Bind();
	glBufferData(m_target, size, pData, m_usage);
}

void mdge::gl::Buffer::Overwrite(GLsizeiptr size, const void *pData) const {
	assert(size <= m_size);
	Bind();
	glBufferSubData(m_target, 0, size, pData);
}

void mdge::gl::Buffer::Overwrite(GLsizeiptr size, GLintptr offset, const void *pData) const {
	assert(size <= m_size);
	Bind();
	glBufferSubData(m_target, offset, size, pData);
}

void mdge::gl::Buffer::Bind() const {
	if (m_buffer != GetBoundBuffer(m_target)) { glBindBuffer(m_target, m_buffer); }
}

GLuint mdge::gl::Buffer::GetBoundBuffer(GLenum target) {
	GLint id;
	switch (target) {
		case GL_ARRAY_BUFFER:
			glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &id);
			break;
		case GL_ATOMIC_COUNTER_BUFFER:
			glGetIntegerv(GL_ATOMIC_COUNTER_BUFFER_BINDING, &id);
			break;
		case GL_COPY_READ_BUFFER:
			glGetIntegerv(GL_COPY_READ_BUFFER_BINDING, &id);
			break;
		case GL_COPY_WRITE_BUFFER:
			glGetIntegerv(GL_COPY_WRITE_BUFFER_BINDING, &id);
			break;
		case GL_DISPATCH_INDIRECT_BUFFER:
			glGetIntegerv(GL_DISPATCH_INDIRECT_BUFFER_BINDING, &id);
			break;
		case GL_DRAW_INDIRECT_BUFFER:
			glGetIntegerv(GL_DRAW_INDIRECT_BUFFER_BINDING, &id);
			break;
		case GL_ELEMENT_ARRAY_BUFFER:
			glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &id);
			break;
		case GL_PIXEL_PACK_BUFFER:
			glGetIntegerv(GL_PIXEL_PACK_BUFFER_BINDING, &id);
			break;
		case GL_PIXEL_UNPACK_BUFFER:
			glGetIntegerv(GL_PIXEL_UNPACK_BUFFER_BINDING, &id);
			break;
		case GL_QUERY_BUFFER:
			glGetIntegerv(GL_QUERY_BUFFER_BINDING, &id);
			break;
		case GL_SHADER_STORAGE_BUFFER:
			glGetIntegerv(GL_SHADER_STORAGE_BUFFER_BINDING, &id);
			break;
		case GL_TEXTURE_BUFFER:
			glGetIntegerv(GL_TEXTURE_BUFFER_BINDING, &id);
			break;
		case GL_TRANSFORM_FEEDBACK_BUFFER:
			glGetIntegerv(GL_TRANSFORM_FEEDBACK_BUFFER_BINDING, &id);
			break;
		case GL_UNIFORM_BUFFER:
			glGetIntegerv(GL_UNIFORM_BUFFER_BINDING, &id);
			break;
		default:
			id = GL_NULL_HANDLE;
			break;
	}
	return (GLuint) id;
}

GLsizeiptr mdge::gl::Buffer::ElementSize(GLenum type) { 
	switch (type) {
		case GL_BYTE:
			return sizeof(GLbyte);
		case GL_UNSIGNED_BYTE:
			return sizeof(GLubyte);
		case GL_SHORT:
			return sizeof(GLshort);
		case GL_UNSIGNED_SHORT:
			return sizeof(GLushort);
		case GL_INT:
			return sizeof(GLint);
		case GL_UNSIGNED_INT:
			return sizeof(GLuint);
		case GL_HALF_FLOAT:
			return sizeof(GLhalf);
		case GL_FLOAT:
			return sizeof(GLfloat);
		case GL_DOUBLE:
			return sizeof(GLdouble);
		case GL_FIXED:
			return sizeof(GLfixed);
		default:
			return -1;
			break;
	}
}
