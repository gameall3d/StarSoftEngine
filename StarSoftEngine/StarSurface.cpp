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

		m_nWidth = nWidth;
		m_nHeight = nHeight;

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

	}

	EStarResult StarSurface::UnlockRect()
	{
		if (m_bLocked)
		{
			m_bLocked = false;
			return SR_OK;
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

