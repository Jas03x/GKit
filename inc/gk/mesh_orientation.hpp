#ifndef MESH_ORIENTATION_HPP
#define MESH_ORIENTATION_HPP

#include <gk/matrix.hpp>

enum Orientation
{
	Y_UP = 0,
	Z_UP = 1,
	UNKNOWN = 2
};

const Matrix4F& GetOrientationMatrix(unsigned int orientation);

#endif // MESH_ORIENTATION_HPP