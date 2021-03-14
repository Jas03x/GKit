#include <gk/3d/core/view.hpp>

#include <cassert>
#include <cstdio>

View* View::Instance = NULL;

View::View(unsigned int width, unsigned int height)
{
	m_Projection[0] = Vector3F( 2.0f / static_cast<float>(width), 0.0f,   0.0f);
	m_Projection[1] = Vector3F( 0.0f,  2.0f / static_cast<float>(height), 0.0f);
	m_Projection[2] = Vector3F(-1.0f, -1.0f, 1.0f);
}

View::~View()
{

}

void View::CreateInstance(unsigned int width, unsigned int height)
{
	if (Instance == NULL)
	{
		Instance = new View(width, height);
	}
}

void View::DeleteInstance()
{
	if (Instance)
	{
		delete Instance;
		Instance = NULL;
	}
}

View* View::GetInstance()
{
	assert(Instance != nullptr);

	if (Instance)
	{
		return Instance;
	}

	return nullptr;
}

const Matrix3F& View::GetMatrix() const
{
	return m_Projection;
}
