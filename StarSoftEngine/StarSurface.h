#ifndef __STARSUFACE_H_
#define __STARSUFACE_H_

#include "StarBase.h"
#include "StarRectangle.h"

class StarSurface
{
public:
	EStarResult Create(int nWidth, int nHeight, EColorFormat eColorFormat);

	EStarResult LockRect(void** out_ppData, const StarRectangle* in_pRect);

private:
	EColorFormat	m_eColorFormat;
	int				m_nWidth;
	int				m_nHeight;
	

	float*			m_pData;
};

#endif // end of __STARSUFACE_H_