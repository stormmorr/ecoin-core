/*

Code - osirem.com
Copyright OSIREM LTD (C) 2016
www.bitolyl.com/osirem bitcoin-office.com ecn.world

This source is proprietary, and cannot be used, in part or in full without
the express permission of the original author. The original author retain the
rights to use, modify, and/or relicense this code without notice.

*/

#ifndef THEORY_H
#define THEORY_H

#include <vector>
#include <math.h>

#include "emSystem.h"

#include "Include/Cube.h"

using namespace Cube;

namespace ecoin
{

	enum TheoryType
	{
		TheoryType_Default,
		TheoryType_SizeTest,
		TheoryType_MovementAnalysis,
		TheoryType_Collision,
		TheoryType_Total
	};

	class emSystem;

	class emTheory : emSystem
	{
	public:
		emTheory(emSystem* f_Element) : emSystem(*(f_Element)) {};
		emTheory() : emSystem() {};
		~emTheory() {};

		uint acApply(emSystem* f_System, float f_QualityRank, bool f_testVolumes);
		uint acTheorise_System(emSystem* f_System, float f_QualityRank, bool f_testVolumes);

		uint acVolume(emSystem* f_System, float f_QualityRank);
		uint acMove(emSystem* f_System, float f_QualityRank);
		uint acCollide(emSystem* f_System, float f_QualityRank);
		
		void acSpecify(BiVector f_Theory, BiVector f_X, BiVector f_Y, BiVector f_Z)
			{
			m_Theory = f_Theory;
			m_X = f_X;
			m_Y = f_Y;
			m_Z = f_Z;
			}

		bool acClassify(void);

		uint m_Type;
		uint m_Status;

		BiVector m_Theory;
		BiVector m_X;
		BiVector m_Y;
		BiVector m_Z;

		float m_Max_X;
		float m_Max_Y;
		float m_Max_Z;
		float m_Min_X;
		float m_Min_Y;
		float m_Min_Z;

		float m_Scale;

		BiVector m_Value;

		vector<uint> m_vec_eslIndex;
	};

	

};

#endif