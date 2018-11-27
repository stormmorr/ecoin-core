/*

Variable - osirem.com
Copyright OSIREM LTD (C) 2016
www.osirem.com www.qage.org www.geopomp.com

This source is proprietary, and cannot be used, in part or in full without
the express permission of the original author. The original author retain the
rights to use, modify, and/or relicense this code without notice.

*/

#ifndef MUTYPE_H
#define MUTYPE_H

#include <vector>
#include <math.h>

#define uint unsigned int

#ifdef ecoin
using namespace ecoin;
#endif

namespace ecoin
{
	enum
	{
		MuTYPE_Function,
		MuTYPE_Variable,
		MuTYPE_System,
		MuTYPE_Code,
		MuTYPE_Operator,
		MuTYPE_Any,
		MuTYPE_Total
	};

	class MuType
		{
		public:
			MuType() : m_Type(MuTYPE_Any) {};
			MuType(uint f_Type) : m_Type(f_Type) {};
			uint m_Type;
		};

}


#endif