
#ifndef __PARM_RANGE_H__
#define __PARM_RANGE_H__

#include "PRM/PRM_Range.h"

namespace cchdk
{

	/****************************************************************************************************/
	
	struct Range
	{
		Range(float amin = 0.f, float amax = 1.f, PRM_RangeFlag aminflag=PRM_RANGE_UI, PRM_RangeFlag amaxflag=PRM_RANGE_UI) : range(aminflag, amin, amaxflag, amax) { }

		PRM_Range* get() { return &this->range; }

		PRM_Range range;
	};

	/****************************************************************************************************/

	struct NoRange
	{
		PRM_Range* get() { return 0; }
	};

	/****************************************************************************************************/
}

#endif

