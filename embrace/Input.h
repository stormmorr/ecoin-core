/*

Code - osirem.com
Copyright OSIREM LTD (C) 2016
www.bitolyl.com/osirem bitcoin-office.com ecn.world

This source is proprietary, and cannot be used, in part or in full without
the express permission of the original author. The original author retain the
rights to use, modify, and/or relicense this code without notice.

*/

#ifndef INPUT_H
#define INPUT_H

#include <vector>
#include <math.h>

#include "emSystem.h"

#include "Include\Cube.h"

using namespace Cube;
using namespace std;

namespace ecoin
{

	enum emInputType
	{
		emInputType_Default,
		emInputType_Force,
		emInputType_Total
	};

	class emInput
	{
	public:
		emInput(emInput* f_Input) : m_Type(f_Input->m_Type), m_Status(f_Input->m_Status)
			{
			m_R = f_Input->m_R;
			m_G = f_Input->m_G;
			m_B = f_Input->m_B;

			//m_Orten = f_Element->m_Orten;
			};
		emInput(CubeBicycle* f_Bicycle) : m_Type(ELEM_Element), m_Status(ELEM_Element)
			{
			acGen_fromBicycle(f_Bicycle);

			//m_Orten = f_Element->m_Orten;
			};
		emInput(uint f_Type) : m_Type(f_Type), m_Status(ELEM_Status_Birth)
			{
			acColorise(0.5f, 0.5f, 0.5f, 1.0f);
			};
		emInput(uint f_Type, uint f_Status) : m_Type(f_Type), m_Status(f_Status)
			{
			acColorise(0.5f, 0.5f, 0.5f, 1.0f);
			};
		emInput(uint f_Type, uint f_Status, classColor f_Color) : m_Type(f_Type), m_Status(f_Status)
			{
			acColorise(f_Color.m_R, f_Color.m_G, f_Color.m_B, f_Color.m_A);
			};
		emInput(uint f_Type, uint f_Status, classColor* f_Color) : m_Type(f_Type), m_Status(f_Status)
			{
			acColorise(f_Color->m_R, f_Color->m_G, f_Color->m_B, f_Color->m_A);
			};
		emInput(float f_Volume) : m_Type(0), m_Status(0)
			{
			m_Volume = f_Volume;
			};
		emInput() : m_Type(0), m_Status(0)
			{
			};
		virtual ~emInput() {};

		classResult* acStructuredAnalysis(void);

		void acColorise(float f_R, float f_G, float f_B, float f_A)
			{
			m_R = f_R;
			m_G = f_G;
			m_B = f_B;
			m_A = f_A;
			}

		void acColorise(classColor *f_Color)
			{
			m_R = f_Color->m_R;
			m_G = f_Color->m_G;
			m_B = f_Color->m_B;
			m_A = f_Color->m_A;
			}

		void acColorise(classColor f_Color)
			{
			m_R = f_Color.m_R;
			m_G = f_Color.m_G;
			m_B = f_Color.m_B;
			m_A = f_Color.m_A;
			}

		void acGen_fromBicycle(CubeBicycle* f_Bicycle);

		uint m_Type;
		uint m_Status;

		float m_R;
		float m_G;
		float m_B;
		float m_A;

		BiVector m_Center;
		//BiOrientation m_Orten;
		classWorth m_Worth;

		vector<emInput*> m_vec_Input_Action;

		vector<emTheory*> m_vec_Control_Theory;

		vector<emInput*> m_vec_Tree_Input;

		vector<Cube::BiVector> vec_Vertex;
		vector<Cube::BiVector> vec_Color;

		aabb* m_aabb;
		float m_Volume;

		vector<int> m_vec_Collission;
		vector<int> m_vec_IsIn;

		//vector<emJoint*> m_vec_Joint;
		// Join Inputs with force vector actions

		float acCompare(classElement* f_Element, float f_QualityRank, bool f_testVolumes);

		  ////////////////////////////////////
		 // LEGACY of Structured Analysis
		//
		//vector<classResult*> m_vec_Result;
		int m_Base;

		vector<uint> m_vec_eslIndex;

		  ////////////////////////////
		 // Input
		//
		BiVector m_Value;

		float m_Max_X;
		float m_Max_Y;
		float m_Max_Z;
		float m_Min_X;
		float m_Min_Y;
		float m_Min_Z;

		float m_Scale;

		BiVector m_Force[256];
		BiVector m_ColorVariance[256];

		   ////////////////////////////
		  //
		 // Automation
		//
		vector<BiVector> m_vec_Auto_Way;
		vector<BiVector> m_vec_Auto_Adjustment;
		vector<int> m_vec_Auto_Way_Layout;
		vector<int> m_vec_Auto_Adjustment_Layout;
	};

};

#endif