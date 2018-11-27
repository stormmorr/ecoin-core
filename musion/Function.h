/*

Function - osirem.com
Copyright OSIREM LTD (C) 2016
www.osirem.com www.qage.org www.geopomp.com

This source is proprietary, and cannot be used, in part or in full without
the express permission of the original author. The original author retain the
rights to use, modify, and/or relicense this code without notice.

*/

#ifndef FUNCTION_H
#define FUNCTION_H

#include <vector>
#include <math.h>

#include "MuType.h"
#include "Variable.h"
#include "Code.h"
#include "Operator.h"

namespace ecoin
{

	enum
	{
		MuFunc_RandFloat,
		MuFunc_RandInt,
		MuFunc_Sin,
		MuFunc_Cos,
		MuFunc_Tan,
		MuFunc_Log,
		MuFunc_InvSin,
		MuFunc_InvCos,
		MuFunc_InvTan,
		MuFunc_Sqr,
		MuFunc_Sqrt,
		MuFunc_ArcTan2,
		MuFunc_Gate_NOT,
		MuFunc_Gate_AND,
		MuFunc_Gate_OR,
		MuFunc_Gate_XOR,
		MuFunc_Gate_NAND,
		MuFunc_Gate_NOR,
		MuFunc_Gate_XNOR,
		MuSetGLOBAL_,
	};

	class Function : MuType
		{
		public:
			Function() : MuType(MuTYPE_Function) {};
			Function(uint f_Function) : MuType(MuTYPE_Function),
				m_Function(f_Function) { m_Function = f_Function; };

			uint acPrep_addVariable(Variable* f_Var);

			Variable* acExecute(void);

			uint m_Function;

			std::vector<Code*> m_vec_Code;
			std::vector<Variable*> m_vec_Variable;
			std::vector<Operator*> m_vec_Operator;
		};

	extern int agFind_FunctionType(std::string f_String);

};


#endif