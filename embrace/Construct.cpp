/*

Code - osirem.com
Copyright OSIREM LTD (C) 2016
www.bitolyl.com/osirem bitcoin-office.com ecn.world

This source is proprietary, and cannot be used, in part or in full without
the express permission of the original author. The original author retain the
rights to use, modify, and/or relicense this code without notice.

*/

#include "pch.h"

#include "Construct.h"

using namespace ecoin;

namespace ecoin
{

	void emConstruct::acEncryptHesh(CubeHESH* f_Hesh, CubeHESH* f_PreviousHesh)
		{
		for(int f_Count = 0; f_Count < f_Hesh->vec_Bicycle.size(); f_Count++)
			{
			CubeBicycle* f_CubeBicycleHesh = f_Hesh->vec_Bicycle[f_Count];
			CubeBicycle* f_CubeBicyclePrevious = f_PreviousHesh->vec_Bicycle[f_Count];

			acEncryptBicycle(f_CubeBicycleHesh, f_CubeBicyclePrevious);
			}
		}

	void emConstruct::acEncryptBicycle(CubeBicycle* f_Bicycle, CubeBicycle* f_BicyclePrev)
		{
		classCorner* f_Corner;

		Cube::BiVector f_Vertex1 = f_Bicycle->vec_Vertex[0];
		Cube::BiVector f_Vertex2 = f_Bicycle->vec_Vertex[1];
		Cube::BiVector f_Vertex3 = f_Bicycle->vec_Vertex[2];
		Cube::BiVector f_Vertex4 = f_Bicycle->vec_Vertex[3];
		Cube::BiVector f_Vertex5 = f_Bicycle->vec_Vertex[4];
		Cube::BiVector f_Vertex6 = f_Bicycle->vec_Vertex[5];
		Cube::BiVector f_Vertex7 = f_Bicycle->vec_Vertex[6];
		Cube::BiVector f_Vertex8 = f_Bicycle->vec_Vertex[7];
		}

	void emConstruct::acDecryptHesh(Cube::CubeHESH* f_Hesh)
		{
		acDecryptHeader(f_Hesh->vec_Bicycle[f_Hesh->m_adIndex[0]]);

		for(uint f_Count = 1; f_Count < 1 + m_Con_Input; f_Count++)
			{
			if(f_Count < f_Hesh->m_adIndex.size() && f_Hesh->m_adIndex[f_Count] < f_Hesh->vec_Bicycle.size())
				{
				emInput* f_Input = acDecryptInput(f_Hesh->vec_Bicycle[f_Hesh->m_adIndex[f_Count]]);
				m_vec_Input.push_back(f_Input);
				}
			}

		for(uint f_Count = 1 + m_Con_Input; f_Count < 1 + m_Con_Input + m_Con_System; f_Count++)
			{
			if(f_Count < f_Hesh->m_adIndex.size() && f_Hesh->m_adIndex[f_Count] < f_Hesh->vec_Bicycle.size())
				{
				emSystem* f_System = acDecryptSystem(f_Hesh->vec_Bicycle[f_Hesh->m_adIndex[f_Count]]);
				m_vec_System.push_back(f_System);
				}
			}

		for(uint f_Count = 1 + m_Con_Input + m_Con_System; f_Count < 1 + m_Con_Input + m_Con_System + m_Con_Theory; f_Count++)
			{
			if(f_Count < f_Hesh->m_adIndex.size() && f_Hesh->m_adIndex[f_Count] < f_Hesh->vec_Bicycle.size())
				{
				emTheory* f_Theory = acDecryptTheory(f_Hesh->vec_Bicycle[f_Hesh->m_adIndex[f_Count]]);
				m_vec_Theory.push_back(f_Theory);
				}
			}

		for(uint f_Count = 1 + m_Con_Input + m_Con_System + m_Con_Theory; f_Count < 1 + m_Con_Input + m_Con_System + m_Con_Theory + m_Con_Theorem; f_Count++)
			{
			if(f_Count < f_Hesh->m_adIndex.size() && f_Hesh->m_adIndex[f_Count] < f_Hesh->vec_Bicycle.size())
				{
				emSystem* f_System = acDecryptSystem(f_Hesh->vec_Bicycle[f_Hesh->m_adIndex[f_Count]]);
				m_vec_System.push_back(f_System);
				}
			}

		for(uint f_Count = 1 + m_Con_Input + m_Con_System + m_Con_Theory + m_Con_Theorem; f_Count < 1 + m_Con_Input + m_Con_System + m_Con_Theory + m_Con_Theorem + m_Con_Law; f_Count++)
			{
			if(f_Count < f_Hesh->m_adIndex.size() && f_Hesh->m_adIndex[f_Count] < f_Hesh->vec_Bicycle.size())
				{
				emTheory* f_Theory = acDecryptTheory(f_Hesh->vec_Bicycle[f_Hesh->m_adIndex[f_Count]]);
				m_vec_Theory.push_back(f_Theory);
				}
			}

		m_Status = eslType_Birth_Header;
		}

