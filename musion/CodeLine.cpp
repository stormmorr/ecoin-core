/*

CodeLine - osirem.com
Copyright OSIREM LTD (C) 2016
www.osirem.com www.qage.org www.geopomp.com

This source is proprietary, and cannot be used, in part or in full without
the express permission of the original author. The original author retain the
rights to use, modify, and/or relicense this code without notice.

*/

#include "pch.h"

#include "Code.h"
#include "CodeLine.h"

using namespace ecoin;

namespace ecoin
{

CodeLine::CodeLine(std::string f_Line, uint f_CHK, System* f_System)
	{ ////////////////
	 ////////////////
	// Construct
	//
	//
	m_Chk = f_CHK;
	m_System = f_System;

	bool f_Scan = true;
	int f_Size = f_Line.size();

	if(m_Chk < f_Size - 6)
		{
		Code* f_CodeB = new Code(f_Line, m_Chk, 0);

		if (f_CodeB->m_Code_DigitA == '#' ||
			f_CodeB->m_Code_DigitB == '#' ||
			f_CodeB->m_Code_DigitC == '#')
			{
			f_Scan = false;
			}
		else
			{
			m_vec_Code.push_back(f_CodeB);
			}

		  //////////
		 //////////
		// END
		if(f_Scan)
			{
			m_END = f_CodeB->acDecide_END();
			m_Chk = f_CodeB->acDecide_MAX();

			uint f_Cntr = 1;

			while(f_Scan)
				{
				if(m_Chk < f_Line.size() - 6)
					{
					Code* f_CodeA = new Code(f_Line, m_Chk, f_Cntr);

					m_Chk = f_CodeA->m_Chk;

					if(f_CodeA->m_Code_DigitA == '/' &&
						f_CodeA->m_Code_DigitB == '/')
						{
						f_Scan = false;
						}

					if (f_CodeA->m_Code_DigitA == '#' ||
						f_CodeA->m_Code_DigitB == '#' ||
						f_CodeA->m_Code_DigitC == '#')
						{
						f_Scan = false;
						}
					else
						{
						m_vec_Code.push_back(f_CodeA);
						}
					}
				else
					{
					f_Scan = false;
					}

				f_Cntr++;
				}
			}
		}

	  //////////////////////
	 //////////////
	// Setup
	//
	//
	uint f_CodeSize = m_vec_Code.size();

	uint f_Count = 0;
	while(f_Count < f_CodeSize)
		{
		Code* f_Code = m_vec_Code[f_Count];

		if(f_Code->m_Code_DigitB != '/' &&
			f_Code->m_Code_DigitB != '#')
			{
			switch(f_Code->m_Code)
				{
				case MuCode_Var:
					{
					if(f_Count > 0)
						{
						if(((m_CodeLine != MuLine_Assign_Opr) &&
							(m_CodeLine != MuLine_Function)))
							{
							m_CodeLine = MuLine_Assign;
							}

						Variable* f_Var = new Variable(f_Code->m_Name[0]->m_MxName, f_Code->m_Code, f_Code->m_Type[0]->m_VarType);
						m_vec_Variable.push_back(f_Var);

						if(f_Code->m_Number.size() == 1)
							{
							Variable* f_VarFRT = new Variable("Con", MuCode_Constant, f_Code->m_Type[0]->m_VarType);
							*(f_VarFRT) = *(f_Code->m_Number[0]->m_MxVar);
							m_vec_Variable.push_back(f_VarFRT);
							}
						else if(f_Code->m_Number.size() > 1)
							{
#ifdef ECOIN_SECURE
							throw;
#endif
							}
						}
					else
						{
						if(((m_CodeLine != MuLine_Assign_Opr) &&
							(m_CodeLine != MuLine_Function)))
							{
							m_CodeLine = MuLine_Init_Var;
							}

						if((f_Code->m_Name.size() > 0) &&
							(f_Code->m_Type.size() > 0))
							{
							Variable* f_Var = new Variable(f_Code->m_Name[0]->m_MxName, f_Code->m_Code, f_Code->m_Type[0]->m_VarType);
							m_vec_Variable.push_back(f_Var);
							}

						if(f_Code->m_Number.size() == 1)
							{
							Variable* f_VarFRT = new Variable("Con", MuCode_Constant, f_Code->m_Type[0]->m_VarType);
							*(f_VarFRT) = *(f_Code->m_Number[0]->m_MxVar);
							m_vec_Variable.push_back(f_VarFRT);
							}
						else if(f_Code->m_Number.size() > 1)
							{
#ifdef ECOIN_SECURE
							throw;
#endif
							}
						}
					}break;

				case MuCode_Function:
					{
					if(((m_CodeLine != MuLine_Assign_Opr) &&
						(m_CodeLine != MuLine_Function)))
						{
						m_CodeLine = MuLine_Function;
						}

					Code* f_ParamCode1 = m_vec_Code[f_Code->m_Index + 1];
					Code* f_ParamCode2 = m_vec_Code[f_Code->m_Index + 2];

					//generate new function
					Function* f_Function = new Function(f_Code->m_Type[1]->m_VarType);

					f_Function->acPrep_addVariable(f_Code->m_MxVar);
					f_Function->acPrep_addVariable(f_ParamCode1->m_MxVar);
					f_Function->acPrep_addVariable(f_ParamCode2->m_MxVar);

					m_vec_Variable.push_back(f_Code->m_MxVar);
					m_vec_Variable.push_back(f_ParamCode1->m_MxVar);
					m_vec_Variable.push_back(f_ParamCode2->m_MxVar);

					m_vec_Function.push_back(f_Function);
					}break;

				case MuCode_System:
					{
					if(f_Count > 0)
						{
						if(((m_CodeLine != MuLine_Assign_Opr) &&
							(m_CodeLine != MuLine_Function)))
							{
							m_CodeLine = MuLine_Assign;
							}

						Variable* f_Var = m_System->av_Var(f_Code->m_Name[0]->m_MxName);
						m_vec_Variable.push_back(f_Var);

						if(f_Code->m_Number.size() == 1)
							{
							Variable* f_VarFRT = new Variable("Con", MuCode_Constant, f_Code->m_Type[0]->m_VarType);
							*(f_VarFRT) = *(f_Code->m_Number[0]->m_MxVar);
							m_vec_Variable.push_back(f_VarFRT);
							}
						else if(f_Code->m_Number.size() > 1)
							{
#ifdef ECOIN_SECURE
							throw;
#endif
							}
						}
					else
						{
						if(((m_CodeLine != MuLine_Assign_Opr) &&
							(m_CodeLine != MuLine_Function)))
							{
							m_CodeLine = MuLine_Init_Var;
							}

						Variable* f_Var = m_System->av_Var(f_Code->m_Name[0]->m_MxName);
						m_vec_Variable.push_back(f_Var);
						}
					}break;

				case MuCode_Constant:
					{
					if(((m_CodeLine != MuLine_Assign_Opr) &&
						(m_CodeLine != MuLine_Function)))
						{
						m_CodeLine = MuLine_Init_Var;
						}

					Variable* f_Var = new Variable(f_Code->m_Name[0]->m_MxName, MuCode_Constant, f_Code->m_Type[0]->m_VarType);
					m_vec_Variable.push_back(f_Var);

					if(f_Code->m_Number.size() == 1)
						{
						Variable* f_VarFRT = new Variable("Con", MuCode_Constant, f_Code->m_Type[0]->m_VarType);
						*(f_VarFRT) = *(f_Code->m_Number[0]->m_MxVar);
						m_vec_Variable.push_back(f_VarFRT);
						}
					else if(f_Code->m_Number.size() > 1)
						{
#ifdef ECOIN_SECURE
						throw;
#endif
						}
					}break;

				case MuCode_Operator:
					{
					m_CodeLine = MuLine_Assign_Opr;

					Code* f_LtCode = m_vec_Code[f_Code->m_Index - 1];
					Code* f_OpCode = m_vec_Code[f_Code->m_Index];
					Code* f_RtCode = m_vec_Code[f_Code->m_Index + 1];

					Operator* f_Operator = new Operator(f_OpCode->m_MxName, f_OpCode->m_VarType);

					f_Operator->m_Operator = f_OpCode->m_VarType;

					std::string f_StrCode = f_LtCode->m_MxName;
					std::string f_StrCodeName = f_LtCode->m_Name[0]->m_MxName;

					if(f_StrCode.compare("Sys") == 0)
						{
						if(f_StrCodeName.compare("Pos") == 0)
							{
							f_Operator->resultHand = m_System->Pos;
							m_vec_Variable.push_back(m_System->Pos);

							f_Operator->leftHand = m_System->Pos;
							m_vec_Variable.push_back(m_System->Pos);

#if 0
							f_Operator->rightHand = new Variable(f_RtCode->m_Name[0]->m_MxName, f_RtCode->m_Code, f_RtCode->m_Type[0]->m_VarType);
#endif
							f_Operator->rightHand = f_RtCode->m_Number[0]->m_MxVar;
							m_vec_Variable.push_back(f_Operator->rightHand);
							}
						if(f_StrCodeName.compare("Posx") == 0)
							{
							f_Operator->resultHand = m_System->Posx;
							m_vec_Variable.push_back(m_System->Posx);

							f_Operator->leftHand = m_System->Posx;
							m_vec_Variable.push_back(m_System->Posx);

#if 0
							f_Operator->rightHand = new Variable(f_RtCode->m_Name[0]->m_MxName, f_RtCode->m_Code, f_RtCode->m_Type[0]->m_VarType);
#endif
							f_Operator->rightHand = f_RtCode->m_Number[0]->m_MxVar;
							m_vec_Variable.push_back(f_Operator->rightHand);
							}
						if(f_StrCodeName.compare("Posy") == 0)
							{
							f_Operator->resultHand = m_System->Posy;
							m_vec_Variable.push_back(m_System->Posy);

							f_Operator->leftHand = m_System->Posy;
							m_vec_Variable.push_back(m_System->Posy);

#if 0
							f_Operator->rightHand = new Variable(f_RtCode->m_Name[0]->m_MxName, f_RtCode->m_Code, f_RtCode->m_Type[0]->m_VarType);
#endif
							f_Operator->rightHand = f_RtCode->m_Number[0]->m_MxVar;
							m_vec_Variable.push_back(f_Operator->rightHand);
							}
						if(f_StrCodeName.compare("Posz") == 0)
							{
							f_Operator->resultHand = m_System->Posz;
							m_vec_Variable.push_back(m_System->Posz);

							f_Operator->leftHand = m_System->Posz;
							m_vec_Variable.push_back(m_System->Posz);

#if 0
							f_Operator->rightHand = new Variable(f_RtCode->m_Name[0]->m_MxName, f_RtCode->m_Code, f_RtCode->m_Type[0]->m_VarType);
#endif
							f_Operator->rightHand = f_RtCode->m_Number[0]->m_MxVar;
							m_vec_Variable.push_back(f_Operator->rightHand);
							}
						else if(f_StrCodeName.compare("Color") == 0)
							{
							f_Operator->resultHand = m_System->Color;
							m_vec_Variable.push_back(m_System->Color);

							f_Operator->leftHand = m_System->Color;
							m_vec_Variable.push_back(m_System->Color);

#if 0
							f_Operator->rightHand = new Variable(f_RtCode->m_Name[0]->m_MxName, f_RtCode->m_Code, f_RtCode->m_Type[0]->m_VarType);
#endif
							f_Operator->rightHand = f_RtCode->m_MxVar;
							m_vec_Variable.push_back(f_Operator->rightHand);
							}
						}
					else if(f_StrCode.compare("Var") == 0)
						{
						f_Operator->resultHand = new Variable(f_LtCode->m_Name[0]->m_MxName, f_LtCode->m_Code, f_LtCode->m_Type[0]->m_VarType);
						m_vec_Variable.push_back(f_Operator->resultHand);

						f_Operator->leftHand = new Variable(f_LtCode->m_Name[0]->m_MxName, f_LtCode->m_Code, f_LtCode->m_Type[0]->m_VarType);
						m_vec_Variable.push_back(f_Operator->leftHand);

						f_Operator->rightHand = f_RtCode->m_MxVar;
						m_vec_Variable.push_back(f_Operator->rightHand);
						}
					else
						{
#ifdef ECOIN_SECURE
						throw;
#endif
						}

					m_vec_Operator.push_back(f_Operator);
					}break;

				case MuCode_Override:
					{ /////////////
					 /// Pause ///
					/////////////
					}break;
				}
			}

		f_Count++;
		}
	}

CodeLine::~CodeLine()
	{
	m_vec_Code.clear();
	m_vec_Variable.clear();
	m_vec_Operator.clear();
	}

int CodeLine::acSearch_CodeType(uint f_TYPE)
	{
	for(uint f_CT = 0; f_CT < m_vec_Code.size(); f_CT++)
		{
		Code* f_Code = m_vec_Code[f_CT];

		if(f_Code->m_Code == f_TYPE)
			{
			return f_CT;
			}
		}

	return -5;
	}

bool CodeLine::ac_Execute(void)
{
	  //////////////////////
	 //////////////
	// Setup
	//
	//
	uint f_VarSize = m_vec_Code.size();

	if(f_VarSize == 0 ||
	   f_VarSize == 1)
		{
#ifdef ECOIN_SECURE
#if 0
			throw;
#else
			return false;
#endif
#endif
		}

	 //////////////////
	// Operator
	//
	if(m_CodeLine == MuLine_Assign_Opr)
		{
		if(m_vec_Operator.size() > 0)
			{
			uint f_OpSize = m_vec_Operator.size();

			uint f_Count = 0;
			while(f_Count < f_OpSize)
				{
				Operator* f_Operator = m_vec_Operator[f_Count];
				f_Operator->ac_Execute();

				f_Count++;
				}
			} /////////////
		else
			{
#ifdef ECOIN_SECURE
			throw;
#endif
			}
		}    // Assign
	else if(m_CodeLine == MuLine_Assign)
		{
		if(f_VarSize == 0 ||
		   f_VarSize == 1)
			{
#ifdef ECOIN_SECURE
			throw;
#endif
			}
		else
			{
			if(f_VarSize == 2)
				{
				Variable* f_VarA = m_vec_Variable[0];
				Variable* f_VarB = m_vec_Variable[1];

				*(f_VarA) = *(f_VarB);
				}
			else
				{
				if(f_VarSize == 3)
					{
					Variable* f_VarA = m_vec_Variable[0];
					Variable* f_VarB = m_vec_Variable[1];
					Variable* f_VarC = m_vec_Variable[2];

					*(f_VarA) = *(f_VarB);
					*(f_VarC) = *(f_VarB);
					}
				else
					{
#ifdef ECOIN_SECURE
					throw;
#endif
					}
				}
			}
		}
	else if(m_CodeLine == MuLine_Init_Var)
		{
		if(f_VarSize == 0 ||
		   f_VarSize == 1)
			{
#ifdef ECOIN_SECURE
			throw;
#endif
			}
		else
			{
			if(f_VarSize == 2)
				{
				Variable* f_VarRes = m_vec_Variable[0];
				Variable* f_VarB = m_vec_Variable[1];

				*(f_VarRes) = *(f_VarB);
				}
			else
				{
				if (f_VarSize == 3)
					{
					Variable* f_VarRes = m_vec_Variable[0];
					Variable* f_VarB = m_vec_Variable[1];
					Variable* f_VarC = m_vec_Variable[2];

					*(f_VarRes) = *(f_VarB);
					*(f_VarB) = *(f_VarC);
					}
				else
					{
#ifdef ECOIN_SECURE
					throw;
#endif
					}
				}
			}
		}
	else if(m_CodeLine == MuLine_Function)
		{
		if(m_vec_Function.size() > 0)
			{
			uint f_FnSize = m_vec_Function.size();

			uint f_Count = 0;
			while(f_Count < f_FnSize)
				{
				Function* f_Function = m_vec_Function[f_Count];
				f_Function->acExecute();

				f_Count++;
				}
			} /////////////
		}
	else if(m_CodeLine == MuLine_Loop_Start)
		{

		}
	else if(m_CodeLine == MuLine_Loop_End)
		{

		}

	return true;
}

};