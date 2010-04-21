#ifndef GLG_MESH_PLANE_INCLUDED
#define GLG_MESH_PLANE_INCLUDED

#include "mesh.hpp"

namespace glv
{
	mesh buildPlane(
		glm::uint channels,
		glm::vec2 const & Size);

}//namespace glv

#include "plane.inl"

#endif//GLG_MESH_PLANE_INCLUDED
