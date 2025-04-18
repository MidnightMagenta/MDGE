#include "../include/glb_loader.hpp"

void mdge::gltf::Loader::Load(std::filesystem::path path) {
	if (path.extension() == ".glb") { 
		std::ifstream file(path, std::ios::binary | std::ios::beg);
		LoadGLB(file);
	} else if (path.extension() == ".gltf") {
		throw std::runtime_error("GLTF: gltf not supported yet");
	} else {
		throw std::runtime_error("GLTF: Invalid file");
	}
}

void mdge::gltf::Loader::LoadGLB(std::ifstream &file) {
	mdge::gltf::Header header{};
	file.read((char *) &header, sizeof(mdge::gltf::Header));
	if (header.magic != GLTF_MAGIC) { throw std::runtime_error("GLTF: Invalid magic"); }
	if (header.version != 2) { throw std::runtime_error("GLTF: Versions other than 2.0 unsupported"); }

	std::vector<mdge::gltf::Chunk> chunks;
	LoadChunks(file, header.length, &chunks);
	if (chunks[0].header.type != GLTF_JSON_CHUNK) {
		for (mdge::gltf::Chunk &chunk : chunks) { delete[] chunk.data; }
		throw std::runtime_error("GLTF: First chunk must be JSON");
	}

	nlohmann::json jsonParser;
	try {
		std::string jsonData(chunks[0].data, chunks[0].header.length);
		jsonParser = nlohmann::json::parse(jsonData);
	} catch (...) {
		delete[] chunks[0].data;
		throw;
	}

	for (mdge::gltf::Chunk &chunk : chunks) { delete[] chunk.data; }
}

void mdge::gltf::Loader::LoadChunks(std::ifstream &file, size_t fileSize, std::vector<mdge::gltf::Chunk> *chunks) {
	while (file.tellg() < fileSize) {
		mdge::gltf::Chunk chunk;
		file.read((char *) &chunk.header, sizeof(mdge::gltf::ChunkHeader));
		chunk.data = new char[chunk.header.length];
		file.read(chunk.data, chunk.header.length);
		chunks->push_back(chunk);
	}
}
