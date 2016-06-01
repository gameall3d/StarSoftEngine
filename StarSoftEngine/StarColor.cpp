#include "StarColor.h"

namespace Star
{
	const StarColor StarColor::ZERO		= StarColor(0.0, 0.0, 0.0, 0.0);
	const StarColor StarColor::Black	= StarColor(0.0, 0.0, 0.0);
	const StarColor StarColor::White	= StarColor(1.0, 1.0, 1.0);
	const StarColor StarColor::Red		= StarColor(1.0, 0.0, 0.0);
	const StarColor StarColor::Green	= StarColor(0.0, 1.0, 0.0);
	const StarColor StarColor::Blue		= StarColor(0.0, 0.0, 1.0);

	RGBA StarColor::GetAsRGBA(void) const
	{
		uint8 val8;
		uint32 val32 = 0;

		// Red
		val8 = static_cast<uint8>(r * 255);
		val32 = val8 << 24;

		// Green
		val8 = static_cast<uint8>(g * 255);
		val32 += val8 << 16;

		// Blue
		val8 = static_cast<uint8>(b * 255);
		val32 += val8 << 8;

		// Alpha
		val8 = static_cast<uint8>(a * 255);
		val32 += val8;

		return val32;
	}

	ARGB StarColor::GetAsARGB(void) const
	{
		uint8 val8;
		uint32 val32 = 0;

		// Alpha
		val8 = static_cast<uint8>(a * 255);
		val32 = val8 << 24;

		// Red
		val8 = static_cast<uint8>(r * 255);
		val32 += val8 << 16;

		// Green
		val8 = static_cast<uint8>(g * 255);
		val32 += val8 << 8;

		// Blue
		val8 = static_cast<uint8>(b * 255);
		val32 += val8;

		return val32;
	}

	void StarColor::SetAsRGBA(const ARGB val)
	{
		uint32 val32 = val;

		// Red
		r = ((val32 >> 24) & 0xFF) / 255.0f;

		// Green
		g = ((val32 >> 16) & 0xFF) / 255.0f;

		// Blue
		b = ((val32 >> 8) & 0xFF) / 255.0f;

		// Alpha
		a = (val32 & 0xFF) / 255.0f;
	}

	void StarColor::SetAsARGB(const ARGB val)
	{
		uint32 val32 = val;

		// Alpha
		a = ((val32 >> 24) & 0xFF) / 255.0f;

		// Red
		r = ((val32 >> 16) & 0xFF) / 255.0f;

		// Green
		g = ((val32 >> 8) & 0xFF) / 255.0f;

		// Blue
		b = (val32 & 0xFF) / 255.0f;
	}
}

