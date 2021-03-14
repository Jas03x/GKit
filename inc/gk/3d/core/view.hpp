#ifndef GK_VIEW_H
#define GK_VIEW_H

#include <gk/core/math/linear.hpp>

class View
{
private:
	static View* Instance;

	Matrix3F m_Projection;

private:
	View(unsigned int width, unsigned int height);
	~View();

public:
	static void CreateInstance(unsigned int width, unsigned int height);
	static void DeleteInstance();
	static View* GetInstance();

	const Matrix3F& GetMatrix() const;
};

#endif // GK_VIEW_H
