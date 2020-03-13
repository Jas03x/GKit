#include <gk/mesh_orientation.hpp>

static const Matrix4F ORIENTATION_ARRAY[] =
{
	// Y_UP
	{
		Vector4F(1.0f, 0.0f, 0.0f, 0.0f),
		Vector4F(0.0f, 1.0f, 0.0f, 0.0f),
		Vector4F(0.0f, 0.0f, 1.0f, 0.0f),
		Vector4F(0.0f, 0.0f, 0.0f, 1.0f)
	},
	// Z_UP
	{
		Vector4F(1.0f, 0.0f,  0.0f, 0.0f),
		Vector4F(0.0f, 0.0f, -1.0f, 0.0f),
		Vector4F(0.0f, 1.0f,  0.0f, 0.0f),
		Vector4F(0.0f, 0.0f,  0.0f, 1.0f)
	}
};

const Matrix4F& GetOrientationMatrix(unsigned int orientation)
{
	if (orientation >= Orientation::UNKNOWN)
	{
		orientation = 0;
	}

	return ORIENTATION_ARRAY[orientation];
}
