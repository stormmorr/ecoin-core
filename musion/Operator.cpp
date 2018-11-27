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

	Operator::Operator(std::string f_Stg, uint f_Variable) : m_MxName(f_Stg), MuType(MuTYPE_Operator)
		{

		}

	uint agFind_OprType(std::string f_String)
		{
		if (f_String.compare("plus") == 0 ||
			f_String.compare("Plus") == 0 ||
			f_String.compare("=+") == 0)
			{
			return MuOpr_Plus;
			}
		else if (f_String.compare("inc") == 0 ||
				 f_String.compare("increment") == 0 ||
				 f_String.compare("Inc") == 0 ||
				 f_String.compare("Increment") == 0 ||
			 	 f_String.compare("++") == 0)
			{
			return MuOpr_Inc;
			}
		else if (f_String.compare("minus") == 0 ||
				 f_String.compare("Minus") == 0 ||
				 f_String.compare("=-") == 0)
			{
			return MuOpr_Minus;
			}
		else if (f_String.compare("dec") == 0 ||
				 f_String.compare("decrement") == 0 ||
				 f_String.compare("Dec") == 0 ||
				 f_String.compare("Decrement") == 0 ||
			 	 f_String.compare("--") == 0)
			{
			return MuOpr_Dec;
			}
		else if (f_String.compare("mul") == 0 ||
				 f_String.compare("mult") == 0 ||
				 f_String.compare("multiply") == 0 ||
				 f_String.compare("Mul") == 0 ||
				 f_String.compare("MUL") == 0 ||
				 f_String.compare("Mull") == 0 ||
				 f_String.compare("MULL") == 0 ||
				 f_String.compare("Mult") == 0 ||
				 f_String.compare("Multiply") == 0 ||
				 f_String.compare("=*") == 0 ||
				 f_String.compare("**") == 0 ||
				 f_String.compare("Mul**") == 0)
			{
			return MuOpr_Multiply;
			}
		else if (f_String.compare("div") == 0 ||
				 f_String.compare("divide") == 0 ||
				 f_String.compare("Div") == 0 ||
				 f_String.compare("Divide") == 0 ||
				 f_String.compare("DIV") == 0 ||
				 f_String.compare("=/") == 0)
			{
			return MuOpr_Divide;
			}
		else if (f_String.compare("bitleft") == 0 ||
				 f_String.compare("bitl") == 0 ||
				 f_String.compare("bit") == 0 ||
				 f_String.compare("<<") == 0)
			{
			return MuOpr_Bitleft;
			}
		else if (f_String.compare("bitright") == 0 ||
				 f_String.compare("bitr") == 0 ||
				 f_String.compare(">>") == 0)
			{
			return MuOpr_Bitright;
			}
		else if (f_String.compare("mod") == 0 ||
				 f_String.compare("MOD") == 0 ||
				 f_String.compare("Mod") == 0 ||
				 f_String.compare("modulus") == 0 || 
				 f_String.compare("Modulus") == 0 ||
				 f_String.compare("=%") == 0)
			{
			return MuOpr_Modulus;
			}

		return 0;
		}

	void Operator::ac_Execute(void)
		{
		switch(m_Operator)
			{
			case MuOpr_Plus:
				{
				resultHand->m_Value_Float = leftHand->m_Value_Float + rightHand->m_Value_Float;
				resultHand->m_Value_Float2X = leftHand->m_Value_Float2X + rightHand->m_Value_Float2X;
				resultHand->m_Value_Float2Y = leftHand->m_Value_Float2Y + rightHand->m_Value_Float2Y;
				resultHand->m_Value_Float3X = leftHand->m_Value_Float3X + rightHand->m_Value_Float3X;
				resultHand->m_Value_Float3Y = leftHand->m_Value_Float3Y + rightHand->m_Value_Float3Y;
				resultHand->m_Value_Float3Z = leftHand->m_Value_Float3Z + rightHand->m_Value_Float3Z;
				resultHand->m_Value_Float4X = leftHand->m_Value_Float4X + rightHand->m_Value_Float4X;
				resultHand->m_Value_Float4Y = leftHand->m_Value_Float4Y + rightHand->m_Value_Float4Y;
				resultHand->m_Value_Float4Z = leftHand->m_Value_Float4Z + rightHand->m_Value_Float4Z;
				resultHand->m_Value_Float4W = leftHand->m_Value_Float4W + rightHand->m_Value_Float4W;
				resultHand->m_Value_uInt = leftHand->m_Value_uInt + rightHand->m_Value_uInt;
				resultHand->m_Value_Int = leftHand->m_Value_Int + rightHand->m_Value_Int;
				resultHand->m_Value_Int2X = leftHand->m_Value_Int2X + rightHand->m_Value_Int2X;
				resultHand->m_Value_Int2Y = leftHand->m_Value_Int2Y + rightHand->m_Value_Int2Y;
				resultHand->m_Value_Int3X = leftHand->m_Value_Int3X + rightHand->m_Value_Int3X;
				resultHand->m_Value_Int3Y = leftHand->m_Value_Int3Y + rightHand->m_Value_Int3Y;
				resultHand->m_Value_Int3Z = leftHand->m_Value_Int3Z + rightHand->m_Value_Int3Z;
				resultHand->m_Value_Int4X = leftHand->m_Value_Int4X + rightHand->m_Value_Int4X;
				resultHand->m_Value_Int4Y = leftHand->m_Value_Int4Y + rightHand->m_Value_Int4Y;
				resultHand->m_Value_Int4Z = leftHand->m_Value_Int4Z + rightHand->m_Value_Int4Z;
				resultHand->m_Value_Int4W = leftHand->m_Value_Int4W + rightHand->m_Value_Int4W;
				resultHand->m_Value_Bool = leftHand->m_Value_Bool + rightHand->m_Value_Bool;
				resultHand->m_Value_Char = leftHand->m_Value_Char + rightHand->m_Value_Char;
				resultHand->m_Value_String = leftHand->m_Value_String + rightHand->m_Value_String;
				}break;
			case MuOpr_Inc:
				{
				resultHand->m_Value_Float = leftHand->m_Value_Float + rightHand->m_Value_Float;
				resultHand->m_Value_Float2X = leftHand->m_Value_Float2X + rightHand->m_Value_Float2X;
				resultHand->m_Value_Float2Y = leftHand->m_Value_Float2Y + rightHand->m_Value_Float2Y;
				resultHand->m_Value_Float3X = leftHand->m_Value_Float3X + rightHand->m_Value_Float3X;
				resultHand->m_Value_Float3Y = leftHand->m_Value_Float3Y + rightHand->m_Value_Float3Y;
				resultHand->m_Value_Float3Z = leftHand->m_Value_Float3Z + rightHand->m_Value_Float3Z;
				resultHand->m_Value_Float4X = leftHand->m_Value_Float4X + rightHand->m_Value_Float4X;
				resultHand->m_Value_Float4Y = leftHand->m_Value_Float4Y + rightHand->m_Value_Float4Y;
				resultHand->m_Value_Float4Z = leftHand->m_Value_Float4Z + rightHand->m_Value_Float4Z;
				resultHand->m_Value_Float4W = leftHand->m_Value_Float4W + rightHand->m_Value_Float4W;
				resultHand->m_Value_uInt = leftHand->m_Value_uInt + rightHand->m_Value_uInt;
				resultHand->m_Value_Int = leftHand->m_Value_Int + rightHand->m_Value_Int;
				resultHand->m_Value_Int2X = leftHand->m_Value_Int2X + rightHand->m_Value_Int2X;
				resultHand->m_Value_Int2Y = leftHand->m_Value_Int2Y + rightHand->m_Value_Int2Y;
				resultHand->m_Value_Int3X = leftHand->m_Value_Int3X + rightHand->m_Value_Int3X;
				resultHand->m_Value_Int3Y = leftHand->m_Value_Int3Y + rightHand->m_Value_Int3Y;
				resultHand->m_Value_Int3Z = leftHand->m_Value_Int3Z + rightHand->m_Value_Int3Z;
				resultHand->m_Value_Int4X = leftHand->m_Value_Int4X + rightHand->m_Value_Int4X;
				resultHand->m_Value_Int4Y = leftHand->m_Value_Int4Y + rightHand->m_Value_Int4Y;
				resultHand->m_Value_Int4Z = leftHand->m_Value_Int4Z + rightHand->m_Value_Int4Z;
				resultHand->m_Value_Int4W = leftHand->m_Value_Int4W + rightHand->m_Value_Int4W;
				resultHand->m_Value_Bool = leftHand->m_Value_Bool + rightHand->m_Value_Bool;
				resultHand->m_Value_Char = leftHand->m_Value_Char + rightHand->m_Value_Char;
				resultHand->m_Value_String = leftHand->m_Value_String + rightHand->m_Value_String;
				}break;
			case MuOpr_Minus:
				{
				resultHand->m_Value_Float = leftHand->m_Value_Float - rightHand->m_Value_Float;
				resultHand->m_Value_Float2X = leftHand->m_Value_Float2X - rightHand->m_Value_Float2X;
				resultHand->m_Value_Float2Y = leftHand->m_Value_Float2Y - rightHand->m_Value_Float2Y;
				resultHand->m_Value_Float3X = leftHand->m_Value_Float3X - rightHand->m_Value_Float3X;
				resultHand->m_Value_Float3Y = leftHand->m_Value_Float3Y - rightHand->m_Value_Float3Y;
				resultHand->m_Value_Float3Z = leftHand->m_Value_Float3Z - rightHand->m_Value_Float3Z;
				resultHand->m_Value_Float4X = leftHand->m_Value_Float4X - rightHand->m_Value_Float4X;
				resultHand->m_Value_Float4Y = leftHand->m_Value_Float4Y - rightHand->m_Value_Float4Y;
				resultHand->m_Value_Float4Z = leftHand->m_Value_Float4Z - rightHand->m_Value_Float4Z;
				resultHand->m_Value_Float4W = leftHand->m_Value_Float4W - rightHand->m_Value_Float4W;
				resultHand->m_Value_uInt = leftHand->m_Value_uInt - rightHand->m_Value_uInt;
				resultHand->m_Value_Int = leftHand->m_Value_Int - rightHand->m_Value_Int;
				resultHand->m_Value_Int2X = leftHand->m_Value_Int2X - rightHand->m_Value_Int2X;
				resultHand->m_Value_Int2Y = leftHand->m_Value_Int2Y - rightHand->m_Value_Int2Y;
				resultHand->m_Value_Int3X = leftHand->m_Value_Int3X - rightHand->m_Value_Int3X;
				resultHand->m_Value_Int3Y = leftHand->m_Value_Int3Y - rightHand->m_Value_Int3Y;
				resultHand->m_Value_Int3Z = leftHand->m_Value_Int3Z - rightHand->m_Value_Int3Z;
				resultHand->m_Value_Int4X = leftHand->m_Value_Int4X - rightHand->m_Value_Int4X;
				resultHand->m_Value_Int4Y = leftHand->m_Value_Int4Y - rightHand->m_Value_Int4Y;
				resultHand->m_Value_Int4Z = leftHand->m_Value_Int4Z - rightHand->m_Value_Int4Z;
				resultHand->m_Value_Int4W = leftHand->m_Value_Int4W - rightHand->m_Value_Int4W;

#if 0
				resultHand->m_Value_Bool = leftHand->m_Value_Bool - rightHand->m_Value_Bool;
				resultHand->m_Value_Char = leftHand->m_Value_Char - rightHand->m_Value_Char;
				resultHand->m_Value_String = leftHand->m_Value_String - rightHand->m_Value_String;
#endif
				}break;
			case MuOpr_Dec:
				{
				resultHand->m_Value_Float = leftHand->m_Value_Float - rightHand->m_Value_Float;
				resultHand->m_Value_Float2X = leftHand->m_Value_Float2X - rightHand->m_Value_Float2X;
				resultHand->m_Value_Float2Y = leftHand->m_Value_Float2Y - rightHand->m_Value_Float2Y;
				resultHand->m_Value_Float3X = leftHand->m_Value_Float3X - rightHand->m_Value_Float3X;
				resultHand->m_Value_Float3Y = leftHand->m_Value_Float3Y - rightHand->m_Value_Float3Y;
				resultHand->m_Value_Float3Z = leftHand->m_Value_Float3Z - rightHand->m_Value_Float3Z;
				resultHand->m_Value_Float4X = leftHand->m_Value_Float4X - rightHand->m_Value_Float4X;
				resultHand->m_Value_Float4Y = leftHand->m_Value_Float4Y - rightHand->m_Value_Float4Y;
				resultHand->m_Value_Float4Z = leftHand->m_Value_Float4Z - rightHand->m_Value_Float4Z;
				resultHand->m_Value_Float4W = leftHand->m_Value_Float4W - rightHand->m_Value_Float4W;
				resultHand->m_Value_uInt = leftHand->m_Value_uInt - rightHand->m_Value_uInt;
				resultHand->m_Value_Int = leftHand->m_Value_Int - rightHand->m_Value_Int;
				resultHand->m_Value_Int2X = leftHand->m_Value_Int2X - rightHand->m_Value_Int2X;
				resultHand->m_Value_Int2Y = leftHand->m_Value_Int2Y - rightHand->m_Value_Int2Y;
				resultHand->m_Value_Int3X = leftHand->m_Value_Int3X - rightHand->m_Value_Int3X;
				resultHand->m_Value_Int3Y = leftHand->m_Value_Int3Y - rightHand->m_Value_Int3Y;
				resultHand->m_Value_Int3Z = leftHand->m_Value_Int3Z - rightHand->m_Value_Int3Z;
				resultHand->m_Value_Int4X = leftHand->m_Value_Int4X - rightHand->m_Value_Int4X;
				resultHand->m_Value_Int4Y = leftHand->m_Value_Int4Y - rightHand->m_Value_Int4Y;
				resultHand->m_Value_Int4Z = leftHand->m_Value_Int4Z - rightHand->m_Value_Int4Z;
				resultHand->m_Value_Int4W = leftHand->m_Value_Int4W - rightHand->m_Value_Int4W;

#if 0
				resultHand->m_Value_Bool = leftHand->m_Value_Bool - rightHand->m_Value_Bool;
				resultHand->m_Value_Char = leftHand->m_Value_Char - rightHand->m_Value_Char;
				resultHand->m_Value_String = leftHand->m_Value_String - rightHand->m_Value_String;
#endif
				}break;
			case MuOpr_Multiply:
				{
				resultHand->m_Value_Float = leftHand->m_Value_Float * rightHand->m_Value_Float;
				resultHand->m_Value_Float2X = leftHand->m_Value_Float2X * rightHand->m_Value_Float2X;
				resultHand->m_Value_Float2Y = leftHand->m_Value_Float2Y * rightHand->m_Value_Float2Y;
				resultHand->m_Value_Float3X = leftHand->m_Value_Float3X * rightHand->m_Value_Float3X;
				resultHand->m_Value_Float3Y = leftHand->m_Value_Float3Y * rightHand->m_Value_Float3Y;
				resultHand->m_Value_Float3Z = leftHand->m_Value_Float3Z * rightHand->m_Value_Float3Z;
				resultHand->m_Value_Float4X = leftHand->m_Value_Float4X * rightHand->m_Value_Float4X;
				resultHand->m_Value_Float4Y = leftHand->m_Value_Float4Y * rightHand->m_Value_Float4Y;
				resultHand->m_Value_Float4Z = leftHand->m_Value_Float4Z * rightHand->m_Value_Float4Z;
				resultHand->m_Value_Float4W = leftHand->m_Value_Float4W * rightHand->m_Value_Float4W;
				resultHand->m_Value_uInt = leftHand->m_Value_uInt * rightHand->m_Value_uInt;
				resultHand->m_Value_Int = leftHand->m_Value_Int * rightHand->m_Value_Int;
				resultHand->m_Value_Int2X = leftHand->m_Value_Int2X * rightHand->m_Value_Int2X;
				resultHand->m_Value_Int2Y = leftHand->m_Value_Int2Y * rightHand->m_Value_Int2Y;
				resultHand->m_Value_Int3X = leftHand->m_Value_Int3X * rightHand->m_Value_Int3X;
				resultHand->m_Value_Int3Y = leftHand->m_Value_Int3Y * rightHand->m_Value_Int3Y;
				resultHand->m_Value_Int3Z = leftHand->m_Value_Int3Z * rightHand->m_Value_Int3Z;
				resultHand->m_Value_Int4X = leftHand->m_Value_Int4X * rightHand->m_Value_Int4X;
				resultHand->m_Value_Int4Y = leftHand->m_Value_Int4Y * rightHand->m_Value_Int4Y;
				resultHand->m_Value_Int4Z = leftHand->m_Value_Int4Z * rightHand->m_Value_Int4Z;
				resultHand->m_Value_Int4W = leftHand->m_Value_Int4W * rightHand->m_Value_Int4W;

#if 0
				resultHand->m_Value_Bool = leftHand->m_Value_Bool * rightHand->m_Value_Bool;
				resultHand->m_Value_Char = leftHand->m_Value_Char * rightHand->m_Value_Char;
				resultHand->m_Value_String = leftHand->m_Value_String * rightHand->m_Value_String;
#endif
				}break;
			case MuOpr_Divide:
				{
				resultHand->m_Value_Float = leftHand->m_Value_Float / rightHand->m_Value_Float;
				resultHand->m_Value_Float2X = leftHand->m_Value_Float2X / rightHand->m_Value_Float2X;
				resultHand->m_Value_Float2Y = leftHand->m_Value_Float2Y / rightHand->m_Value_Float2Y;
				resultHand->m_Value_Float3X = leftHand->m_Value_Float3X / rightHand->m_Value_Float3X;
				resultHand->m_Value_Float3Y = leftHand->m_Value_Float3Y / rightHand->m_Value_Float3Y;
				resultHand->m_Value_Float3Z = leftHand->m_Value_Float3Z / rightHand->m_Value_Float3Z;
				resultHand->m_Value_Float4X = leftHand->m_Value_Float4X / rightHand->m_Value_Float4X;
				resultHand->m_Value_Float4Y = leftHand->m_Value_Float4Y / rightHand->m_Value_Float4Y;
				resultHand->m_Value_Float4Z = leftHand->m_Value_Float4Z / rightHand->m_Value_Float4Z;
				resultHand->m_Value_Float4W = leftHand->m_Value_Float4W / rightHand->m_Value_Float4W;
				resultHand->m_Value_uInt = leftHand->m_Value_uInt / rightHand->m_Value_uInt;
				resultHand->m_Value_Int = leftHand->m_Value_Int / rightHand->m_Value_Int;
				resultHand->m_Value_Int2X = leftHand->m_Value_Int2X / rightHand->m_Value_Int2X;
				resultHand->m_Value_Int2Y = leftHand->m_Value_Int2Y / rightHand->m_Value_Int2Y;
				resultHand->m_Value_Int3X = leftHand->m_Value_Int3X / rightHand->m_Value_Int3X;
				resultHand->m_Value_Int3Y = leftHand->m_Value_Int3Y / rightHand->m_Value_Int3Y;
				resultHand->m_Value_Int3Z = leftHand->m_Value_Int3Z / rightHand->m_Value_Int3Z;
				resultHand->m_Value_Int4X = leftHand->m_Value_Int4X / rightHand->m_Value_Int4X;
				resultHand->m_Value_Int4Y = leftHand->m_Value_Int4Y / rightHand->m_Value_Int4Y;
				resultHand->m_Value_Int4Z = leftHand->m_Value_Int4Z / rightHand->m_Value_Int4Z;
				resultHand->m_Value_Int4W = leftHand->m_Value_Int4W / rightHand->m_Value_Int4W;

#if 0
				resultHand->m_Value_Bool = leftHand->m_Value_Bool / rightHand->m_Value_Bool;
				resultHand->m_Value_Char = leftHand->m_Value_Char / rightHand->m_Value_Char;
				resultHand->m_Value_String = leftHand->m_Value_String / rightHand->m_Value_String;
#endif
				}break;
			case MuOpr_Bitleft:
				{
				resultHand->m_Value_uInt = leftHand->m_Value_uInt << rightHand->m_Value_uInt;
				resultHand->m_Value_Int = leftHand->m_Value_Int << rightHand->m_Value_Int;
				resultHand->m_Value_Int2X = leftHand->m_Value_Int2X << rightHand->m_Value_Int2X;
				resultHand->m_Value_Int2Y = leftHand->m_Value_Int2Y << rightHand->m_Value_Int2Y;
				resultHand->m_Value_Int3X = leftHand->m_Value_Int3X << rightHand->m_Value_Int3X;
				resultHand->m_Value_Int3Y = leftHand->m_Value_Int3Y << rightHand->m_Value_Int3Y;
				resultHand->m_Value_Int3Z = leftHand->m_Value_Int3Z << rightHand->m_Value_Int3Z;
				resultHand->m_Value_Int4X = leftHand->m_Value_Int4X << rightHand->m_Value_Int4X;
				resultHand->m_Value_Int4Y = leftHand->m_Value_Int4Y << rightHand->m_Value_Int4Y;
				resultHand->m_Value_Int4Z = leftHand->m_Value_Int4Z << rightHand->m_Value_Int4Z;
				resultHand->m_Value_Int4W = leftHand->m_Value_Int4W << rightHand->m_Value_Int4W;
				resultHand->m_Value_Char = leftHand->m_Value_Char << rightHand->m_Value_Char;

#if 0
				resultHand->m_Value_Bool = leftHand->m_Value_Bool << rightHand->m_Value_Bool;
				resultHand->m_Value_String = leftHand->m_Value_String << rightHand->m_Value_String;
#endif
				}break;
			case MuOpr_Bitright:
				{
				resultHand->m_Value_uInt = leftHand->m_Value_Int >> rightHand->m_Value_uInt;
				resultHand->m_Value_Int = leftHand->m_Value_Int >> rightHand->m_Value_Int;
				resultHand->m_Value_Int2X = leftHand->m_Value_Int2X >> rightHand->m_Value_Int2X;
				resultHand->m_Value_Int2Y = leftHand->m_Value_Int2Y >> rightHand->m_Value_Int2Y;
				resultHand->m_Value_Int3X = leftHand->m_Value_Int3X >> rightHand->m_Value_Int3X;
				resultHand->m_Value_Int3Y = leftHand->m_Value_Int3Y >> rightHand->m_Value_Int3Y;
				resultHand->m_Value_Int3Z = leftHand->m_Value_Int3Z >> rightHand->m_Value_Int3Z;
				resultHand->m_Value_Int4X = leftHand->m_Value_Int4X >> rightHand->m_Value_Int4X;
				resultHand->m_Value_Int4Y = leftHand->m_Value_Int4Y >> rightHand->m_Value_Int4Y;
				resultHand->m_Value_Int4Z = leftHand->m_Value_Int4Z >> rightHand->m_Value_Int4Z;
				resultHand->m_Value_Int4W = leftHand->m_Value_Int4W >> rightHand->m_Value_Int4W;
				resultHand->m_Value_Char = leftHand->m_Value_Char >> rightHand->m_Value_Char;

#if 0
				resultHand->m_Value_Bool = leftHand->m_Value_Bool >> rightHand->m_Value_Bool;
				resultHand->m_Value_String = leftHand->m_Value_String >> rightHand->m_Value_String;
#endif
				}break;
			case MuOpr_Modulus:
				{
				resultHand->m_Value_uInt = leftHand->m_Value_uInt % rightHand->m_Value_uInt;
				resultHand->m_Value_Int = leftHand->m_Value_Int % rightHand->m_Value_Int;
				resultHand->m_Value_Int2X = leftHand->m_Value_Int2X % rightHand->m_Value_Int2X;
				resultHand->m_Value_Int2Y = leftHand->m_Value_Int2Y % rightHand->m_Value_Int2Y;
				resultHand->m_Value_Int3X = leftHand->m_Value_Int3X % rightHand->m_Value_Int3X;
				resultHand->m_Value_Int3Y = leftHand->m_Value_Int3Y % rightHand->m_Value_Int3Y;
				resultHand->m_Value_Int3Z = leftHand->m_Value_Int3Z % rightHand->m_Value_Int3Z;
				resultHand->m_Value_Int4X = leftHand->m_Value_Int4X % rightHand->m_Value_Int4X;
				resultHand->m_Value_Int4Y = leftHand->m_Value_Int4Y % rightHand->m_Value_Int4Y;
				resultHand->m_Value_Int4Z = leftHand->m_Value_Int4Z % rightHand->m_Value_Int4Z;
				resultHand->m_Value_Int4W = leftHand->m_Value_Int4W % rightHand->m_Value_Int4W;
				resultHand->m_Value_Char = leftHand->m_Value_Char % rightHand->m_Value_Char;

#if 0
				resultHand->m_Value_Bool = leftHand->m_Value_Bool % rightHand->m_Value_Bool;
				resultHand->m_Value_String = leftHand->m_Value_String % rightHand->m_Value_String;
#endif
				}break;
			}
		}

};