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
		StarVertexData wvpVD0, wvpVD1, wvpVD2;
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

		wvpVD0.pos = homPos0;
		wvpVD1.pos = homPos1;
		wvpVD2.pos = homPos2;
//		wvpVD0.pos.z = wvpPos0.w;
//		wvpVD1.pos.z = wvpPos1.w;
//		wvpVD2.pos.z = wvpPos2.w;
		wvpVD0.color = pV0->color;
		wvpVD1.color = pV1->color;
		wvpVD2.color = pV2->color;

		RasterizeTriangle(&wvpVD0, &wvpVD1, &wvpVD2);
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
		out_pPos->z = in_pPos->z * rhw;
		out_pPos->w = 1.0f;
	}

	void StarDevice::RasterizeTriangle(StarVertexData* pPos0, StarVertexData* pPos1, StarVertexData* pPos2)
	{
		// sort Position by y
		const StarVertexData* pVertices[3] = { pPos0, pPos1, pPos2 };

		if (pPos1->pos.y < pVertices[0]->pos.y)
		{
			pVertices[1] = pVertices[0];
			pVertices[0] = pPos1;
		}
		if (pPos2->pos.y < pVertices[0]->pos.y)
		{
			pVertices[2] = pVertices[1];
			pVertices[1] = pVertices[0];
			pVertices[0] = pPos2;
		}
		else if (pPos2->pos.y < pVertices[1]->pos.y)
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
		const StarVector4& vA = pVertices[0]->pos;
		const StarVector4& vB = pVertices[1]->pos;
		const StarVector4& vC = pVertices[2]->pos;

		const StarColor& cA = pVertices[0]->color;
		const StarColor& cB = pVertices[1]->color;
		const StarColor& cC = pVertices[2]->color;


		// reciprocal of slope, 1/(y/x)
		const float32 fRecSlopeX[3] = {
			(vB.y - vA.y > 0.0f) ? (vB.x - vA.x) / (vB.y - vA.y) : 0.0f,
			(vC.y - vA.y > 0.0f) ? (vC.x - vA.x) / (vC.y - vA.y) : 0.0f,
			(vC.y - vB.y > 0.0f) ? (vC.x - vB.x) / (vC.y - vB.y) : 0.0f
		};

		// begin rasterization
		float32 fLeftEdge = vA.x;
		float32 fRightEdge = vA.x;
		StarColor leftColor = cA;
		StarColor rightColor = cA;
		StarColor fDeltaColorLeft, fDeltaColorRight;
		float32 fLeftZ = vA.z;
		float32 fRightZ = vA.z;
		

		for (uint32 nPart = 0; nPart < 2; ++nPart)
		{
			uint32 nStartY, nEndY;
			float32 fDeltaXLeft, fDeltaXRight;
			float32 fDeltaZLeft, fDeltaZRight;
			float32 fRecDiffY;
			

			switch (nPart)
			{
			case 0:	// Draw upper part of triangle, decide upper slope and begin point
			{
						nStartY = ftol(ceilf(vA.y));
						nEndY = ftol(ceilf(vB.y));

						if (nStartY == nEndY)
						{
							fRecDiffY = 1 / (float32)(ftol(ceilf(vC.y)) - nStartY);
							fDeltaColorLeft = (cC - cA) * fRecDiffY;
							fDeltaColorRight = (cC - cB) * fRecDiffY;
							fDeltaZLeft = (vC.z - vA.z) * fRecDiffY;
							fDeltaZRight = (vC.z - vB.z) * fRecDiffY;
							break;
						}
						
						// reciprocal of diffY
						fRecDiffY = 1 / (float32)(nEndY - nStartY);
						// left reciprocal of slope must small than right
						// type 1
						if (fRecSlopeX[0] > fRecSlopeX[1])	// decide the left or right
						{
							fDeltaXLeft = fRecSlopeX[1];
							fDeltaXRight = fRecSlopeX[0];
							fDeltaColorLeft = (cC - cA) * fRecDiffY;
							fDeltaColorRight = (cB - cA) * fRecDiffY;
							fDeltaZLeft = (vC.z - vA.z) * fRecDiffY;
							fDeltaZRight = (vB.z - vA.z) * fRecDiffY;

						}
						else  // type 0
						{
							fDeltaXLeft = fRecSlopeX[0];
							fDeltaXRight = fRecSlopeX[1];
							fDeltaColorLeft = (cB - cA) * fRecDiffY;
							fDeltaColorRight = (cC - cA) * fRecDiffY;
							fDeltaZLeft = (vB.z - vA.z) * fRecDiffY;
							fDeltaZRight = (vC.z - vA.z) * fRecDiffY;

						}

						// consider the difference because of the ceil operation
						const float fCeilDiffY = (float)nStartY - vA.y;
						fLeftEdge += fDeltaXLeft * fCeilDiffY;
						fRightEdge += fDeltaXRight * fCeilDiffY;
						leftColor += fDeltaColorLeft * fCeilDiffY;
						rightColor += fDeltaColorRight * fCeilDiffY;
						fLeftZ += fDeltaZLeft * fCeilDiffY;
						fRightZ += fDeltaZRight * fCeilDiffY;
			}
				break;

			case 1:	// Draw lower part of triangle, decide down slope and begin point
			{
						nEndY = ftol(ceilf(vC.y));

						if (nEndY == nStartY)
							return;

						fRecDiffY = 1.0f / (float)(nEndY - nStartY);
						const float fCeilDiffY = (float)nStartY - vB.y;
						// type 1
						if (fRecSlopeX[1] > fRecSlopeX[2])
						{
							fDeltaXLeft = fRecSlopeX[1];
							fDeltaXRight = fRecSlopeX[2];
							fRightEdge = vB.x + fDeltaXRight * fCeilDiffY;
							fDeltaColorRight = (cC - cB) * fRecDiffY;
							rightColor = cB + fDeltaColorRight * fCeilDiffY;
							fDeltaZRight = (vC.z - vB.z) * fRecDiffY;
							fRightZ = vB.z + fDeltaZRight * fCeilDiffY;
						}
						else
						{
							fDeltaXLeft = fRecSlopeX[2];
							fDeltaXRight = fRecSlopeX[1];
							fLeftEdge = vB.x + fDeltaXLeft * fCeilDiffY;
							fDeltaColorLeft = (cC - cB) * fRecDiffY;
							leftColor = cB + fDeltaColorLeft * fCeilDiffY;
							fDeltaZLeft = (vC.z - vB.z) * fRecDiffY;
							fLeftZ = vB.z + fDeltaZLeft * fCeilDiffY;
						}
			}
				break;
			default:
				break;
			}

			for (; nStartY < nEndY; nStartY++, fLeftEdge += fDeltaXLeft, fRightEdge += fDeltaXRight, 
				leftColor += fDeltaColorLeft, rightColor += fDeltaColorRight,
				fLeftZ += fDeltaZLeft, fRightZ += fDeltaZRight)
			{
				const int nLeftEdge = ftol(ceilf(fLeftEdge));
				const int nRightEdge = ftol(ceilf(fRightEdge));

				StarScanLineVertexData startVD, endVD;
				startVD.color = leftColor;
				startVD.nXPos = nLeftEdge;
				startVD.fZPos = fLeftZ;
				
				endVD.color = rightColor;
				endVD.nXPos = nRightEdge;
				endVD.fZPos = fRightZ;

				//RasterizeScanline(nStartY, nLeftEdge, nRightEdge, leftColor, rightColor);
				RasterizeScanline(nStartY, &startVD, &endVD);
			}

		} // end of two parts for loop
	}

	void StarDevice::RasterizeScanline(int32 nYPos, int32 nStartXPos, int32 nEndXpos, StarColor startColor, StarColor endColor)
	{
		float32* pFrameData = m_pRenderInfo->m_pFrameData + (nYPos * m_pRenderInfo->m_nColorBufferPitch + nStartXPos * m_pRenderInfo->m_nColorFloats);
		float32* pDepthData = m_pRenderInfo->m_pDepthData + (nYPos * m_pRenderInfo->m_nDepthBufferPitch + nStartXPos);

		StarColor deltaColor;
		int diff = nEndXpos - nStartXPos;
		StarColor pixelColor = startColor;
		if (diff != 0)
		{
			deltaColor = (endColor - startColor) / (float)(nEndXpos - nStartXPos);
		}
		
		
		for (; nStartXPos < nEndXpos; nStartXPos++, pFrameData += m_pRenderInfo->m_nColorFloats, pixelColor += deltaColor)
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

	void StarDevice::RasterizeScanline(int32 nYPos, StarScanLineVertexData* pStartVD, StarScanLineVertexData* pEndVD)
	{
		uint32 nStartXPos = pStartVD->nXPos;
		uint32 nEndXpos = pEndVD->nXPos;

		float32* pFrameData = m_pRenderInfo->m_pFrameData + (nYPos * m_pRenderInfo->m_nColorBufferPitch + nStartXPos * m_pRenderInfo->m_nColorFloats);
		float32* pDepthData = m_pRenderInfo->m_pDepthData + (nYPos * m_pRenderInfo->m_nDepthBufferPitch + nStartXPos);

		// for color
		StarColor deltaColor;
		float32 diffX = (float32)(nEndXpos - nStartXPos);
		StarColor pixelColor = pStartVD->color;

		// for depth
		float32 deltaZ;
		float32 fDepth = pStartVD->fZPos;

		if (diffX != 0)
		{
			deltaColor = (pEndVD->color - pStartVD->color) / diffX;

			deltaZ = (pEndVD->fZPos - pStartVD->fZPos) / diffX;
		}

		

		for (; nStartXPos < nEndXpos; nStartXPos++, pFrameData += m_pRenderInfo->m_nColorFloats, pixelColor += deltaColor, fDepth += deltaZ, ++pDepthData)
		{
			if (fDepth < *pDepthData)
			{
				*pDepthData = fDepth;
			}
			else
			{
				continue;
			}

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

		StarSurface* pDepthBuffer = m_pRenderTarget->GetDepthBuffer();

		if (pDepthBuffer)
		{
			EStarResult rltBuffer = pDepthBuffer->LockRect((void**)&m_pRenderInfo->m_pDepthData, 0);

			m_pRenderInfo->m_nDepthBufferPitch = pDepthBuffer->GetWidth();
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