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

void emSystem::acGen_fromBicycle(CubeBicycle *f_Bicycle)
{
	vec_Vertex.push_back(f_Bicycle->vec_Vertex[0]);
	vec_Vertex.push_back(f_Bicycle->vec_Vertex[1]);
	vec_Vertex.push_back(f_Bicycle->vec_Vertex[2]);
	vec_Vertex.push_back(f_Bicycle->vec_Vertex[3]);
	vec_Vertex.push_back(f_Bicycle->vec_Vertex[4]);
	vec_Vertex.push_back(f_Bicycle->vec_Vertex[5]);
	vec_Vertex.push_back(f_Bicycle->vec_Vertex[6]);
	vec_Vertex.push_back(f_Bicycle->vec_Vertex[7]);

	vec_Color.push_back(f_Bicycle->vec_Color[0]);
	vec_Color.push_back(f_Bicycle->vec_Color[1]);
	vec_Color.push_back(f_Bicycle->vec_Color[2]);
	vec_Color.push_back(f_Bicycle->vec_Color[3]);
	vec_Color.push_back(f_Bicycle->vec_Color[4]);
	vec_Color.push_back(f_Bicycle->vec_Color[5]);
	vec_Color.push_back(f_Bicycle->vec_Color[6]);
	vec_Color.push_back(f_Bicycle->vec_Color[7]);

	m_aabb = new aabb();
	m_aabb->empty();

	for(unsigned int f_CountVertex = 0; f_CountVertex < 8; f_CountVertex++)
		{
		*(m_aabb) += vec_Vertex[f_CountVertex];
		}

	m_Center = m_aabb->center();
	m_Volume = m_aabb->volume();
}

