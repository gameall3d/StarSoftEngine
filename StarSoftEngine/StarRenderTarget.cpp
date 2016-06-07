#include "StarRenderTarget.h"

namespace Star
{
	StarRenderTarget::StarRenderTarget(StarDevice* pDevice):
		m_pDevice(pDevice)
	{
	}


	StarRenderTarget::~StarRenderTarget()
	{
	}

	EStarResult StarRenderTarget::SetColorBuffer(StarSurface* in_pColorBuffer)
	{
		SAFE_DELETE(m_pColorBuffer);
		m_pColorBuffer = in_pColorBuffer;

		return SR_OK;
	}

	StarSurface* StarRenderTarget::GetColorBuffer()
	{
		return m_pColorBuffer;
	}

	EStarResult StarRenderTarget::SetDepthBuffer(StarSurface* in_pDepthBuffer)
	{
		SAFE_DELETE(m_pDepthBuffer);
		m_pDepthBuffer = in_pDepthBuffer;

		return SR_OK;
	}

	StarSurface* StarRenderTarget::GetDepthBuffer()
	{
		return m_pDepthBuffer;
	}
}