	void emConstruct::acDecryptHeader(CubeBicycle* f_Bicycle)
		{/////////
		// Var //
		uint f_UInt_1 = (uint)ag_Unsign(f_Bicycle->vec_Vertex[0].m_X);
		uint f_UInt_2 = (uint)ag_Unsign(f_Bicycle->vec_Vertex[1].m_X);
		uint f_UInt_3 = (uint)ag_Unsign(f_Bicycle->vec_Vertex[2].m_X);
		uint f_UInt_4 = (uint)ag_Unsign(f_Bicycle->vec_Vertex[3].m_X);
		uint f_UInt_5 = (uint)ag_Unsign(f_Bicycle->vec_Vertex[4].m_X);
		uint f_UInt_6 = (uint)ag_Unsign(f_Bicycle->vec_Vertex[5].m_X);
		uint f_UInt_7 = (uint)ag_Unsign(f_Bicycle->vec_Vertex[6].m_X);
		uint f_UInt_8 = (uint)ag_Unsign(f_Bicycle->vec_Vertex[7].m_X);

		uint f_UInt_Y_1 = (uint)ag_Unsign(f_Bicycle->vec_Vertex[0].m_Y);
		uint f_UInt_Y_2 = (uint)ag_Unsign(f_Bicycle->vec_Vertex[1].m_Y);
		uint f_UInt_Y_3 = (uint)ag_Unsign(f_Bicycle->vec_Vertex[2].m_Y);

		  //////////////////////////////////////////////////////////////////
		 // Prime - At the moment set cross referencing indexes here
		//
		if(f_UInt_1 >= 0 && f_UInt_1 < ConstructStyleTotal)
			{ //Decode prime
			m_Type = f_UInt_1;
			}
		else
			{
			throw;
			}

		if(f_UInt_2 >= 0 && f_UInt_2 < emStatus_Total)
			{ //Decode prime
			m_Status = f_UInt_2;
			}
		else
			{
			throw;
			}

		if(f_UInt_3 > 0)
			{ //Decode prime
			m_vec_eslIndex.push_back(f_UInt_3);
			}
		else
			{
			throw;
			}

		if(f_UInt_4 >= 0)
			{ //Decode prime
			m_Con_Input = f_UInt_4;
			}
		else
			{
			throw;
			}

		if(f_UInt_5 >= 0)
			{ //Decode prime
			m_Con_System = f_UInt_5;
			}
		else
			{
			throw;
			}

		if(f_UInt_6 >= 0)
			{ //Decode prime
			m_Con_Theory = f_UInt_6;
			}
		else
			{
			throw;
			}

		if(f_UInt_7 >= 0)
			{ //Decode prime
			m_Con_Theorem = f_UInt_7;
			}
		else
			{
			throw;
			}

		if(f_UInt_8 >= 0)
			{ //Decode prime
			m_Con_Law = f_UInt_8;
			}
		else
			{
			throw;
			}

		if(f_UInt_Y_1 >= 0)
			{ //Decode prime
			m_Con_Header = f_UInt_Y_1;
			}
		else
			{
			throw;
			}

		if(f_UInt_Y_2 >= 0)
			{ //Decode prime
			m_Con_Body = f_UInt_Y_2;
			}
		else
			{
			throw;
			}

		if(f_UInt_Y_3 >= 0)
			{ //Decode prime
			m_Con_ESL = f_UInt_Y_3;
			}
		else
			{
			throw;
			}

		m_vec_Header.push_back(f_Bicycle);
		}

