#ifndef GK_VIEW_FRUSTUM_HPP
#define GK_VIEW_FRUSTUM_HPP

#include <gk/frustum.hpp>

class ViewFrustum : public Frustum
{
private:
	static ViewFrustum* Instance;

public:
	ViewFrustum();

	void Bind();

public:
	static ViewFrustum* GetInstance();
};

#endif /* GK_VIEW_FRUSTUM_HPP */
