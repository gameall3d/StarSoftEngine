#include "StarDevice.h"
#include "StarSurface.h"
#include "StarRenderInfo.h"
#include "StarVector2.h"
#include "StarColor.h"
#include "StarRenderTarget.h"
#include "StarPresentTarget.h"

namespace Star
{
	StarDevice::StarDevice(Star3D* in_pStar3D, const StarDevice_Parameters* in_pDeviceParameters)
	{
		m_pRenderInfo = new StarRenderInfo();

		memcpy(&m_DeviceParameters, in_pDeviceParameters, sizeof(StarDevice_Parameters));

		for (int i = 0; i < STST_NUM; i++)
		{
			m_TransformMatrix[i] = StarMatrix44::IDENTITY;
		}
	}

	StarDevice::~StarDevice()
	{
		SAFE_DELETE(m_pRenderInfo);
	}

	EStarResult StarDevice::Create()
	{
#ifdef WIN32
		m_pPresentTarget = new StarPresentTargetWin32(this);
#endif // WIN32

		if (!m_pPresentTarget)
		{
			return SR_FAILED;
		}

		return m_pPresentTarget->Create();
	}

	EStarResult StarDevice::CreateSurface(StarSurface** out_ppSurface, uint32 nWidth, uint32 nHeight, EColorFormat eColorFormat)
	{
		if (!out_ppSurface)
		{
			return SR_FAILED;
		}

		*out_ppSurface = new StarSurface(this);

		if (!(*out_ppSurface))
		{
			return SR_FAILED;
		}

		EStarResult rltCreate = (*out_ppSurface)->Create(nWidth, nHeight, eColorFormat);
		if (FUNC_FAILED(rltCreate))
		{
			return rltCreate;
		}

		return SR_OK;
	}

	void StarDevice::DrawTriangle(StarVertexData* pV0, StarVertexData* pV1, StarVertexData* pV2)
	{
		StarVector4 wvpPos0, wvpPos1, wvpPos2, homPos0, homPos1, homPos2;
		StarVector2 ScreenPos0, ScreenPos1, ScreenPos2;
		wvpPos0 = pV0->pos * m_WVPMatrix;
		wvpPos1 = pV1->pos * m_WVPMatrix;
		wvpPos2 = pV2->pos * m_WVPMatrix;

		if (CheckCVV(&wvpPos0) != 0) return;
		if (CheckCVV(&wvpPos1) != 0) return;
		if (CheckCVV(&wvpPos2) != 0) return;

		Homoginize(&wvpPos0, &homPos0);
		Homoginize(&wvpPos1, &homPos1);
		Homoginize(&wvpPos2, &homPos2);

		ScreenPos0.x = homPos0.x;
		ScreenPos0.y = homPos0.y;
		ScreenPos1.x = homPos1.x;
		ScreenPos1.y = homPos1.y;
		ScreenPos2.x = homPos2.x;
		ScreenPos2.y = homPos2.y;

		RasterizeTriangle(&ScreenPos0, &ScreenPos1, &ScreenPos2);
	}
	
	int StarDevice::CheckCVV(StarVector4* pPos)
	{
		float w = pPos->w;
		int check = 0;

		if (pPos->z < 0.0f) check |= 1;
		if (pPos->z > w)	check |= 2;
		if (pPos->x < -w)	check |= 4;
		if (pPos->x > w)	check |= 8;
		if (pPos->y < -w)	check |= 16;
		if (pPos->y > w)	check |= 32;

		return check;
	}

	void StarDevice::Homoginize(const StarVector4* in_pPos, StarVector4* out_pPos)
	{
		float rhw = 1.0f / in_pPos->w;

		out_pPos->x = (in_pPos->x * rhw + 1.0f) * m_DeviceParameters.nBackBufferWidth * 0.5f;
		out_pPos->y = (1.0f - in_pPos->y * rhw) * m_DeviceParameters.nBackBufferHeight * 0.5f;
		out_pPos->z = in_pPos->x * rhw;
		out_pPos->w = 1.0f;
	}