classResult* emSystem::acStructuredAnalysis(void)
{
	classResult* f_Result = new classResult();
	classCorner* f_Corner = new classCorner();

	f_Corner->acSpecify(vec_Vertex[0], vec_Vertex[1], vec_Vertex[4], vec_Vertex[2]);
	f_Result->m_vec_Corner.push_back(f_Corner);

	f_Corner = new classCorner();
	f_Corner->acSpecify(vec_Vertex[1], vec_Vertex[0], vec_Vertex[5], vec_Vertex[3]);
	f_Result->m_vec_Corner.push_back(f_Corner);

	f_Corner = new classCorner();
	f_Corner->acSpecify(vec_Vertex[2], vec_Vertex[3], vec_Vertex[6], vec_Vertex[0]);
	f_Result->m_vec_Corner.push_back(f_Corner);

	f_Corner = new classCorner();
	f_Corner->acSpecify(vec_Vertex[3], vec_Vertex[2], vec_Vertex[7], vec_Vertex[1]);
	f_Result->m_vec_Corner.push_back(f_Corner);

	f_Corner = new classCorner();
	f_Corner->acSpecify(vec_Vertex[4], vec_Vertex[5], vec_Vertex[0], vec_Vertex[6]);
	f_Result->m_vec_Corner.push_back(f_Corner);

	f_Corner = new classCorner();
	f_Corner->acSpecify(vec_Vertex[5], vec_Vertex[4], vec_Vertex[1], vec_Vertex[7]);
	f_Result->m_vec_Corner.push_back(f_Corner);

	f_Corner = new classCorner();
	f_Corner->acSpecify(vec_Vertex[6], vec_Vertex[7], vec_Vertex[2], vec_Vertex[4]);
	f_Result->m_vec_Corner.push_back(f_Corner);

	f_Corner = new classCorner();
	f_Corner->acSpecify(vec_Vertex[7], vec_Vertex[6], vec_Vertex[3], vec_Vertex[5]);
	f_Result->m_vec_Corner.push_back(f_Corner);

	BiVector f_Vec;

	f_Vec = vec_Vertex[1] - vec_Vertex[0];
	f_Result->m_Dist01 = f_Vec.acLength();

	f_Vec = vec_Vertex[2] - vec_Vertex[3];
	f_Result->m_Dist23 = f_Vec.acLength();

	f_Vec = vec_Vertex[4] - vec_Vertex[5];
	f_Result->m_Dist45 = f_Vec.acLength();

	f_Vec = vec_Vertex[6] - vec_Vertex[7];
	f_Result->m_Dist67 = f_Vec.acLength();

	f_Vec = vec_Vertex[0] - vec_Vertex[2];
	f_Result->m_Dist02 = f_Vec.acLength();

	f_Vec = vec_Vertex[1] - vec_Vertex[3];
	f_Result->m_Dist13 = f_Vec.acLength();

	f_Vec = vec_Vertex[4] - vec_Vertex[6];
	f_Result->m_Dist46 = f_Vec.acLength();

	f_Vec = vec_Vertex[5] - vec_Vertex[7];
	f_Result->m_Dist57 = f_Vec.acLength();

	f_Vec = vec_Vertex[0] - vec_Vertex[4];
	f_Result->m_Dist04 = f_Vec.acLength();

	f_Vec = vec_Vertex[1] - vec_Vertex[5];
	f_Result->m_Dist15 = f_Vec.acLength();

	f_Vec = vec_Vertex[2] - vec_Vertex[6];
	f_Result->m_Dist26 = f_Vec.acLength();

	f_Vec = vec_Vertex[3] - vec_Vertex[7];
	f_Result->m_Dist37 = f_Vec.acLength();

	f_Result->m_Dist01234567 = (f_Result->m_Dist01 + f_Result->m_Dist23 + f_Result->m_Dist45 + f_Result->m_Dist67) / 4; //X
	f_Result->m_Dist02134657 = (f_Result->m_Dist02 + f_Result->m_Dist13 + f_Result->m_Dist46 + f_Result->m_Dist57) / 4; //Z
	f_Result->m_Dist04152637 = (f_Result->m_Dist04 + f_Result->m_Dist15 + f_Result->m_Dist26 + f_Result->m_Dist37) / 4; //Y

	f_Result->m_SurfToLegRatioX = ((f_Result->m_Dist01234567 - f_Result->m_Dist02134657) + (f_Result->m_Dist01234567 - f_Result->m_Dist04152637));
	f_Result->m_SurfToLegRatioZ = ((f_Result->m_Dist02134657 - f_Result->m_Dist01234567) + (f_Result->m_Dist02134657 - f_Result->m_Dist04152637));
	f_Result->m_SurfToLegRatioY = ((f_Result->m_Dist04152637 - f_Result->m_Dist02134657) + (f_Result->m_Dist04152637 - f_Result->m_Dist01234567));

	if((f_Result->m_SurfToLegRatioX > f_Result->m_SurfToLegRatioY) && (f_Result->m_SurfToLegRatioX > f_Result->m_SurfToLegRatioZ))
		{
		if((f_Result->m_SurfToLegRatioY < f_Result->m_SurfToLegRatioX) && (f_Result->m_SurfToLegRatioY < f_Result->m_SurfToLegRatioZ))
			{
			f_Result->m_SurfToLegRatio = f_Result->m_SurfToLegRatioY / f_Result->m_SurfToLegRatioX;
			}
		else if((f_Result->m_SurfToLegRatioZ < f_Result->m_SurfToLegRatioX) && (f_Result->m_SurfToLegRatioZ < f_Result->m_SurfToLegRatioY))
			{
			f_Result->m_SurfToLegRatio = f_Result->m_SurfToLegRatioZ / f_Result->m_SurfToLegRatioX;
			}
		}
	else if((f_Result->m_SurfToLegRatioY > f_Result->m_SurfToLegRatioX) && (f_Result->m_SurfToLegRatioY > f_Result->m_SurfToLegRatioZ))
		{
		if((f_Result->m_SurfToLegRatioX < f_Result->m_SurfToLegRatioY) && (f_Result->m_SurfToLegRatioX < f_Result->m_SurfToLegRatioZ))
			{
			f_Result->m_SurfToLegRatio = f_Result->m_SurfToLegRatioX / f_Result->m_SurfToLegRatioY;
			}
		else if((f_Result->m_SurfToLegRatioZ < f_Result->m_SurfToLegRatioX) && (f_Result->m_SurfToLegRatioZ < f_Result->m_SurfToLegRatioY))
			{
			f_Result->m_SurfToLegRatio = f_Result->m_SurfToLegRatioZ / f_Result->m_SurfToLegRatioY;
			}
		}
	else if((f_Result->m_SurfToLegRatioZ > f_Result->m_SurfToLegRatioY) && (f_Result->m_SurfToLegRatioZ > f_Result->m_SurfToLegRatioX))
		{
		if((f_Result->m_SurfToLegRatioX < f_Result->m_SurfToLegRatioY) && (f_Result->m_SurfToLegRatioX < f_Result->m_SurfToLegRatioZ))
			{
			f_Result->m_SurfToLegRatio = f_Result->m_SurfToLegRatioX / f_Result->m_SurfToLegRatioZ;
			}
		else if((f_Result->m_SurfToLegRatioY < f_Result->m_SurfToLegRatioX) && (f_Result->m_SurfToLegRatioY < f_Result->m_SurfToLegRatioZ))
			{
			f_Result->m_SurfToLegRatio = f_Result->m_SurfToLegRatioY / f_Result->m_SurfToLegRatioZ;
			}
		}

	f_Result->m_LevelParent = -1;
	f_Result->m_Parent = nullptr;

	//BiPlane m_ProportionalPlane;

	//m_vec_Result.push_back(f_Result);
	return f_Result;
}

