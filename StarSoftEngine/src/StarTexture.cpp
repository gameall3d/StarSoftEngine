#include "StarTexture.h"
#include "StarDevice.h"
#include "StarSurface.h"

namespace Star
{
	StarTexture::StarTexture(StarDevice* pDevice)
		:m_pDevice(pDevice), m_pSurfaceData(0)
	{
	}


	StarTexture::~StarTexture()
	{
	}

	EStarResult StarTexture::Create(uint32 nWidth, uint32 nHeight, EColorFormat eColorFormat)
	{

		EStarResult rltCreateSurface = m_pDevice->CreateSurface(&m_pSurfaceData, nWidth, nHeight, eColorFormat);

		return SR_OK;
	}

	EStarResult StarTexture::LockRect(void** out_ppData, const StarRectangle* pRect)
	{
		return m_pSurfaceData->LockRect(out_ppData, pRect);
	}

	EStarResult StarTexture::UnlockRect()
	{
		return m_pSurfaceData->UnlockRect();
	}

	EStarResult StarTexture::SampleTexture(StarColor& out_Color, float32 fU, float32 fV)
	{
		m_pSurfaceData->SamplePoint(out_Color, fU, fV);

		return SR_OK;
	}
}

