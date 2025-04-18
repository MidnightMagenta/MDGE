#include "../include/glb_loader.hpp"
#include <iostream>

void mdge::glb::Loader::Load(std::ifstream &file) {
	file.seekg(0);
	mdge::glb::Header header{};
	file.read((char *) &header, sizeof(mdge::glb::Header));
	if (header.magic != GLB_MAGIC) { throw std::runtime_error("GLB: Invalid magic"); }
	if (header.version != 2) { throw std::runtime_error("GLB: Versions other than 2.0 unsupported"); }

	mdge::glb::Chunk jsonChunk;
	file.read((char *) &jsonChunk.header, sizeof(mdge::glb::ChunkHeader));
	if (jsonChunk.header.type != GLB_JSON_CHUNK) { throw std::runtime_error("GLB: First chunk must be JSON"); }
	jsonChunk.data = new char[jsonChunk.header.length];
	file.read(jsonChunk.data, jsonChunk.header.length);

	nlohmann::json jsonParser;
	try {
		std::string jsonData(jsonChunk.data, jsonChunk.header.length);
		jsonParser = nlohmann::json::parse(jsonData);
	} catch (...) {
		delete[] jsonChunk.data;
		throw;
	}

	delete[] jsonChunk.data;
}
