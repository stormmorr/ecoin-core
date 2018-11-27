/*

Variable - osirem.com
Copyright OSIREM LTD (C) 2016
www.osirem.com www.qage.org www.geopomp.com

This source is proprietary, and cannot be used, in part or in full without
the express permission of the original author. The original author retain the
rights to use, modify, and/or relicense this code without notice.

*/

#ifndef OPERATOR_H
#define OPERATOR_H

#include <vector>
#include <math.h>

#include "MuType.h"
#include "Variable.h"

namespace ecoin
{
	enum MuOperatorType
	{
		MuOpr_Plus,
		MuOpr_Inc,
		MuOpr_Minus,
		MuOpr_Dec,
		MuOpr_Multiply,
		MuOpr_Divide,
		MuOpr_Bitleft,
		MuOpr_Bitright,
		MuOpr_Modulus,
		MuOpr_Total
	};

	class Operator : MuType
	{
	public:
		Operator() {};
		Operator(std::string f_Stg, uint f_Operator);
		~Operator() {};

		void ac_Execute(void);

		Variable* leftHand;
		Variable* rightHand;
		Variable* resultHand;

		uint m_Operator;

		std::string m_MxName;
	};

	extern uint agFind_OprType(std::string f_String);

};

#endif
