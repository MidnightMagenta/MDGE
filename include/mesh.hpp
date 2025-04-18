#ifndef MDGE_MESH_COMPONENT_H
#define MDGE_MESH_COMPONENT_H

#include "../include/object.hpp"
#include <filesystem>

#if MDGE_USE_VULKAN
//TODO: Vulkan implementation
#else
#include "../include/OpenGL/gl_vertex_buffer.hpp"
#endif

namespace mdge {
class Mesh : public mdge::Component {
public:
	Mesh() {}
	Mesh(const std::filesystem::path &path) {
		m_meshPath = path;
		LoadMesh();
	}
	~Mesh() {}

	void Initialize(const std::filesystem::path &path) {
		m_meshPath = path;
		LoadMesh();
	}

private:
	void LoadMesh();

	std::filesystem::path m_meshPath = "";
#if MDGE_USE_VULKAN
//TODO: Vulkan implementation
#else
	mdge::gl::VertexBuffer m_buffer;
#endif
};

class Material : public mdge::Component {
public:
private:
	
};
}// namespace mdge

#endif// !MDGE_MESH_COMPONENT_H
