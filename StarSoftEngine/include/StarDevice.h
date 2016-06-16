#ifndef __STARDEVICE_H_
#define __STARDEVICE_H_

#include "StarPrerequisites.h"
#include "StarTypes.h"
#include "StarMatrix44.h"
#include "FreeImage/FreeImage.h"

namespace Star
{
	class StarDevice
	{
	protected:
		friend class Star3D;
		StarDevice(Star3D* in_pStar3D, const StarDevice_Parameters* in_pDeviceParameters);
		~StarDevice();
		
		EStarResult Create();

	public:
		EStarResult CreateSurface(StarSurface** out_ppSurface, uint32 nWidth, uint32 nHeight, EColorFormat eColorFormat);
		EStarResult CreateTexture(StarTexture** out_ppTexture, uint32 nWidth, uint32 nHeight, EColorFormat eColorFormat);
		EStarResult CreateTextureFromFile(StarTexture** out_ppTexture, std::string srcFile);
		EStarResult SetTexture(uint32 nSamplerNum, StarTexture* pTexture);

		EStarResult PreRender();
		EStarResult Present(StarRenderTarget* in_pRenderTarget);
		void PostRender();

		void DrawTriangle(StarVertexData* pV0, StarVertexData* pV1, StarVertexData* pV2);
		int32 CheckCVV(StarVector4* pPos);
		void Homoginize(const StarVector4* in_pPos, StarVector4* out_pPos);
		void RasterizeTriangle(StarVertexData* pPos0, StarVertexData* pPos1, StarVertexData* pPos2);
		void RasterizeTriangleSimple(StarVertexData* pPos0, StarVertexData* pPos1, StarVertexData* pPos2);
		void RasterizeScanline(int32 nYPos, int32 nStartXPos, int32 nEndXpos, StarColor startColor, StarColor endColor);
		void RasterizeScanline(int32 nYPos, StarScanLineVertexData* pStartVD, StarScanLineVertexData* pEndVD);
		void ProcessScanLine(int32 nYPos, const StarVertexData* pAVD, const StarVertexData* pBVD, const StarVertexData* pCVD, const StarVertexData* pDVD);

		void SetTransform(EStarTransformStateType eTransformState, StarMatrix44* mat);


		const StarDevice_Parameters& GetDeviceParameters();
		void SetRenderTarget(StarRenderTarget* in_pRenderTarget);
		StarRenderTarget* GetRenderTarget();

	private:
		StarDevice_Parameters m_DeviceParameters;
		StarPresentTarget* m_pPresentTarget;
		StarRenderTarget* m_pRenderTarget;
		StarRenderInfo* m_pRenderInfo;
		StarTexture*	m_pTexture;

		StarMatrix44 m_TransformMatrix[STST_NUM];
		StarMatrix44 m_WVPMatrix;
	};
}


#endif // end of __SARTDEVICE_H_