	void StarDevice::RasterizeTriangle(StarVector2* pPos0, StarVector2* pPos1, StarVector2* pPos2)
	{
		// sort Position by y
		const StarVector2* pVertices[3] = { pPos0, pPos1, pPos2 };

		if (pPos1->y < pVertices[0]->y)
		{
			pVertices[1] = pVertices[0];
			pVertices[0] = pPos1;
		}
		if (pPos2->y < pVertices[0]->y)
		{
			pVertices[2] = pVertices[1];
			pVertices[1] = pVertices[0];
			pVertices[0] = pPos2;
		}
		else if (pPos2->y < pVertices[0]->y)
		{
			pVertices[2] = pVertices[1];
			pVertices[1] = pPos2;
		}

		// pos references
		// type:0		1
		//		A       A
		//	   / |      | \
		//	  /  |      |  \
		//	  B  |      |   B
		//	  \  |      |  /
		//     \ |	    | /
		//		 C		 C
		const StarVector2& vA = *pVertices[0];
		const StarVector2& vB = *pVertices[1];
		const StarVector2& vC = *pVertices[2];

		// reciprocal of slope, 1/(y/x)
		const float fRecSlopeX[3] = {
			(vB.y - vA.y > 0.0f) ? (vB.x - vA.x) / (vB.y - vA.y) : 0.0f,
			(vC.y - vA.y > 0.0f) ? (vC.x - vA.x) / (vC.y - vA.y) : 0.0f,
			(vC.y - vB.y > 0.0f) ? (vC.x - vB.x) / (vC.y - vB.y) : 0.0f
		};

		// begin rasterization
		float fLeftEdge = vA.x;
		float fRightEdge = vA.x;
		for (int nPart = 0; nPart < 2; ++nPart)
		{
			int nStartY, nEndY;
			float fDeltaXLeft, fDeltaXRight;

			switch (nPart)
			{
			case 0:	// Draw upper part of triangle, decide upper slope and begin point
			{
						nStartY = ftol(ceilf(vA.y));
						nEndY = ftol(ceilf(vB.y));

						// left reciprocal of slope must small than right
						// type 1
						if (fRecSlopeX[0] > fRecSlopeX[1])	// decide the left or right
						{
							fDeltaXLeft = fRecSlopeX[1];
							fDeltaXRight = fRecSlopeX[0];
						}
						else  // type 0
						{
							fDeltaXLeft = fRecSlopeX[0];
							fDeltaXRight = fRecSlopeX[1];
						}

						// consider the difference because of the ceil operation
						const float fCeilDiffY = (float)nStartY - vA.y;
						fLeftEdge += fDeltaXLeft * fCeilDiffY;
						fRightEdge += fDeltaXRight * fCeilDiffY;
			}
				break;

			case 1:	// Draw lower part of triangle, decide down slope and begin point
			{
						nEndY = ftol(ceilf(vC.y));

						const float fCeilDiffY = (float)nStartY - vB.y;
						// type 1
						if (fRecSlopeX[1] > fRecSlopeX[2])
						{
							fDeltaXLeft = fRecSlopeX[1];
							fDeltaXRight = fRecSlopeX[2];
							fRightEdge = vB.x + fDeltaXRight * fCeilDiffY;
						}
						else
						{
							fDeltaXLeft = fRecSlopeX[2];
							fDeltaXRight = fRecSlopeX[1];
							fLeftEdge = vB.x + fDeltaXLeft * fCeilDiffY;
						}
			}
				break;
			default:
				break;
			}

			for (; nStartY < nEndY; nStartY++, fLeftEdge += fDeltaXLeft, fRightEdge += fDeltaXRight)
			{
				const int nLeftEdge = ftol(ceilf(fLeftEdge));
				const int nRightEdge = ftol(ceilf(fRightEdge));
				RasterizeScanline(nStartY, nLeftEdge, nRightEdge);
			}

		} // end of two parts for loop
	}

	void StarDevice::RasterizeScanline(int32 nYPos, int32 nStartXPos, int32 nEndXpos)
	{
		float32* pFrameData = m_pRenderInfo->m_pFrameData + (nYPos * m_pRenderInfo->m_nColorBufferPitch + nStartXPos * m_pRenderInfo->m_nColorFloats);

		StarColor pixelColor = StarColor::Green;
		for (; nStartXPos < nEndXpos; nStartXPos++, pFrameData += m_pRenderInfo->m_nColorFloats)
		{
			switch (m_pRenderInfo->m_nColorFloats)
			{
			case 4:pFrameData[3] = pixelColor.a;
			case 3:pFrameData[2] = pixelColor.b;
			case 2:pFrameData[1] = pixelColor.g;
			case 1:pFrameData[0] = pixelColor.r;
			}
		}
	}

	EStarResult StarDevice::PreRender()
	{
		StarSurface* pColorBuffer = m_pRenderTarget->GetColorBuffer();

		if (pColorBuffer)
		{
			EStarResult rltBuffer = pColorBuffer->LockRect((void**)&m_pRenderInfo->m_pFrameData, 0);

			m_pRenderInfo->m_nColorFloats = pColorBuffer->GetFormatFloats();
			m_pRenderInfo->m_nColorBufferPitch = pColorBuffer->GetWidth() * m_pRenderInfo->m_nColorFloats;
		}

		return SR_OK;
	}

	void StarDevice::PostRender()
	{
		if (m_pRenderInfo->m_pFrameData)
		{
			StarSurface* pColorBuffer = m_pRenderTarget->GetColorBuffer();
			if (pColorBuffer)
				pColorBuffer->UnlockRect();
		}
	}

	EStarResult StarDevice::Present(StarRenderTarget* in_pRenderTarget)
	{
		if (!in_pRenderTarget)
		{
			return SR_INVALIDPARAM;
		}

		StarSurface* pColorBuffer = in_pRenderTarget->GetColorBuffer();
		if (!pColorBuffer)
		{
			return SR_FAILED;
		}

		if (pColorBuffer->GetWidth() != m_DeviceParameters.nBackBufferWidth ||
			pColorBuffer->GetHeight() != m_DeviceParameters.nBackBufferHeight)
		{
			return SR_FAILED;
		}

		const uint32 nColorFloats = pColorBuffer->GetFormatFloats();
		const float32* pSource;
		if (FUNC_FAILED(pColorBuffer->LockRect((void**)&pSource, 0)))
		{
			return SR_UKNOWN;
		}

		EStarResult rltPresent = m_pPresentTarget->Present(pSource, nColorFloats);

		pColorBuffer->UnlockRect();

		return rltPresent;
	}

	void StarDevice::SetTransform(EStarTransformStateType eTransformState, StarMatrix44* mat)
	{
		m_TransformMatrix[eTransformState] = *mat;
		m_WVPMatrix = m_TransformMatrix[STST_WORLD] * m_TransformMatrix[STST_VIEW] * m_TransformMatrix[STST_PROJECTION];
	}

	const StarDevice_Parameters& StarDevice::GetDeviceParameters()
	{
		return m_DeviceParameters;
	}

	void StarDevice::SetRenderTarget(StarRenderTarget* in_pRenderTarget)
	{
		m_pRenderTarget = in_pRenderTarget;
	}

	StarRenderTarget* StarDevice::GetRenderTarget()
	{
		return m_pRenderTarget;
	}
}