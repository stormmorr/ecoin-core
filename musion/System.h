/*

System - osirem.com
Copyright OSIREM LTD (C) 2016
www.osirem.com www.qage.org www.geopomp.com

This source is proprietary, and cannot be used, in part or in full without
the express permission of the original author. The original author retain the
rights to use, modify, and/or relicense this code without notice.

*/

#ifndef SYSTEM_H
#define SYSTEM_H

#include <vector>
#include <math.h>

#include "Variable.h"

namespace ecoin
{

	class System
	{
	public:
		System();
		~System();

		Variable* av_Var(std::string f_String);

		Variable* Pos;
		Variable* Posx;
		Variable* Posy;
		Variable* Posz;
		Variable* Color;
		Variable* Matrix1;
		Variable* Matrix2;
		Variable* Matrix3;
		Variable* Matrix4;

		Variable* nonce;

		std::vector<Variable*> m_Global;
	};

};

#endif