#if 0
float emSystem::acCompare(emSystem* f_Element, float f_QualityRank, bool f_testVolumes)
{
	float f_Mark = 0;

	if(m_vec_Element.size() != f_Element->m_vec_Element.size())
		{
		f_Mark += 37.8f;
		}
	else
		{
		if(f_testVolumes)
			{
			for(int f_Count = 0; f_Count < m_vec_Element.size(); f_Count++)
				{
				emSystem* f_TestElement = m_vec_Element[f_Count];
				emSystem* f_sizeTestElement = f_Element->m_vec_Element[f_Count];

				if((f_TestElement->m_Volume < (f_sizeTestElement->m_Volume - f_QualityRank)) || (f_TestElement->m_Volume > (f_sizeTestElement->m_Volume + f_QualityRank)))
					{
					float f_Float = f_TestElement->m_Volume - f_sizeTestElement->m_Volume;

#if 1
					if(f_Float >= 0)
						{
						f_Mark += f_Float;
						}
					else
						{
						f_Mark -= f_Float;
						}
#else
					f_Mark += f_Float;
#endif
					}
				}
			}
		}

	if(m_vec_Surface.size() != f_Element->m_vec_Surface.size())
		{
		f_Mark += 14.8f;
		}

	if(m_vec_Leg.size() != f_Element->m_vec_Leg.size())
		{
		f_Mark += 8.58f;
		}

	return f_Mark;
}
#endif

void emSystem::acProcessInputs(void)
{
	if(m_Status == emStatus_Live)
		{
		m_Status = emStatus_Processing;

		for(int helly = 0; helly < m_vec_Input_Action.size(); helly++)
			{
			emInput* f_Input = m_vec_Input_Action[helly];

			acApply(f_Input);
			}

		m_Status = emStatus_Inputs;
		}
	else
		{
		if (m_Status == emStatus_Processing)
			{
			printf("emError System Processing\n");
			}
		else if(m_Status == emStatus_Inputs)
			{
			printf("emError System in Inpute Mode Already\n");
			}
		else
			{
			printf("emError System in Birth Mode\n");

			m_Status = emStatus_Error;
			}
		}

	for(int aero = 0; aero < m_vec_Input_Action.size(); aero++)
		{
		emSystem* f_System = m_vec_Tree_emSystem[aero];

		f_System->acProcessInputs();
		}
}

bool emSystem::acApply(emInput* f_Input)
{
	if(m_Type != eslType_Live)
		{
		printf("emError Apply Input Type not Live\n");
		return false;
		}

	switch(f_Input->m_Type)
		{
		case emInputType_Default:
		case emInputType_Force:
			{
			for(int helly = 0; helly < vec_Vertex.size(); helly++)
				{
				vec_Vertex[helly] += f_Input->m_Force[helly] * g_TimeScale;
				vec_Color[helly] += f_Input->m_ColorVariance[helly] * g_TimeScale;
				}
			}break;
		}

	return true;
}

};