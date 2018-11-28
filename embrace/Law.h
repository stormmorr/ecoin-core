/*

Code - osirem.com
Copyright OSIREM LTD (C) 2016
www.bitolyl.com/osirem bitcoin-office.com ecn.world

This source is proprietary, and cannot be used, in part or in full without
the express permission of the original author. The original author retain the
rights to use, modify, and/or relicense this code without notice.

*/

#ifndef LAW_H
#define LAW_H

#include <vector>
#include <math.h>

#include "emSystem.h"

#define MAX_LAW 0
#define NO_LAW 16

namespace ecoin
{

	enum LawType
	{
		emLawType_Constraint,
		emLawType_Entitlement
	};

	enum LawClass
	{
		emLawType_Default,
		emLawType_SizeTest,
		emLawType_Forcefull,
		emLawType_Union,
		emLawType_Collisive,
		emLawType_Reactive,
		emLawType_Heat,
		emLawType_Light,
		emLawType_Natural,
		emLawType_Mechanical,
		emLawType_Chemical,
		emLawType_Ethereal,
		emLawType_Mathematical,
		emLawType_Special,
		emLawType_Total
	};

	class emSystem;

	class emLaw : emSystem
	{
	public:
		emLaw(emSystem* f_emSystem) : emSystem(*(f_emSystem)) {};
		~emLaw() {};

		uint acApply(emSystem* f_System, float f_QualityRank, bool f_testVolumes);
		uint acJudge_System(emSystem* f_System, float f_QualityRank, bool f_testVolumes);

		uint acVolume(emSystem* f_System, float f_QualityRank, bool f_testVolumes);
		uint acMove(emSystem* f_System, float f_QualityRank, bool f_testVolumes);
		uint acCollide(emSystem* f_System, float f_QualityRank, bool f_testVolumes);
		
		float m_Width;
		float m_Height;
		float m_Length;

		BiVector m_Position;
		matrix m_Orientation;

		uint m_Type;
		uint m_Status;
		uint m_Separation;

		float m_Max_X;
		float m_Max_Y;
		float m_Max_Z;
		float m_Min_X;
		float m_Min_Y;
		float m_Min_Z;

		float m_Scale;
		int m_Count;

		BiVector m_Value;

		vector<uint> m_vec_eslIndex;
	};

};

#endif