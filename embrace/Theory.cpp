/*

Code - osirem.com
Copyright OSIREM LTD (C) 2016
www.bitolyl.com/osirem bitcoin-office.com ecn.world

This source is proprietary, and cannot be used, in part or in full without
the express permission of the original author. The original author retain the
rights to use, modify, and/or relicense this code without notice.

*/

#include "pch.h"

#include "Theory.h"
#include "Law.h"
#include "Input.h"

using namespace ecoin;

namespace ecoin
{

uint emTheory::acTheorise_System(emSystem* f_System, float f_QualityRank, bool f_testVolumes)
{
	f_System->acProcessInputs();

	if(f_System->m_Status != emStatus_Inputs)
		{
		return 255;
		}
	
	acApply(f_System, f_QualityRank, f_testVolumes);

	for(int aero = 0; aero < f_System->m_vec_Tree_emSystem.size(); aero++)
		{
		//emSystem* f_System = f_System->m_vec_Tree_emSystem[aero];

		f_System->acProcessInputs();
		}
}

uint emTheory::acApply(emSystem* f_System, float f_QualityRank, bool f_testVolumes)
{
	uint f_Result = MAX_LAW;

	switch(m_Type)
		{
		case TheoryType_Default:
		case TheoryType_SizeTest:
			{
			if(f_testVolumes)
				{
				f_Result = acVolume(f_System, f_QualityRank);
				}
			}break;

		case TheoryType_MovementAnalysis:
			{
			f_Result = acMove(f_System, f_QualityRank);
			}break;

		case TheoryType_Collision:
			{
			f_Result = acCollide(f_System, f_QualityRank);
			}break;
		}

	return f_Result;
}

uint emTheory::acVolume(emSystem* f_System, float f_QualityRank)
{
	return MAX_LAW;
}

uint emTheory::acMove(emSystem* f_System, float f_QualityRank)
{
	return MAX_LAW;
}

uint emTheory::acCollide(emSystem* f_System, float f_QualityRank)
{
	return MAX_LAW;
}

};