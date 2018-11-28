/*

Color - osirem.com
Copyright OSIREM LTD (C) 2016
www.bitolyl.com/osirem bitcoin-office.com ecn.world

This source is proprietary, and cannot be used, in part or in full without
the express permission of the original author. The original author retain the
rights to use, modify, and/or relicense this code without notice.

*/

#ifndef COLOR_H
#define COLOR_H

#include <vector>
#include <math.h>

namespace ecoin
{

	enum
	{
		ELEM_CStatus_Birth,
		ELEM_CStatus_Processing,
		ELEM_CStatus_Accepted,
		ELEM_CStatus_Total
	};

	class classColor
	{
	public:
		classColor() : m_Status(ELEM_CStatus_Birth)
			{
			m_R = 0.5f;
			m_G = 0.5f;
			m_B = 0.5f;
			m_A = 1.0f;
			};

		classColor(float f_R, float f_G, float f_B, float f_A) : m_Status(ELEM_CStatus_Birth)
			{
			m_R = f_R;
			m_G = f_G;
			m_B = f_B;
			m_A = f_A;
			};

		void operator+=(const classColor &f_Color)
			{
			m_R += f_Color.m_R;
			m_G += f_Color.m_G;
			m_B += f_Color.m_B;

			acDivide(2.0f);
			}

		void operator*=(const classColor &f_Color)
			{
			m_R *= f_Color.m_R;
			m_G *= f_Color.m_G;
			m_B *= f_Color.m_B;

			acDivide(3.0f);
			}

		void operator/=(const float &f)
			{
			m_R /= f;
			m_G /= f;
			m_B /= f;
			}

		void acDivide(float f)
			{
			m_R /= f;
			m_G /= f;
			m_B /= f;
			}

		void acBurn(classColor f_Color, float f_Division)
			{
			m_R += (f_Color.m_R * f_Color.m_R) * 2;
			m_G += (f_Color.m_G * f_Color.m_G);
			m_B += (f_Color.m_B * f_Color.m_B) / 2;
			acDivide(f_Division);
			}

		uint m_Status;

		float m_R;
		float m_G;
		float m_B;
		float m_A;
	};

};

#endif