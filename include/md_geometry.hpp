#ifndef MDGE_MD_GEOMETRY_H
#define MDGE_MD_GEOMETRY_H

#include <inttypes.h>
#define MDG_MAGIC 0x4D444745

namespace mdge::mdg {
#pragma pack(push, 1)
struct FileHeader {
	uint32_t magic = MDG_MAGIC;
	uint32_t version = 1;
	uint32_t flags = 0;
	uint32_t objHeaderCount;
	uint64_t objHeaderOffset;
};

struct ObjectHeader {
	char name[64];
	uint64_t vertexCount;
	uint64_t vertexOffset;
	uint64_t indexCount;
	uint64_t indexOffset;
};

struct MDGVert {
	float position[3];
	float normal[3];
	float texCoord[2];
};

typedef uint32_t MDGIndex;

#pragma pack(pop)
}// namespace mdge::mdg

#endif// !MDGE_MD_GEOMETRY_H
