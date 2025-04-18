#ifndef MDGE_TRANSFORM_COMPONENT_H
#define MDGE_TRANSFORM_COMPONENT_H

#include "../include/object.hpp"
#include <MidnightMath/include/md_math.hpp>

#define MDGE_TRANSFORM_POSITIVE_X mdm::vec3(1, 0, 0)
#define MDGE_TRANSFORM_NEGATIVE_X mdm::vec3(-1, 0, 0)
#define MDGE_TRANSFORM_POSITIVE_Y mdm::vec3(0, 1, 0);
#define MDGE_TRANSFORM_NEGATIVE_Y mdm::vec3(0, -1, 0);
#define MDGE_TRANSFORM_POSITIVE_Z mdm::vec3(0, 0, 1);
#define MDGE_TRANSFORM_NEGATIVE_Z mdm::vec3(0, 0, -1);

#define MDGE_TRANSFORM_RIGHT MDGE_TRANSFORM_POSITIVE_X
#define MDGE_TRANSFORM_LEFT MDGE_TRANSFORM_NEGATIVE_X
#define MDGE_TRANSFORM_FORWARD MDGE_TRANSFORM_POSITIVE_Y
#define MDGE_TRANSFORM_BACK MDGE_TRANSFORM_NEGATIVE_Y
#define MDGE_TRANSFORM_UP MDGE_TRANSFORM_POSITIVE_Z
#define MDGE_TRANSFORM_DOWN MDGE_TRANSFORM_NEGATIVE_Z

namespace mdge {
class Transform : public mdge::Component {
public:
	Transform() {}
	~Transform() {}

	mdm::vec3 &Position() { return m_position; }
	mdm::vec3 &Rotation() { return m_rotation; }
	mdm::vec3 &Scale() { return m_scale; }

private:
	mdm::vec3 m_position = {0, 0, 0};
	mdm::vec3 m_rotation = {0, 0, 0};
	mdm::vec3 m_scale = {1, 1, 1};
};
}// namespace mdge

#endif// !MDGE_TRANSFORM_COMPONENT_H
