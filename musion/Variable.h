/*

Variable - osirem.com
Copyright OSIREM LTD (C) 2016
www.osirem.com www.qage.org www.geopomp.com

This source is proprietary, and cannot be used, in part or in full without
the express permission of the original author. The original author retain the
rights to use, modify, and/or relicense this code without notice.

*/

#ifndef VARIABLE_H
#define VARIABLE_H

#include <vector>
#include <math.h>

#include "MuType.h"

namespace ecoin
{
	enum Variable_Precision
		{
		MuVar_Start,
		MuVar_Float,
		MuVar_Float2,
		MuVar_Float3,
		MuVar_Float4,
		MuVar_uInt,
		MuVar_Int,
		MuVar_Int2,
		MuVar_Int3,
		MuVar_Int4,
		MuVar_Bool,
		MuVar_Char,
		MuVar_String,
		MuVar_Full,
		MuVar_Total
		};

	enum Variable_Action
		{
		MuAct_Start,
		MuAct_Random,
		MuAct_RandInt,
		MuAct_Reset,
		MuAct_Inc,
		MuAct_Dec,
		MuAct_Double,
		MuAct_Total
		};

	class Variable : MuType
		{
		public:
			Variable() { m_Action = MuAct_Start; };
			Variable(std::string f_Stg, uint f_CodeType, uint f_PrecType);
			~Variable() {};

			uint get_Value(void);

			Variable(uint f_PrecType);

			Variable(uint f_CodeType, uint f_PrecType) : MuType(MuTYPE_Variable), m_CodeType(f_CodeType), m_PrecType(f_PrecType)
				{
				m_Value_Float = 0.0f;
				m_Value_Float2X = 0.0f;
				m_Value_Float2Y = 0.0f;
				m_Value_Float3X = 0.0f;
				m_Value_Float3Y = 0.0f;
				m_Value_Float3Z = 0.0f;
				m_Value_Float4X = 0.0f;
				m_Value_Float4Y = 0.0f;
				m_Value_Float4Z = 0.0f;
				m_Value_Float4W = 0.0f;
				m_Value_uInt = 0;
				m_Value_Int = 0;
				m_Value_Int2X = 0;
				m_Value_Int2Y = 0;
				m_Value_Int3X = 0;
				m_Value_Int3Y = 0;
				m_Value_Int3Z = 0;
				m_Value_Int4X = 0;
				m_Value_Int4Y = 0;
				m_Value_Int4Z = 0;
				m_Value_Int4W = 0;
				m_Value_Bool = false;
				m_Value_Char = '\n';
				m_Value_String.clear();
				};

			void operator=(const Variable &v)
				{
				m_PrecType = v.m_PrecType;
				m_CodeType = v.m_CodeType;
				m_Value_Float = v.m_Value_Float;
				m_Value_Float2X = v.m_Value_Float2X;
				m_Value_Float2Y = v.m_Value_Float2Y;
				m_Value_Float3X = v.m_Value_Float3X;
				m_Value_Float3Y = v.m_Value_Float3Y;
				m_Value_Float3Z = v.m_Value_Float3Z;
				m_Value_Float4X = v.m_Value_Float4X;
				m_Value_Float4Y = v.m_Value_Float4Y;
				m_Value_Float4Z = v.m_Value_Float4Z;
				m_Value_Float4W = v.m_Value_Float4W;
				m_Value_uInt = v.m_Value_uInt;
				m_Value_Int = v.m_Value_Int;
				m_Value_Int2X = v.m_Value_Int2X;
				m_Value_Int2Y = v.m_Value_Int2Y;
				m_Value_Int3X = v.m_Value_Int3X;
				m_Value_Int3Y = v.m_Value_Int3Y;
				m_Value_Int3Z = v.m_Value_Int3Z;
				m_Value_Int4X = v.m_Value_Int4X;
				m_Value_Int4Y = v.m_Value_Int4Y;
				m_Value_Int4Z = v.m_Value_Int4Z;
				m_Value_Int4W = v.m_Value_Int4W;
				m_Value_Bool = v.m_Value_Bool;
				m_Value_Char = v.m_Value_Char;
				m_Value_String = v.m_Value_String;
				}

