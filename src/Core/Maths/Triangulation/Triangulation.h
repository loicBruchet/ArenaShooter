#ifndef CORE_TRIANGULATION_H_INCLUDED
#define CORE_TRIANGULATION_H_INCLUDED



#include "define.h"

namespace gce
{
	std::vector<Vector2f32> ProjectToPlane(std::vector<Vector3f32> const& points);

	std::vector<uint32> Triangulate(std::vector<Vector2f32> const& pos);
}

#endif