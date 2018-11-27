/*

CodeLine - osirem.com
Copyright OSIREM LTD (C) 2016
www.osirem.com www.qage.org www.geopomp.com

This source is proprietary, and cannot be used, in part or in full without
the express permission of the original author. The original author retain the
rights to use, modify, and/or relicense this code without notice.

*/

#ifndef CODELINE_H
#define CODELINE_H

#include <vector>
#include <math.h>

#include "MuType.h"
#include "Variable.h"
#include "Code.h"
#include "Function.h"
#include "System.h"

namespace ecoin
{

	enum MuLine
	{
		MuLine_Init_Var,
		MuLine_Assign,
		MuLine_Assign_Opr,
		MuLine_Function,
		MuLine_Loop_Start,
		MuLine_Loop_End
	};

	class CodeLine
	{
	public:
		CodeLine() {};
		CodeLine(std::string f_Line, uint f_CHK, System* f_System);
		~CodeLine();

		int acSearch_CodeType(uint f_TYPE);

		bool ac_Execute(void);

		std::vector<Code*> m_vec_Code;
		std::vector<Variable*> m_vec_Variable;
		std::vector<Operator*> m_vec_Operator;
		std::vector<Function*> m_vec_Function;

		Variable* m_Result;

		System* m_System;

		uint m_CodeLine;
		uint m_ChkSRT;
		uint m_ChkEND;
		uint m_END;
		uint m_Chk;
	};

};

#endif