			void operator=(const int &v)
				{
				m_Value_Float = (float)v;
				m_Value_Float2X = (float)v;
				m_Value_Float2Y = (float)v;
				m_Value_Float3X = (float)v;
				m_Value_Float3Y = (float)v;
				m_Value_Float3Z = (float)v;
				m_Value_Float4X = (float)v;
				m_Value_Float4Y = (float)v;
				m_Value_Float4Z = (float)v;
				m_Value_Float4W = (float)v;
				m_Value_uInt = (uint)v;
				m_Value_Int = v;
				m_Value_Int2X = v;
				m_Value_Int2Y = v;
				m_Value_Int3X = v;
				m_Value_Int3Y = v;
				m_Value_Int3Z = v;
				m_Value_Int4X = v;
				m_Value_Int4Y = v;
				m_Value_Int4Z = v;
				m_Value_Int4W = v;
				m_Value_Bool = v;
				m_Value_Char = v;

#if 0
				m_Value_String = v.m_Value_String;
#endif
				}

			void operator=(const float &v)
				{
				m_Value_Float = v;
				m_Value_Float2X = v;
				m_Value_Float2Y = v;
				m_Value_Float3X = v;
				m_Value_Float3Y = v;
				m_Value_Float3Z = v;
				m_Value_Float4X = v;
				m_Value_Float4Y = v;
				m_Value_Float4Z = v;
				m_Value_Float4W = v;
				m_Value_uInt = (uint)v;
				m_Value_Int = (int)v;
				m_Value_Int2X = (int)v;
				m_Value_Int2Y = (int)v;
				m_Value_Int3X = (int)v;
				m_Value_Int3Y = (int)v;
				m_Value_Int3Z = (int)v;
				m_Value_Int4X = (int)v;
				m_Value_Int4Y = (int)v;
				m_Value_Int4Z = (int)v;
				m_Value_Int4W = (int)v;
				m_Value_Bool = (bool)v;
				m_Value_Char = (char)v;

#if 0
				m_Value_String = v.m_Value_String;
#endif
				}

			void operator+=(const Variable &v)
				{
				m_Value_Float += v.m_Value_Float;
				m_Value_Float2X += v.m_Value_Float2X;
				m_Value_Float2Y += v.m_Value_Float2Y;
				m_Value_Float3X += v.m_Value_Float3X;
				m_Value_Float3Y += v.m_Value_Float3Y;
				m_Value_Float3Z += v.m_Value_Float3Z;
				m_Value_Float4X += v.m_Value_Float4X;
				m_Value_Float4Y += v.m_Value_Float4Y;
				m_Value_Float4Z += v.m_Value_Float4Z;
				m_Value_Float4W += v.m_Value_Float4W;
				m_Value_uInt += v.m_Value_uInt;
				m_Value_Int += v.m_Value_Int;
				m_Value_Int2X += v.m_Value_Int2X;
				m_Value_Int2Y += v.m_Value_Int2Y;
				m_Value_Int3X += v.m_Value_Int3X;
				m_Value_Int3Y += v.m_Value_Int3Y;
				m_Value_Int3Z += v.m_Value_Int3Z;
				m_Value_Int4X += v.m_Value_Int4X;
				m_Value_Int4Y += v.m_Value_Int4Y;
				m_Value_Int4Z += v.m_Value_Int4Z;
				m_Value_Int4W += v.m_Value_Int4W;
				m_Value_Bool += v.m_Value_Bool;
				m_Value_Char += v.m_Value_Char;
				m_Value_String += v.m_Value_String;
				}

			void operator-=(const Variable &v)
				{
				m_Value_Float -= v.m_Value_Float;
				m_Value_Float2X -= v.m_Value_Float2X;
				m_Value_Float2Y -= v.m_Value_Float2Y;
				m_Value_Float3X -= v.m_Value_Float3X;
				m_Value_Float3Y -= v.m_Value_Float3Y;
				m_Value_Float3Z -= v.m_Value_Float3Z;
				m_Value_Float4X -= v.m_Value_Float4X;
				m_Value_Float4Y -= v.m_Value_Float4Y;
				m_Value_Float4Z -= v.m_Value_Float4Z;
				m_Value_Float4W -= v.m_Value_Float4W;
				m_Value_uInt -= v.m_Value_uInt;
				m_Value_Int -= v.m_Value_Int;
				m_Value_Int2X -= v.m_Value_Int2X;
				m_Value_Int2Y -= v.m_Value_Int2Y;
				m_Value_Int3X -= v.m_Value_Int3X;
				m_Value_Int3Y -= v.m_Value_Int3Y;
				m_Value_Int3Z -= v.m_Value_Int3Z;
				m_Value_Int4X -= v.m_Value_Int4X;
				m_Value_Int4Y -= v.m_Value_Int4Y;
				m_Value_Int4Z -= v.m_Value_Int4Z;
				m_Value_Int4W -= v.m_Value_Int4W;
				m_Value_Bool -= v.m_Value_Bool;
				m_Value_Char -= v.m_Value_Char;
				}

