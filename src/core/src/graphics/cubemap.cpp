#include <gk/core/graphics/cubemap.hpp>

#include <gk/core/importer/bitmap.hpp>

CubeMap::CubeMap()
{
	m_Texture = 0;
}

CubeMap::CubeMap(const Bitmap* bitmaps)
{
	this->Load(bitmaps);
}

CubeMap::~CubeMap()
{
	if (m_Texture != nullptr)
	{
		delete m_Texture;
	}
}

bool CubeMap::Load(const Bitmap* bitmaps)
{
	const RenderingContext* context = RenderingContext::GetInstance();

	m_Texture = new CubeMapTexture(bitmaps, GFX_LINEAR, GFX_CLAMP_TO_EDGE);

	return true;
}

void CubeMap::Bind(unsigned int uniform, int target) const
{
	m_Texture->Bind(uniform, target);
}

CubeMapTexture* CubeMap::GetTexture() const
{
	return m_Texture;
}
