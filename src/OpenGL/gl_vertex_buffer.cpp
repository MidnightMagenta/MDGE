#include "../include/OpenGL/gl_vertex_buffer.hpp"

void mdge::gl::VertexBuffer::Create(const CreateInfo &createInfo) {
	m_buffer.Create(GL_ARRAY_BUFFER, createInfo.arraySize, createInfo.data, createInfo.usage);
	m_buffer.Bind();
	glCreateVertexArrays(1, &m_array);

	Bind();

	for (const auto &attribute : createInfo.attributes) {
		if (m_attribLocations.contains(attribute.location)) {
			throw std::runtime_error("Invalid vertex attribute locations");
		}
		m_attribLocations.insert(attribute.location);
		glVertexAttribPointer(attribute.location, attribute.valueCount, attribute.dataType, GL_FALSE,
							  createInfo.vertexSize, (void *) attribute.offset);
	}

	Enable();
}

void mdge::gl::VertexBuffer::Destroy() {
	Bind();
	Disable();
	glDeleteVertexArrays(1, &m_array);
}

void mdge::gl::VertexBuffer::Bind() {
	m_buffer.Bind();
	GLint id;
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &id);
	if (m_array != (GLuint) id) { glBindVertexArray(m_array); }
}

void mdge::gl::VertexBuffer::Unbind() {
	m_buffer.Unbind();
	glBindVertexArray(GL_NULL_HANDLE);
}

void mdge::gl::VertexBuffer::Enable() {
	for (GLuint location : m_attribLocations) { glEnableVertexAttribArray(location); }
}

void mdge::gl::VertexBuffer::Disable() {
	for (GLuint location : m_attribLocations) { glDisableVertexAttribArray(location); }
}
