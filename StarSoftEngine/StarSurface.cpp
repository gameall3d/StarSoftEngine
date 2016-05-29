#include "StarSurface.h"

EStarResult StarSurface::Create(int nWidth, int nHeight, EColorFormat eColorFormat)
{
	int nFloats;

	switch (eColorFormat)
	{
	case CFMT_R32:
		nFloats = 1;
		break;
	case CFMT_R32G32:
		nFloats = 2;
		break;
	case CFMT_R32G32B32:
		nFloats = 3;
		break;
	case CFMT_R32G32B32A32:
		nFloats = 4;
		break;
	default:
		FUNC_FAILING("StarSurface::Create: Invalide Color Format");
		return SR_FAILED;
	}

	m_pData = new float[m_nWidth * m_nHeight * nFloats];

	if (!m_pData)
	{
		FUNC_FAILING("StarSurface::Create: create data failed.");
		return SR_FAILED;
	}

	return SR_OK;
}