			void operator+(const Variable &v)
				{
				m_Value_Float += v.m_Value_Float;
				m_Value_Float2X += v.m_Value_Float2X;
				m_Value_Float2Y += v.m_Value_Float2Y;
				m_Value_Float3X += v.m_Value_Float3X;
				m_Value_Float3Y += v.m_Value_Float3Y;
				m_Value_Float3Z += v.m_Value_Float3Z;
				m_Value_Float4X += v.m_Value_Float4X;
				m_Value_Float4Y += v.m_Value_Float4Y;
				m_Value_Float4Z += v.m_Value_Float4Z;
				m_Value_Float4W += v.m_Value_Float4W;
				m_Value_uInt += v.m_Value_uInt;
				m_Value_Int += v.m_Value_Int;
				m_Value_Int2X += v.m_Value_Int2X;
				m_Value_Int2Y += v.m_Value_Int2Y;
				m_Value_Int3X += v.m_Value_Int3X;
				m_Value_Int3Y += v.m_Value_Int3Y;
				m_Value_Int3Z += v.m_Value_Int3Z;
				m_Value_Int4X += v.m_Value_Int4X;
				m_Value_Int4Y += v.m_Value_Int4Y;
				m_Value_Int4Z += v.m_Value_Int4Z;
				m_Value_Int4W += v.m_Value_Int4W;
				m_Value_Bool += v.m_Value_Bool;
				m_Value_Char += v.m_Value_Char;
				m_Value_String += v.m_Value_String;
				}

			void operator-(const Variable &v)
				{
				m_Value_Float -= v.m_Value_Float;
				m_Value_Float2X -= v.m_Value_Float2X;
				m_Value_Float2Y -= v.m_Value_Float2Y;
				m_Value_Float3X -= v.m_Value_Float3X;
				m_Value_Float3Y -= v.m_Value_Float3Y;
				m_Value_Float3Z -= v.m_Value_Float3Z;
				m_Value_Float4X -= v.m_Value_Float4X;
				m_Value_Float4Y -= v.m_Value_Float4Y;
				m_Value_Float4Z -= v.m_Value_Float4Z;
				m_Value_Float4W -= v.m_Value_Float4W;
				m_Value_uInt -= v.m_Value_uInt;
				m_Value_Int -= v.m_Value_Int;
				m_Value_Int2X -= v.m_Value_Int2X;
				m_Value_Int2Y -= v.m_Value_Int2Y;
				m_Value_Int3X -= v.m_Value_Int3X;
				m_Value_Int3Y -= v.m_Value_Int3Y;
				m_Value_Int3Z -= v.m_Value_Int3Z;
				m_Value_Int4X -= v.m_Value_Int4X;
				m_Value_Int4Y -= v.m_Value_Int4Y;
				m_Value_Int4Z -= v.m_Value_Int4Z;
				m_Value_Int4W -= v.m_Value_Int4W;
				m_Value_Bool -= v.m_Value_Bool;
				m_Value_Char -= v.m_Value_Char;
				}

			uint m_PrecType;
			uint m_CodeType;

			float m_Value_Float;
			float m_Value_Float2X;
			float m_Value_Float2Y;
			float m_Value_Float3X;
			float m_Value_Float3Y;
			float m_Value_Float3Z;
			float m_Value_Float4X;
			float m_Value_Float4Y;
			float m_Value_Float4Z;
			float m_Value_Float4W;
			uint m_Value_uInt;
			int m_Value_Int;
			int m_Value_Int2X;
			int m_Value_Int2Y;
			int m_Value_Int3X;
			int m_Value_Int3Y;
			int m_Value_Int3Z;
			int m_Value_Int4X;
			int m_Value_Int4Y;
			int m_Value_Int4Z;
			int m_Value_Int4W;

			bool m_Value_Bool;

			char m_Value_Char;

			std::string m_Value_String;

			std::string m_MxName;

			uint8 m_Action;
		};

	extern int agFind_VarType(std::string f_String);

	extern Variable* av_Identity(uint f_PrecType);
}


#endif