	emInput* emConstruct::acDecryptInput(CubeBicycle* f_Bicycle)
		{
		uint f_UInt_1 = (uint)ag_Unsign(f_Bicycle->vec_Vertex[0].m_X);
		uint f_UInt_2 = (uint)ag_Unsign(f_Bicycle->vec_Vertex[1].m_X);
		uint f_UInt_3 = (uint)ag_Unsign(f_Bicycle->vec_Vertex[2].m_X);

		BiVector f_Vert3 = f_Bicycle->vec_Vertex[3];
		BiVector f_Vert4 = f_Bicycle->vec_Vertex[4];
		BiVector f_Vert5 = f_Bicycle->vec_Vertex[5];
		BiVector f_VertConclusion34 = f_Vert3 - f_Vert4;
		float f_Length34 = ag_Unsign(f_VertConclusion34.acLength());

		emInput* f_Input = new emInput();

		  //////////////
		 // Prime
		//
		if(f_UInt_1 >= 0 && f_UInt_1 < emInputType_Total)
			{ //Decode prime
			f_Input->m_Type = f_UInt_1;
			}
		else
			{
			throw;
			}

		if(f_UInt_2 >= 0 && f_UInt_2 < emStatus_Total)
			{ //Decode prime
			f_Input->m_Status = f_UInt_2;
			}
		else
			{
			throw;
			}

		if(f_UInt_3 > 0)
			{ //Decode prime
			f_Input->m_vec_eslIndex.push_back(f_UInt_3);
			}
		else
			{
			throw;
			}

		f_Input->m_Max_X = f_Vert3.m_X;
		f_Input->m_Max_Y = f_Vert3.m_Y;
		f_Input->m_Max_Z = f_Vert3.m_Z;
		f_Input->m_Max_X = f_Vert4.m_X;
		f_Input->m_Max_Y = f_Vert4.m_Y;
		f_Input->m_Max_Z = f_Vert4.m_Z;
		f_Input->m_Value = f_Vert5.m_X;
		f_Input->m_Scale = f_Length34;

		m_vec_Header.push_back(f_Bicycle);
		return f_Input;
		}

	emSystem* emConstruct::acDecryptSystem(CubeBicycle* f_Bicycle)
		{
		uint f_UInt_1 = (uint)ag_Unsign(f_Bicycle->vec_Vertex[0].m_X);
		uint f_UInt_2 = (uint)ag_Unsign(f_Bicycle->vec_Vertex[1].m_X);
		uint f_UInt_3 = (uint)ag_Unsign(f_Bicycle->vec_Vertex[2].m_X);

		BiVector f_Vert3 = f_Bicycle->vec_Vertex[3];
		BiVector f_Vert4 = f_Bicycle->vec_Vertex[4];
		BiVector f_Vert5 = f_Bicycle->vec_Vertex[5];
		BiVector f_VertConclusion34 = f_Vert3 - f_Vert4;
		float f_Length34 = ag_Unsign(f_VertConclusion34.acLength());

		emSystem* f_System = new emSystem();

		  //////////////
		 // Prime
		//
		if(f_UInt_1 >= 0 && f_UInt_1 < emInputType_Total)
			{ //Decode prime
			f_System->m_Type = f_UInt_1;
			}
		else
			{
			throw;
			}

		if(f_UInt_2 >= 0 && f_UInt_2 < emStatus_Total)
			{ //Decode prime
			f_System->m_Status = f_UInt_2;
			}
		else
			{
			throw;
			}

		if(f_UInt_3 > 0)
			{ //Decode prime
			f_System->m_vec_eslIndex.push_back(f_UInt_3);
			}
		else
			{
			throw;
			}

		f_System->m_Max_X = f_Vert3.m_X;
		f_System->m_Max_Y = f_Vert3.m_Y;
		f_System->m_Max_Z = f_Vert3.m_Z;
		f_System->m_Max_X = f_Vert4.m_X;
		f_System->m_Max_Y = f_Vert4.m_Y;
		f_System->m_Max_Z = f_Vert4.m_Z;
		f_System->m_Value = f_Vert5.m_X;
		f_System->m_Scale = f_Length34;

		m_vec_Header.push_back(f_Bicycle);
		return f_System;
		}

