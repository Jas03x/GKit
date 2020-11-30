#include <gk/view_frustum.hpp>

ViewFrustum* ViewFrustum::Instance = nullptr;

ViewFrustum::ViewFrustum()
{
}

ViewFrustum* ViewFrustum::GetInstance()
{
	return Instance;
}

void ViewFrustum::Bind()
{
	Instance = this;
}
