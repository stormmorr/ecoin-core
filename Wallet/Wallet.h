/*

Wallet.h - osirem.com
Copyright OSIREM LTD (C) 2016
www.bitolyl.com/osirem bitcoin-office.com ecn.world

This source is proprietary, and cannot be used, in part or in full without
the express permission of the original author. The original author retain the
rights to use, modify, and/or relicense this code without notice.

*/

#ifndef WALLET_H
#define WALLET_H

#include "ecoinPage.xaml.h"

namespace ecoin
{

class Wallet
	{
	public:
		Wallet();
		~Wallet();

		void Update(void);
		void UpdateHesh(bool f_Update = false);

		bool Open(void);
		std::string Prepare(void);
		std::string Prepare_Start(void);
		double GetBalance(bool f_InDollars, int f_UI = false);
		int GetGold(void);
		std::string GetAdr(void);
		void SetGold(int f_eGold);

		bool acFindChangeAdr(std::string f_Adr);

		bool acSpend(int f_egoldspend);

		ecoinPage^ m_ecoinPage;

		void acPushKey(CKey* f_Key)
			{
			m_vec_Key.push_back(f_Key);
			}

		int acGetKeySize(void)
			{
			return m_vec_Key.size();
			}

		bool acGetKey(std::string f_Owner, CKey* f_Key)
			{
			bool f_Found = false;
			int f_Index = 0;
			for(int f_Helly = 0; f_Helly < m_vec_Key.size(); f_Helly++)
				{
				if(m_vec_Key[f_Helly]->m_owner.size() >= 27)
					{
					if(m_vec_Key[f_Helly]->m_owner.compare(f_Owner) == 0)
						{
						f_Found = true;
						f_Index = f_Helly;
						}
					}
				}

			if(f_Found)
				{
				f_Key = m_vec_Key[f_Index];
				return true;
				}
			else
				{
				return false;
				}
			}

		CKey* acGetKey(int f_Index)
			{
			return m_vec_Key[f_Index];
			}

		CKey* GetNextChangeKey(void);

		double GetEValue(bool f_UI = false);

		bool m_Start;

		 ////////
		// OK
		bool m_OK;

		Windows::Storage::StorageFile^ m_File;
		Windows::Storage::StorageFile^ m_HeshFile;

		int m_Count;
		bool m_CountYNG;
		int m_HeshCount;
		int m_HeshRCount;
		bool m_Repeat;
		int m_ChangeIdx;
		int m_RefreshLVL;
		int m_RefreshCNT;
		int m_RefreshEValueCNT;
		int m_RefreshEValueLVL;

	protected:
		double m_Bank_ecoin;
		int m_Bank_egold;
		std::string m_Bank_Adr;

	public:
		std::vector<CKey*> m_vec_Key;
		float m_Price;
		float m_Unit;
		float m_TX;
		float m_Circulationamt;
	};

extern Wallet* g_Wallet;

extern std::string ag_Key_Serialize(CKey* f_key);

};

#endif