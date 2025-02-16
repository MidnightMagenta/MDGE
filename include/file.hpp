#ifndef MDGE_FILE_H
#define MDGE_FILE_H

#include <fstream>
#include <string>

namespace mdge::file {
bool LoadPlainText(std::string path, std::string *data);
size_t LoadBinary(std::string path, char *data);
}

#endif// !MDGE_FILE_H
