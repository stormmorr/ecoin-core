/*

Code - osirem.com
Copyright OSIREM LTD (C) 2016
www.bitolyl.com/osirem bitcoin-office.com ecn.world

This source is proprietary, and cannot be used, in part or in full without
the express permission of the original author. The original author retain the
rights to use, modify, and/or relicense this code without notice.

*/

#ifndef CONSTRUCT_H
#define CONSTRUCT_H

#include <vector>
#include <math.h>

#include "Theory.h"
#include "Law.h"
#include "Input.h"

using namespace Cube;

namespace ecoin
{

	enum ConstructStyle
	{
		emCON_Style_Micro,
		emCON_Style_Micro_Biological,
		emCON_Style_Macro_Small,
		emCON_Style_Macro_Medium,
		emCON_Style_Macro_Large,
		emCON_Style_Macro_Huge,
		emCON_Style_Macro_Planet,
		emCON_Style_Macro_Solar,
		emCON_Style_Macro_Galactic,
		emCON_Style_Macro_Universal,
		ConstructStyleTotal
	};

	class emConstruct
	{
	public:
		emConstruct() : m_Type(emCON_Style_Macro_Small), m_Status(emStatus_Birth)
			{
			acClear();
			};
		emConstruct(uint f_Type) : m_Type(f_Type), m_Status(emStatus_Birth)
			{
			acClear();
			};
		~emConstruct()
			{
			acClear();
			};

		void acClear(void)
			{
			m_Con_Header = 0;
			m_Con_Body = 0;
			m_Con_ESL = 0;
			m_Nom_Header = 0;
			m_Nom_Body = 0;
			m_Nom_ESL = 0;
			m_Con_Input = 0;
			m_Con_System = 0;
			m_Con_Theory = 0;
			m_Con_Theorem = 0;
			m_Con_Law = 0;
			m_vec_Header.clear();
			m_vec_Body.clear();
			m_vec_ESL.clear();
			m_vec_eslIndex.clear();
			m_vec_Input.clear();
			m_vec_System.clear();
			m_vec_Theory.clear();
			m_vec_Theorem.clear();
			m_vec_Law.clear();
			}

		void acEncryptBicycle(CubeBicycle* f_Bicycle, CubeBicycle* f_BicyclePrev);

		void acEncryptHesh(CubeHESH* f_Hesh, CubeHESH* f_PreviousHesh);
		void acDecryptHesh(Cube::CubeHESH* f_Hesh);
		void acDecryptHeader(CubeBicycle* f_Bicycle);
		emInput* acDecryptInput(CubeBicycle* f_Bicycle);
		emSystem* acDecryptSystem(CubeBicycle* f_Bicycle);
		emTheory* acDecryptTheory(CubeBicycle* f_Bicycle);
		emLaw* acDecryptLaw(CubeBicycle* f_Bicycle);

		classWorth* acStructuredAnalysis(/*target*/);

		uint m_Type;
		uint m_Status;

		vector<uint> m_vec_eslIndex;

		float m_Scale;

		uint m_Con_Header;
		uint m_Con_Body;
		uint m_Con_ESL;
		uint m_Nom_Header;
		uint m_Nom_Body;
		uint m_Nom_ESL;

		vector<CubeBicycle*> m_vec_Header;
		vector<CubeBicycle*> m_vec_Body;
		vector<CubeBicycle*> m_vec_ESL;

		uint m_Con_Input;
		uint m_Con_System;
		uint m_Con_Theory;
		uint m_Con_Theorem;
		uint m_Con_Law;

		vector<emInput*> m_vec_Input;
		vector<emSystem*> m_vec_System;
		vector<emTheory*> m_vec_Theory;
		vector<emTheorem*> m_vec_Theorem;
		vector<emLaw*> m_vec_Law;
		//BiOrientation m_Orten;
	};

};

#endif