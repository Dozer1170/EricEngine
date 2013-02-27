#include "Includes.h"
#ifndef __POINT_H__
#define __POINT_H__

namespace EricOgreEngine
{
	class DLL_EXPORT Point
	{
	public:
		Point(int xVal, int yVal);
		~Point(void);
		int x;
		int y;
	};
}
#endif