	emTheory* emConstruct::acDecryptTheory(CubeBicycle* f_Bicycle)
		{
		uint f_UInt_1 = (uint)ag_Unsign(f_Bicycle->vec_Vertex[0].m_X);
		uint f_UInt_2 = (uint)ag_Unsign(f_Bicycle->vec_Vertex[1].m_X);
		uint f_UInt_3 = (uint)ag_Unsign(f_Bicycle->vec_Vertex[2].m_X);

		BiVector f_Vert3 = f_Bicycle->vec_Vertex[3];
		BiVector f_Vert4 = f_Bicycle->vec_Vertex[4];
		BiVector f_Vert5 = f_Bicycle->vec_Vertex[5];
		BiVector f_VertConclusion34 = f_Vert3 - f_Vert4;
		float f_Length34 = ag_Unsign(f_VertConclusion34.acLength());

		emTheory* f_Theory = new emTheory();

		  //////////////
		 // Prime
		//
		if(f_UInt_1 >= 0 && f_UInt_1 < emInputType_Total)
			{ //Decode prime
			f_Theory->m_Type = f_UInt_1;
			}
		else
			{
			throw;
			}

		if(f_UInt_2 >= 0 && f_UInt_2 < emStatus_Total)
			{ //Decode prime
			f_Theory->m_Status = f_UInt_2;
			}
		else
			{
			throw;
			}

		if(f_UInt_3 > 0)
			{ //Decode prime
			f_Theory->m_vec_eslIndex.push_back(f_UInt_3);
			}
		else
			{
			throw;
			}

		f_Theory->m_Max_X = f_Vert3.m_X;
		f_Theory->m_Max_Y = f_Vert3.m_Y;
		f_Theory->m_Max_Z = f_Vert3.m_Z;
		f_Theory->m_Max_X = f_Vert4.m_X;
		f_Theory->m_Max_Y = f_Vert4.m_Y;
		f_Theory->m_Max_Z = f_Vert4.m_Z;
		f_Theory->m_Value = f_Vert5.m_X;
		f_Theory->m_Scale = f_Length34;

		m_vec_Header.push_back(f_Bicycle);
		return f_Theory;
		}

	emLaw* emConstruct::acDecryptLaw(CubeBicycle* f_Bicycle)
		{
		uint f_UInt_1 = (uint)ag_Unsign(f_Bicycle->vec_Vertex[0].m_X);
		uint f_UInt_2 = (uint)ag_Unsign(f_Bicycle->vec_Vertex[1].m_X);
		uint f_UInt_3 = (uint)ag_Unsign(f_Bicycle->vec_Vertex[2].m_X);

		BiVector f_Vert3 = f_Bicycle->vec_Vertex[3];
		BiVector f_Vert4 = f_Bicycle->vec_Vertex[4];
		BiVector f_Vert5 = f_Bicycle->vec_Vertex[5];
		BiVector f_VertConclusion34 = f_Vert3 - f_Vert4;
		float f_Length34 = ag_Unsign(f_VertConclusion34.acLength());

		emLaw* f_Law = new emLaw(new emSystem);

		  //////////////
		 // Prime
		//
		if(f_UInt_1 >= 0 && f_UInt_1 < emLawType_Total)
			{ //Decode prime
			f_Law->m_Type = f_UInt_1;
			}
		else
			{
			throw;
			}

		if(f_UInt_2 >= 0 && f_UInt_2 < emStatus_Total)
			{ //Decode prime
			f_Law->m_Status = f_UInt_2;
			}
		else
			{
			throw;
			}

		if(f_UInt_3 > 0)
			{ //Decode prime
			f_Law->m_vec_eslIndex.push_back(f_UInt_3);
			}
		else
			{
			throw;
			}

		f_Law->m_Max_X = f_Vert3.m_X;
		f_Law->m_Max_Y = f_Vert3.m_Y;
		f_Law->m_Max_Z = f_Vert3.m_Z;
		f_Law->m_Max_X = f_Vert4.m_X;
		f_Law->m_Max_Y = f_Vert4.m_Y;
		f_Law->m_Max_Z = f_Vert4.m_Z;
		f_Law->m_Value = f_Vert5.m_X;
		f_Law->m_Scale = f_Length34;

		m_vec_Header.push_back(f_Bicycle);
		return f_Law;
		}

};