/*

Code - osirem.com
Copyright OSIREM LTD (C) 2016
www.bitolyl.com/osirem bitcoin-office.com ecn.world

This source is proprietary, and cannot be used, in part or in full without
the express permission of the original author. The original author retain the
rights to use, modify, and/or relicense this code without notice.

*/

#ifndef EMBSYSTEM_H
#define EMBSYSTEM_H

#include <vector>
#include <math.h>

#include "emColor.h"

using namespace std;

namespace ecoin
{

	enum emSystemType
	{
		emSystemType_Default,
		emSystemType_Total
	};

	enum emStatus
	{
		emStatus_Birth,
		emStatus_Birth_Header_On,
		emStatus_Birth_Body_On,
		emStatus_Live,
		emStatus_Processing,
		emStatus_Inputs,
		emStatus_Accepted,
		emStatus_Error,
		emStatus_Total
	};

	enum eslType
	{
		eslType_Birth,
		eslType_Birth_Header,
		eslType_Birth_Body,
		eslType_Birth_ESL,
		eslType_Live,
		eslType_Total
	};

	class emSystem;
	class emTheory;
	class emTheorem;
	class emLaw;
	class emResult;
	class emInput;
	class emJoint;

	class emForce
	{
	public:
		emForce()
		{
			m_Aggressor = nullptr;
			m_Saint = nullptr;
		};
		emForce(emSystem* f_A, emSystem* f_S)
		{
			m_Aggressor = f_A;
			m_Saint = f_S;
		};
		~emForce() {};

		float m_Newton;

		emSystem* m_Aggressor;
		emSystem* m_Saint;
	};

	class emSystem
	{
	public:
		emSystem(emSystem* f_emSystem) : m_Type(f_emSystem->m_Type), m_Status(f_emSystem->m_Status)
			{
			m_R = f_emSystem->m_R;
			m_G = f_emSystem->m_G;
			m_B = f_emSystem->m_B;

			//m_Orten = f_Element->m_Orten;
			};
		emSystem(CubeBicycle* f_Bicycle) : m_Type(ELEM_Element), m_Status(ELEM_Element)
			{
			acGen_fromBicycle(f_Bicycle);

			//m_Orten = f_Element->m_Orten;
			};
		emSystem(uint f_Type) : m_Type(f_Type), m_Status(ELEM_Status_Birth)
			{
			acColorise(0.5f, 0.5f, 0.5f, 1.0f);
			};
		emSystem(uint f_Type, uint f_Status) : m_Type(f_Type), m_Status(f_Status)
			{
			acColorise(0.5f, 0.5f, 0.5f, 1.0f);
			};
		emSystem(uint f_Type, uint f_Status, classColor f_Color) : m_Type(f_Type), m_Status(f_Status)
			{
			acColorise(f_Color.m_R, f_Color.m_G, f_Color.m_B, f_Color.m_A);
			};
		emSystem(uint f_Type, uint f_Status, classColor* f_Color) : m_Type(f_Type), m_Status(f_Status)
			{
			acColorise(f_Color->m_R, f_Color->m_G, f_Color->m_B, f_Color->m_A);
			};
		emSystem(float f_Volume) : m_Type(0), m_Status(0)
			{
			m_Volume = f_Volume;
			};
		emSystem() : m_Type(0), m_Status(0)
			{
			};
		virtual ~emSystem() {};

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

		bool acApply(emInput* f_Input);

		void acProcessInputs(void);

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

		vector<emResult*> m_vec_Output_Result;

		vector<emSystem*> m_vec_Tree_emSystem;

		vector<uint> m_vec_eslIndex;

		vector<Cube::BiVector> vec_Vertex;
		vector<Cube::BiVector> vec_Color;

		aabb* m_aabb;
		float m_Volume;

		float m_Max_X;
		float m_Max_Y;
		float m_Max_Z;
		float m_Min_X;
		float m_Min_Y;
		float m_Min_Z;

		float m_Scale;

		BiVector m_Value;

		vector<int> m_vec_Collission;
		vector<int> m_vec_IsIn;

		vector<emJoint*> m_vec_Joint;
		// Join emSystems with force vector actions

		float acCompare(classElement* f_Element, float f_QualityRank, bool f_testVolumes);

		  ////////////////////////////////////
		 // LEGACY of Structured Analysis
		//
		//vector<classResult*> m_vec_Result;
		int m_Base;
	};

};

#endif