#include "../include/file.hpp"

bool mdge::file::LoadPlainText(std::string path, std::string *data) {
	std::ifstream file(path, std::ios::ate);
	if (!file.is_open()) { return false; }
	size_t size = file.tellg();
	data->resize(size);
	file.seekg(0);
	file.read(data->data(), size);
	return true;
}

size_t mdge::file::LoadBinary(std::string path, char *data) {
	std::ifstream file(path, std::ios::binary | std::ios::ate);
	size_t size = file.tellg();
	data = new char[size];
	file.seekg(0);
	file.read(data, size);
	return size;
}
