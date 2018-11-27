/*

Variable - osirem.com
Copyright OSIREM LTD (C) 2016
www.osirem.com www.qage.org www.geopomp.com

This source is proprietary, and cannot be used, in part or in full without
the express permission of the original author. The original author retain the
rights to use, modify, and/or relicense this code without notice.

*/

#include "pch.h"

#include "Variable.h"
#include "Code.h"
#include "CodeLine.h"

using namespace ecoin;

namespace ecoin
{

Variable::Variable(std::string f_Stg, uint f_CodeType, uint f_PrecType) : MuType(MuTYPE_Variable), m_MxName(f_Stg), m_CodeType(f_CodeType), m_PrecType(f_PrecType)
	{
	m_Value_Float = 1.0f;
	m_Value_Float2X = 1.0f;
	m_Value_Float2Y = 1.0f;
	m_Value_Float3X = 1.0f;
	m_Value_Float3Y = 1.0f;
	m_Value_Float3Z = 1.0f;
	m_Value_Float4X = 1.0f;
	m_Value_Float4Y = 1.0f;
	m_Value_Float4Z = 1.0f;
	m_Value_Float4W = 1.0f;
	m_Value_uInt = 1;
	m_Value_Int = 1;
	m_Value_Int2X = 1;
	m_Value_Int2Y = 1;
	m_Value_Int3X = 1;
	m_Value_Int3Y = 1;
	m_Value_Int3Z = 1;
	m_Value_Int4X = 1;
	m_Value_Int4Y = 1;
	m_Value_Int4Z = 1;
	m_Value_Int4W = 1;
	m_Value_Bool = true;
	m_Value_Char = 'A';
	m_Value_String = "Alpha";
	m_Action = MuAct_Start;
	}

Variable::Variable(uint f_PrecType) : MuType(MuTYPE_Variable), m_MxName("IdentVar"), m_CodeType(MuCode_Var), m_PrecType(f_PrecType)
	{
	m_Value_Float = 1.0f;
	m_Value_Float2X = 1.0f;
	m_Value_Float2Y = 1.0f;
	m_Value_Float3X = 1.0f;
	m_Value_Float3Y = 1.0f;
	m_Value_Float3Z = 1.0f;
	m_Value_Float4X = 1.0f;
	m_Value_Float4Y = 1.0f;
	m_Value_Float4Z = 1.0f;
	m_Value_Float4W = 1.0f;
	m_Value_uInt = 1;
	m_Value_Int = 1;
	m_Value_Int2X = 1;
	m_Value_Int2Y = 1;
	m_Value_Int3X = 1;
	m_Value_Int3Y = 1;
	m_Value_Int3Z = 1;
	m_Value_Int4X = 1;
	m_Value_Int4Y = 1;
	m_Value_Int4Z = 1;
	m_Value_Int4W = 1;
	m_Value_Bool = true;
	m_Value_Char = 'A';
	m_Value_String = "Alpha";
	m_Action = MuAct_Start;
	};

int agFind_VarType(std::string f_String)
	{
	if(f_String.compare("float") == 0 ||
	   f_String.compare("Float") == 0 ||
	   f_String.compare("float1") == 0 ||
	   f_String.compare("Float1") == 0)
		{
		return MuVar_Float;
		}
	else if(f_String.compare("float2") == 0 ||
			f_String.compare("Float2") == 0)
		{
		return MuVar_Float2;
		}
	else if(f_String.compare("float3") == 0 ||
			f_String.compare("Float3") == 0)
		{
		return MuVar_Float3;
		}
	else if(f_String.compare("float4") == 0 ||
			f_String.compare("Float4") == 0)
		{
		return MuVar_Float4;
		}
	else if(f_String.compare("int") == 0 ||
	   f_String.compare("Int") == 0 ||
	   f_String.compare("int1") == 0 ||
	   f_String.compare("Int1") == 0)
		{
		return MuVar_Int;
		}
	else if(f_String.compare("int2") == 0 ||
			f_String.compare("Int2") == 0)
		{
		return MuVar_Int2;
		}
	else if(f_String.compare("int3") == 0 ||
			f_String.compare("Int3") == 0)
		{
		return MuVar_Int3;
		}
	else if(f_String.compare("int4") == 0 ||
			f_String.compare("Int4") == 0)
		{
		return MuVar_Int4;
		}
	else if(f_String.compare("uint") == 0 ||
			f_String.compare("uInt") == 0)
		{
		return MuVar_uInt;
		}
	else if(f_String.compare("bool") == 0 ||
			f_String.compare("Bool") == 0)
		{
		return MuVar_Bool;
		}
	else if(f_String.compare("char") == 0 ||
			f_String.compare("Char") == 0)
		{
		return MuVar_Char;
		}
	else if(f_String.compare("string") == 0 ||
			f_String.compare("String") == 0)
		{
		return MuVar_String;
		}

	return 0;
	}

uint Variable::get_Value(void)
	{
	/*if(m_CodeType == MuVar_Float)
		{

		}
	else if(m_Variable == MuVar_Int)
		{

		}
	else if (m_Variable == MuVar_Float2)
	{
	}
	else if(f_String.compare("float3") == 0 ||
			f_String.compare("Float3") == 0)
		{
		return MuVar_Float3;
		}
	else if(f_String.compare("float4") == 0 ||
			f_String.compare("Float4") == 0)
		{
		return MuVar_Float4;
		}
	else if(f_String.compare("int") == 0 ||
	   f_String.compare("Int") == 0 ||
	   f_String.compare("int1") == 0 ||
	   f_String.compare("Int1") == 0)
		{
		return ;
		}
	else if(f_String.compare("int2") == 0 ||
			f_String.compare("Int2") == 0)
		{
		return MuVar_Int2;
		}
	else if(f_String.compare("int3") == 0 ||
			f_String.compare("Int3") == 0)
		{
		return MuVar_Int3;
		}
	else if(f_String.compare("int4") == 0 ||
			f_String.compare("Int4") == 0)
		{
		return MuVar_Int4;
		}
	else if(f_String.compare("uint") == 0 ||
			f_String.compare("uInt") == 0)
		{
		return MuVar_uInt;
		}
	else if(f_String.compare("bool") == 0 ||
			f_String.compare("Bool") == 0)
		{
		return MuVar_Bool;
		}
	else if(f_String.compare("char") == 0 ||
			f_String.compare("Char") == 0)
		{
		return MuVar_Char;
		}
	else if(f_String.compare("string") == 0 ||
			f_String.compare("String") == 0)
		{
		return MuVar_String;
		}*/

	return 0;
	}

	Variable* av_Identity(uint f_PrecType)
		{
		return new Variable(f_PrecType);
		}

};