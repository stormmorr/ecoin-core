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

uint emLaw::acJudge_System(emSystem* f_System, float f_QualityRank, bool f_testVolumes)
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

uint emLaw::acApply(emSystem* f_System, float f_QualityRank, bool f_testVolumes)
{
	uint f_Result = MAX_LAW;

	switch(m_Type)
		{
		case emLawType_Default:
		case emLawType_SizeTest:
			{
			if(f_testVolumes)
				{
				f_Result = acVolume(f_System, f_QualityRank, true);
				}
			}break;

		case emLawType_Forcefull:
			{
			f_Result = acMove(f_System, f_QualityRank, false);
			}break;

		case emLawType_Collisive:
			{
			f_Result = acCollide(f_System, f_QualityRank, true);
			}break;
		}

	return f_Result;
}

uint emLaw::acVolume(emSystem* f_System, float f_QualityRank, bool f_testVolumes)
{
	return MAX_LAW;
}

uint emLaw::acMove(emSystem* f_System, float f_QualityRank, bool f_testVolumes)
{
	return MAX_LAW;
}

uint emLaw::acCollide(emSystem* f_System, float f_QualityRank, bool f_testVolumes)
{
	return MAX_LAW;
}

};