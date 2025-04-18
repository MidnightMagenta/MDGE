#ifndef MDGE_GLB_LOADER_H
#define MDGE_GLB_LOADER_H

#include "../include/file.hpp"
#include <filesystem>
#include <nlohmann/json.hpp>
#include <vector>

#define GLTF_MAGIC 0x46546C67
#define GLTF_JSON_CHUNK 0x4E4F534A
#define GLTF_BINARY_CHUNK 0x004E4942

namespace mdge::gltf {
struct Header {
	uint32_t magic = 0;
	uint32_t version = 0;
	uint32_t length = 0;
};

struct ChunkHeader {
	uint32_t length = 0;
	uint32_t type = 0;
};

struct Chunk {
	ChunkHeader header;
	char *data = nullptr;
};

class Loader {
public:
	Loader() {}
	~Loader() {}

	void Load(std::filesystem::path path);

private:
	void LoadGLB(std::ifstream &file);
	void LoadChunks(std::ifstream &file, size_t fileSize, std::vector<mdge::gltf::Chunk> *chunks);
};

}// namespace mdge::glb

#endif// !MDGE_GLB_LOADER_H
