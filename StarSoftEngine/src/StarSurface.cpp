#include "StarSurface.h"

namespace Star
{
	StarSurface::StarSurface(StarDevice* pDevice) :
		m_pDevice(pDevice), m_nWidth(0), m_nHeight(0), m_bLocked(false),
		m_pPartialLockData(0), m_pData(0)
	{

	}

	StarSurface::~StarSurface()
	{

	}

	EStarResult StarSurface::Create(uint32 nWidth, uint32 nHeight, EColorFormat eColorFormat)
	{
		switch (eColorFormat)
		{
		case CFMT_R32:
			m_nFormatFloats = 1;
			break;
		case CFMT_R32G32:
			m_nFormatFloats = 2;
			break;
		case CFMT_R32G32B32:
			m_nFormatFloats = 3;
			break;
		case CFMT_R32G32B32A32:
			m_nFormatFloats = 4;
			break;
		default:
			FUNC_FAILING(L"StarSurface::Create: Invalide Color Format");
			return SR_FAILED;
		}

		m_eColorFormat = eColorFormat;
		m_nWidth = nWidth;
		m_nHeight = nHeight;
		m_nWidthMin1 = nWidth - 1;
		m_nHeightMin1 = nHeight - 1;

		m_pData = new float[m_nWidth * m_nHeight * m_nFormatFloats];

		if (!m_pData)
		{
			FUNC_FAILING(L"StarSurface::Create: create data failed.");
			return SR_FAILED;
		}

		return SR_OK;
	}

	EStarResult StarSurface::LockRect(void** out_ppData, const StarRectangle* in_pRect)
	{
		if (!out_ppData)
		{
			FUNC_FAILING(L"StarSurface::LockRect: out_ppData points to null.\n");
			return SR_FAILED;
		}

		if (m_bLocked || m_pPartialLockData)
		{
			return SR_OK;
		}

		if (!in_pRect)
		{
			*out_ppData = m_pData;
			m_bLocked = true;
			return SR_OK;
		}

		return SR_OK;
	}

	EStarResult StarSurface::UnlockRect()
	{
		if (m_bLocked)
		{
			m_bLocked = false;
			return SR_OK;
		}

		return SR_FAILED;
	}

	EStarResult StarSurface::Clear(const StarColor& color, const StarRectangle* pRect)
	{
		StarRectangle clearRect;
		if (pRect)
		{
			clearRect = *pRect;
		}
		else
		{
			clearRect.nLeft = 0;
			clearRect.nTop = 0;
			clearRect.nRight = m_nWidth;
			clearRect.nBottom = m_nHeight;
		}
		float32* pData;

		EStarResult rltPointer = LockRect((void**)&pData, 0);

		const uint32 nBridgeStep = (m_nWidth - clearRect.nRight) + clearRect.nLeft;

		switch (m_eColorFormat)
		{
		case CFMT_R32:
			{
						 float32* pCurData = &pData[clearRect.nTop * m_nWidth + clearRect.nLeft];
						 for (uint32 nY = clearRect.nTop; nY < clearRect.nBottom; ++nY, pCurData += nBridgeStep)
						 {
							 for (uint32 nX = clearRect.nLeft; nX < clearRect.nRight; ++nX, ++pCurData)
							 {
								 *pCurData = color.r;
							 }
						 }
			}
			break;
		case CFMT_R32G32B32A32:
			{
								  StarColor* pCurData = &((StarColor*)pData)[clearRect.nTop * m_nWidth + clearRect.nLeft];
								  for (uint32 nY = clearRect.nTop; nY < clearRect.nBottom; ++nY, ++pCurData)
								  {
									  *pCurData = color;
								  }
			}
		default:
			break;
		}

		UnlockRect();
		return SR_OK;
	}

	void StarSurface::SamplePoint(StarColor& out_Color, float32 fU, float32 fV)
	{
		const float32 fX = fU * m_nWidthMin1;
		const float32 fY = fV * m_nHeightMin1;
		const uint32 nPixelX = ftol(fX);
		const uint32 nPixelY = ftol(fY);

		switch (m_eColorFormat)
		{
		case Star::CFMT_R32:
			break;
		case Star::CFMT_R32G32:
			break;
		case Star::CFMT_R32G32B32:
			break;
		case Star::CFMT_R32G32B32A32:
		{
										const StarColor* pPixel = (const StarColor*)&m_pData[4 * (nPixelY * m_nWidth + nPixelX)];
										out_Color = *pPixel;
										break;
		}
		default:
			break;
		}
	}

	uint32 StarSurface::GetFormatFloats()
	{
		return m_nFormatFloats;
	}

	uint32 StarSurface::GetWidth()
	{
		return m_nWidth;
	}

	uint32 StarSurface::GetHeight()
	{
		return m_nHeight;
	}
}

