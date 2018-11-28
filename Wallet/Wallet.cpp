/*

Wallet.cpp - osirem.com
Copyright OSIREM LTD (C) 2016
www.bitolyl.com/osirem bitcoin-office.com ecn.world

This source is proprietary, and cannot be used, in part or in full without
the express permission of the original author. The original author retain the
rights to use, modify, and/or relicense this code without notice.

*/

#include "pch.h"

#include "Wallet.h"

#include <vector>
#include <random>
#include <climits>
#include <algorithm>
#include <functional>

using namespace Platform;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage;
using namespace Windows::UI::ViewManagement;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

using namespace Windows::UI::Popups;

using namespace Windows::Storage::Streams;
using namespace Windows::Storage::Pickers;
using namespace concurrency;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;
using namespace Windows::Media::Transcoding;
using namespace Windows::Media::MediaProperties;
using namespace Windows::UI::Xaml::Shapes;
using namespace Windows::UI::Xaml;
using namespace Windows::Storage::Provider;

using namespace Windows::System;

namespace ecoin
{

Wallet* g_Wallet;

Wallet::Wallet()
{
	m_Start = false;
	m_OK = false;
	m_Count = 0;
	m_CountYNG = true;
	m_HeshCount = 0;
	m_HeshRCount = 0;
	m_Repeat = false;
	m_ChangeIdx = 0;
	m_RefreshLVL = 2;
	m_RefreshCNT = 0;

#if 1
	printf("   //////////////////////////////////\n");
	printf("  // - ecoin a digital currency - //\n");
	printf(" // WELCOME TO YOUR ECOIN WALLET //\n");
	printf("//////////////////////////////////\n");
#endif

#if 0
	unsigned char rnd[8];

	std::random_device engine;	

	for(;;)
		{
		unsigned int f_x = engine();

		float f_xy = (float)((float)f_x / (float)UINT_MAX);

		printf("Random number %f\n", f_xy);
		}
#endif
}

Wallet::~Wallet()
{
}

bool Wallet::acSpend(int f_egoldspend)
{
	if(f_egoldspend > 0)
		{
		if (m_Bank_egold - f_egoldspend >= 0)
			{
			m_Bank_egold -= f_egoldspend;

			Update();

			return true;
			}
		}

	return false;
}

bool Wallet::Open(void)
{
#ifdef ECOIN_MINER
	g_VisualMinerPaused = OK_PAUSE_STOP;
#endif

	g_Schedular = new Schedular();

	printf("Loading ecoin wallet...Please wait, this may take some time...\n");

#if 1
	bool f_Sane = false;
	while(!f_Sane)
		{
		bool f_Un = ECC_Start();

		if(!f_Un)
			{
			if(ECC_InitSanityCheck())
				{
				f_Sane = true;
				}
			else
				{
				ECC_Stop();
				}
			}
		else
			{
			ECC_Stop();
			}
		}

	printf("Sane = %i\n", f_Sane);
#endif

	m_RefreshLVL = 2;

	if(m_Start)
		{ //////////////
		 // Start
		//  Wallet
		Platform::String^ f_StringBuf = StrUsingStr(Prepare_Start());

#if 1
		FileSavePicker^ savePicker = ref new FileSavePicker();
		savePicker->SuggestedStartLocation = PickerLocationId::DocumentsLibrary;

		auto geoExtensions = ref new Platform::Collections::Vector<String^>();
		geoExtensions->Append(".eco");
		savePicker->FileTypeChoices->Insert("ecoin Wallet", geoExtensions);
		savePicker->SuggestedFileName = "ecoinWallet";

		Concurrency::create_task(savePicker->PickSaveFileAsync()).then([this, f_StringBuf](StorageFile^ file)
			{
			if(file != nullptr)
				{
#else
		Concurrency::create_task(KnownFolders::DocumentsLibrary->CreateFileAsync("ecoinWallet.eco", CreationCollisionOption::ReplaceExisting)).then([this, f_StringBuf](StorageFile^ file)
			{
			if(file != nullptr)
				{
#endif
			
					// Prevent updates to the remote version of the file until we finish making changes and call CompleteUpdatesAsync.
				CachedFileManager::DeferUpdates(file);
					// write to file
					/////////
				// Path
				m_File = file;

				Concurrency::create_task(FileIO::WriteTextAsync(m_File, f_StringBuf)).then([this]()
					{
						// Let Windows know that we're finished changing the file so the other app can update the remote version of the file.
						// Completing updates may require Windows to ask for user input.
					//Concurrency::create_task(CachedFileManager::CompleteUpdatesAsync(m_File)).then([this](FileUpdateStatus status)
					//	{
					//	if(status == FileUpdateStatus::Complete)
					//		{
							m_OK = true;
							m_Repeat = false;

							  ////////////
							 // Hesh
							//  File
					/*		char f_Buffer[256];
							FillChar(f_Buffer, 256);
							_itoa_s(g_HESHSAVEID, f_Buffer, 10);
							std::string f_HeshSAVEID = f_Buffer;

							Platform::String^ f_HeshFileName = "ecoinHesh" + StrUsingStr(f_HeshSAVEID) + ".hesh";

#ifdef ECOIN_MINER
							FileSavePicker^ savePicker = ref new FileSavePicker();
							savePicker->SuggestedStartLocation = PickerLocationId::DocumentsLibrary;

							auto geoExtensions = ref new Platform::Collections::Vector<String^>();
							geoExtensions->Append(".hesh");
							savePicker->FileTypeChoices->Insert("Save ecoin Hesh Data File", geoExtensions);
							savePicker->SuggestedFileName = "ecoinHesh1";

							Concurrency::create_task(savePicker->PickSaveFileAsync()).then([this](StorageFile^ file)
								{
								if(file != nullptr)
									{
									m_HeshFile = file;*/
									g_VisualMinerPaused = OK_PAUSE_GO;
									/*}
								});
#endif
							}
						else
							{
							m_OK = true;
							m_Repeat = true;
							}
						});*/
					});
				}
			else
				{
				m_Repeat = true;
				}
			});
		}
	else
		{
#if 1
		auto picker = ref new FileOpenPicker();
		picker->CommitButtonText = "Open ecoin Wallet";
		picker->ViewMode = PickerViewMode::List;
		picker->SuggestedStartLocation = PickerLocationId::DocumentsLibrary;
		picker->FileTypeFilter->Append(".eco");	// Required 

		Windows::Storage::StorageFolder ^ localFolder = Windows::Storage::ApplicationData::Current->LocalFolder;

		task< StorageFile^ > getFileTask(picker->PickSingleFileAsync());

		getFileTask.then([=](StorageFile^ storageFile)
			{
#else
		Concurrency::create_task(KnownFolders::DocumentsLibrary->GetFileAsync("ecoinWallet.eco")).then([this](StorageFile^ storageFile)
			{
#endif
			if(storageFile != nullptr)
				{ /////////
				 // File
				//  Path
				m_File = storageFile;

				Windows::Storage::StorageFolder ^ localFolder = Windows::Storage::ApplicationData::Current->LocalFolder;

				Platform::String ^ destfilename = storageFile->Name + ".copy";

				auto option = Windows::Storage::NameCollisionOption::ReplaceExisting;
				Concurrency::create_task(storageFile->CopyAsync(localFolder, destfilename, option)).then([=](StorageFile^ sampleFileCopy)
					{
					std::string localPath = to_string(sampleFileCopy->Path);

					int* f_Health = new int();
					*(f_Health) = 0;
					int f_WalletVersion = 0;

					FILE* f_Char = NULL;
					fopen_s(&f_Char, localPath.data(), "r");
					if(f_Char != NULL)
						{
						g_WALLETBANK = acBufreadValueDouble(f_Char, f_Health);
						g_HESHSAVEID = acBufreadValueInteger(f_Char, f_Health);
						m_Bank_egold = acBufreadValueInteger(f_Char, f_Health);

						if(*(f_Health) == 0)
							{
							f_WalletVersion = 1;

							m_ChangeIdx = acBufreadValueInteger(f_Char, f_Health);
							int f_KeyNom = acBufreadValueInteger(f_Char, f_Health);

							if(f_KeyNom >= 5)
								{
								f_WalletVersion = 2;
								}

							if(f_KeyNom > 1)
								{
								int f_prefix = acBufreadValueInteger(f_Char);
								std::string f_keypass = acBufreadValueString(f_Char);
								std::string f_pubkey = acBufreadValueString(f_Char);
								std::string f_owner = acBufreadValueString(f_Char);

								unsigned char *f_p = new unsigned char[8];
								ag_hex2bin(f_p, f_keypass.c_str(), f_keypass.size());

								CKey* key = new CKey();

								for(int f_lm = 0; f_lm < 8; f_lm++)
									{
									key->keydata[f_lm] = f_p[f_lm];
									}

								key->m_prefix = f_prefix;
								key->m_owner = f_owner;
								key->m_pubkey = f_pubkey;
								m_Bank_Adr = f_owner;

								g_Wallet->m_vec_Key.push_back(key);
								}
							else if(f_KeyNom == 1) //early version
								{
								std::string f_keypass = acBufreadValueString(f_Char);
								std::string f_pubkey = acBufreadValueString(f_Char);
								std::string f_owner = acBufreadValueString(f_Char);

								unsigned char *f_p = new unsigned char[8];
								ag_hex2bin(f_p, f_keypass.c_str(), f_keypass.size());

								CKey* key = new CKey();

								for(int f_lm = 0; f_lm < 8; f_lm++)
									{
									key->keydata[f_lm] = f_p[f_lm];
									}

								key->m_prefix = 1;
								key->m_owner = f_owner;
								key->m_pubkey = f_pubkey;
								m_Bank_Adr = f_owner;

								g_Wallet->m_vec_Key.push_back(key);
								}
						
							for(int f_m = 1; f_m < f_KeyNom; f_m++)
								{
								int f_prefix = acBufreadValueInteger(f_Char);
								std::string f_keypass = acBufreadValueString(f_Char);
								std::string f_pubkey = acBufreadValueString(f_Char);
								std::string f_owner = acBufreadValueString(f_Char);

								unsigned char *f_p = new unsigned char[8];
								ag_hex2bin(f_p, f_keypass.c_str(), f_keypass.size());

								CKey* key = new CKey();

								for(int f_lm = 0; f_lm < 8; f_lm++)
									{
									key->keydata[f_lm] = f_p[f_lm];
									}

								key->m_prefix = f_prefix;
								key->m_owner = f_owner;
								key->m_pubkey = f_pubkey;

								g_Wallet->m_vec_Key.push_back(key);
								}
							}

							//////////////////////////////
						   // ecoin Wallet
						  // check
						 // upgrade
						// save
						switch(f_WalletVersion)
							{
							case 0:	//empty broken
								{
								m_Bank_ecoin = 0.0f;
								g_WALLETBANK = 0.0f;
								g_HESHSAVEID = 0;
								m_Bank_egold = 5000;

								CKey* f_1key = ag_GenerateAddress(1);

								m_vec_Key.push_back(f_1key);

								m_Bank_Adr = f_1key->m_owner;
								}
							case 1: //version 1
								{
								m_ChangeIdx = 0;

								CKey* f_key = ag_GenerateAddress(2);

								m_vec_Key.push_back(f_key);

								f_key = ag_GenerateAddress(2);

								m_vec_Key.push_back(f_key);

								f_key = ag_GenerateAddress(2);

								m_vec_Key.push_back(f_key);

								f_key = ag_GenerateAddress(2);

								m_vec_Key.push_back(f_key);
								};
							case 2:
								{
								m_Bank_ecoin = g_WALLETBANK;
								m_OK = true;
								m_Repeat = false;

								printf("Wallet fully loaded OK updating...\n");

								fclose(f_Char);

								Update();
								}break;
							}

#ifdef ECOIN_MINER
						  ////////////
						 // Hesh
						//  File
#if 0
						char f_Buffer[256];
						FillChar(f_Buffer, 256);
						_itoa_s(g_HESHSAVEID, f_Buffer, 10);
						std::string f_HeshSAVEID = f_Buffer;

						Platform::String^ f_HeshFileName = "ecoinHesh" + StrUsingStr(f_HeshSAVEID) + ".hesh";
#endif

						FileSavePicker^ savePicker = ref new FileSavePicker();
						savePicker->SuggestedStartLocation = PickerLocationId::DocumentsLibrary;

						auto geoExtensions = ref new Platform::Collections::Vector<String^>();
						geoExtensions->Append(".hesh");
						savePicker->FileTypeChoices->Insert("Save ecoin Hesh Data File", geoExtensions);
						savePicker->SuggestedFileName = "ecoinHesh1";

						Concurrency::create_task(savePicker->PickSaveFileAsync()).then([this](StorageFile^ file)
							{
							if(file != nullptr)
								{
								m_HeshFile = file;

								g_HESHSAVEID++;
								g_VisualMinerPaused = OK_PAUSE_GO;
								}
							});
#endif
						}
					else
						{
						//fail
						m_OK = false;
						m_Repeat = true;
						}
					});
				}
			else
				{
				m_Repeat = true;
				}
			});
		}

	return m_OK;
}

std::string Wallet::Prepare(void)
{
	std::string f_Result;
	f_Result.clear();
	acBufwriteValueDouble(&f_Result, g_WALLETBANK);
	acBufwriteValueInteger(&f_Result, g_HESHSAVEID);
	acBufwriteValueInteger(&f_Result, m_Bank_egold);
	acBufwriteValueInteger(&f_Result, m_ChangeIdx);
	acBufwriteValueInteger(&f_Result, m_vec_Key.size());

	for(int f_m = 0; f_m < m_vec_Key.size(); f_m++)
		{
		f_Result = f_Result + ag_Key_Serialize(m_vec_Key[f_m]);
		}

	m_Bank_ecoin = g_WALLETBANK;

	return f_Result;
}

std::string ag_Key_Serialize(CKey* f_key)
{
	  /////////
	 // Pass
	//
	std::string f_Result;
	f_Result.clear();

	char *lx2 = (char*)malloc(f_key->keydata.size() * 2 * sizeof(char));

	int i2;
	for (i2 = 0; i2 < f_key->keydata.size(); i2++)
		{
		sprintf(lx2 + (i2 * 2) , "%02x", f_key->keydata[i2]);
		}

	std::string f_OutputKey2(lx2);
	std::string f_OutputKey2v3;

	f_OutputKey2v3.clear();
	for(int f_g = 0; f_g < 16; f_g++)
		{
		f_OutputKey2v3.push_back(f_OutputKey2[f_g]);
		}

	acBufwriteValueInteger(&f_Result, f_key->m_prefix);

	f_Result = f_Result + f_OutputKey2v3;
	f_Result = f_Result + "::::";

	std::string f_PUBKEY = f_key->m_pubkey;
	f_Result = f_Result + f_PUBKEY + "::::";

	std::string f_ADR = f_key->m_owner;
	f_Result = f_Result + f_ADR + "::::";

	return f_Result;
}

std::string Wallet::Prepare_Start(void)
{
	std::string f_Result;
	f_Result.clear();

	acBufwriteValueDouble(&f_Result, 0.0f);
	acBufwriteValueInteger(&f_Result, 0);
	acBufwriteValueInteger(&f_Result, 5000);
	acBufwriteValueInteger(&f_Result, 0);
	acBufwriteValueInteger(&f_Result, 1);

	CKey* f_1key = ag_GenerateAddress(1);

	m_vec_Key.push_back(f_1key);

	std::string f_ResultKey = ag_Key_Serialize(f_1key);
	f_Result = f_Result + f_ResultKey;

	CKey* f_key = ag_GenerateAddress(2);

	m_vec_Key.push_back(f_key);

	f_ResultKey.clear();
	f_ResultKey = ag_Key_Serialize(f_key);
	f_Result = f_Result + f_ResultKey;

	f_key = ag_GenerateAddress(2);

	m_vec_Key.push_back(f_key);

	f_ResultKey.clear();
	f_ResultKey = ag_Key_Serialize(f_key);
	f_Result = f_Result + f_ResultKey;

	f_key = ag_GenerateAddress(2);

	m_vec_Key.push_back(f_key);

	f_ResultKey.clear();
	f_ResultKey = ag_Key_Serialize(f_key);
	f_Result = f_Result + f_ResultKey;

	f_key = ag_GenerateAddress(2);

	m_vec_Key.push_back(f_key);

	f_ResultKey.clear();
	f_ResultKey = ag_Key_Serialize(f_key);
	f_Result = f_Result + f_ResultKey;

	m_Bank_ecoin = 0.0f;
	m_Bank_egold = 5000;
	m_Bank_Adr = f_1key->m_owner;

	return f_Result;
}

#ifdef ECOIN_MINER
void Wallet::UpdateHesh(bool f_Update)
{
	m_Count++;

	if((m_File != nullptr) && (m_Count >= 150))
		{	
		if(m_Count >= 160 || f_Update == true || m_CountYNG == true)
			{
			m_CountYNG = false;

			Platform::String^ f_StringBuf = StrUsingStr(Prepare());

			// Prevent updates to the remote version of the file until we finish making changes and call CompleteUpdatesAsync.
			CachedFileManager::DeferUpdates(m_File);
			// write to file
			/////////
			Concurrency::create_task(FileIO::WriteTextAsync(m_File, f_StringBuf)).then([this]()
				{
				// Let Windows know that we're finished changing the file so the other app can update the remote version of the file.
				// Completing updates may require Windows to ask for user input.
				Concurrency::create_task(CachedFileManager::CompleteUpdatesAsync(m_File)).then([this](FileUpdateStatus status)
					{
					if (status == FileUpdateStatus::Complete)
						{
						//Do nothing
						}
					else
						{
						//Do nothing
						}
					});
				});
			}
		}

	m_HeshCount++;
	m_HeshRCount++;
	if(m_HeshCount >= 20)
		{
		m_HeshCount = 0;
		std::string f_StringHESH;
		f_StringHESH.clear();
		Cube::CubeHESH* f_Hesh;

		for(int f_FI = m_HeshRCount - 20; f_FI < m_HeshRCount; f_FI++)
			{
			f_Hesh = Cube::g_HESH[f_FI];
			f_Hesh->acSerialise(&f_StringHESH);
			}

		Platform::String^ f_StringBuf3 = StrUsingStr(f_StringHESH);

		if(m_HeshFile != nullptr)
			{
			// Prevent updates to the remote version of the file until we finish making changes and call CompleteUpdatesAsync.
			CachedFileManager::DeferUpdates(m_HeshFile);
			// write to file

			Concurrency::create_task(FileIO::AppendTextAsync(m_HeshFile, f_StringBuf3)).then([this]()
				{
				// Let Windows know that we're finished changing the file so the other app can update the remote version of the file.
				// Completing updates may require Windows to ask for user input.
				Concurrency::create_task(CachedFileManager::CompleteUpdatesAsync(m_HeshFile)).then([this](FileUpdateStatus status)
					{
					if(status == FileUpdateStatus::Complete)
						{
						//Do nothing
						}
					else
						{
						//Do nothing
						}
					});
				});
			}
		else
			{
			throw;
			}
		}
}
#endif

void Wallet::Update(void)
{
	if(m_File != nullptr)
		{	
		Platform::String^ f_StringBuf = StrUsingStr(Prepare());

		if(g_FileReady == true)
			{
			// Prevent updates to the remote version of the file until we finish making changes and call CompleteUpdatesAsync.
			CachedFileManager::DeferUpdates(m_File);
			// write to file
			/////////
			Concurrency::create_task(FileIO::WriteTextAsync(m_File, f_StringBuf)).then([this]()
				{
				// Let Windows know that we're finished changing the file so the other app can update the remote version of the file.
				// Completing updates may require Windows to ask for user input.
				Concurrency::create_task(CachedFileManager::CompleteUpdatesAsync(m_File)).then([this](FileUpdateStatus status)
					{
					if (status == FileUpdateStatus::Complete)
						{
						//Do nothing
						}
					else
						{
						//Do nothing
						}
					});
				});
			}
		}
}

double Wallet::GetBalance(bool f_InDollars, int f_UI)
{
	if(f_InDollars)
		{
		return m_Bank_ecoin * GetEValue(f_UI);
		}

	if(m_RefreshLVL > 0)
		{
		m_Bank_ecoin = 0.0;

		if(m_RefreshLVL > 1)
			{
			//RefreshLVL == 2 rerun the verification
			}

		for(int f_I = 0; f_I < m_vec_Key.size(); f_I++)
			{
			char* f_balquery = new char[150];
			sprintf(f_balquery, "SELECT amt FROM address WHERE owner = '%s'", m_vec_Key[f_I]->m_owner.c_str());
			g_SafeL->acSelectCommand(f_balquery, true, false);

			if(g_Entries[ag_GetEntry(g_Entries.size() - 1)].size())
				{
				m_Bank_ecoin += atof(g_Entries[ag_GetEntry(g_Entries.size() - 1)][0][0].c_str());
				}
			}

		m_RefreshLVL = 0;
		}
	else
		{
		m_RefreshCNT++;
		if(m_RefreshCNT > 5)
			{
			m_RefreshCNT = 0;
			m_RefreshLVL = 2;
			}
		}

	return m_Bank_ecoin;
}

double Wallet::GetEValue(bool f_UI)
{
	float f_Circulationamt = 0.0f;

	if(m_RefreshEValueLVL > 0)
		{
		if(m_RefreshEValueLVL > 1)
			{
			char* f_circulationquery = new char[150];
			sprintf(f_circulationquery, "SELECT amt FROM address");
			g_SafeL->acSelectCommand(f_circulationquery, true, false);
			int f_CCount = g_Entries[ag_GetEntry(g_Entries.size() - 1)].size();

			for(int f_Int = 0; f_Int < f_CCount; f_Int++)
				{
				float f_amt = atof(g_Entries[ag_GetEntry(g_Entries.size() - 1)][f_Int][0].c_str());

				f_Circulationamt += f_amt;
				}

			m_Circulationamt = f_Circulationamt;
			}

		char* f_coinquery = new char[150];
		sprintf(f_coinquery, "SELECT price, tx, unit FROM coin WHERE assetofficeid = %i", 3);
		g_SafeL->acSelectCommand(f_coinquery, true, true);

		if(g_Entries[ag_GetEntry(g_Entries.size() - 1)].size())
			{
			g_EVALUE = atof(g_Entries[ag_GetEntry(g_Entries.size() - 1)][0][0].c_str());
			m_Unit = atof(g_Entries[ag_GetEntry(g_Entries.size() - 1)][0][2].c_str());
			m_TX = atof(g_Entries[ag_GetEntry(g_Entries.size() - 1)][0][1].c_str());
			m_Price = g_EVALUE;

			if(g_TX < m_TX)
				{
				g_TX = m_TX;
				}
			}
		else //insert coin record - first time
			{
			char* g_syncvalue[4];
			char* g_syncfield[4];

			for(int f_s = 0; f_s < 4; f_s++)
				{
				g_syncvalue[f_s] = (char*)malloc(sizeof(char) * 256);
				g_syncfield[f_s] = (char*)malloc(sizeof(char) * 256);
				}

			sprintf(g_syncfield[0], "assetofficeid");
			sprintf(g_syncfield[1], "price");
			sprintf(g_syncfield[2], "tx");
			sprintf(g_syncfield[3], "unit");

			sprintf(g_syncvalue[0], "%i", 3);
			sprintf(g_syncvalue[1], "%f", 0.0f);
			sprintf(g_syncvalue[2], "%f", 5.09f);
			sprintf(g_syncvalue[3], "%f", 5.09f);

			int f_SyncUniqueID = g_SafeL->Insert("coin", (const char **)g_syncfield, (const char **)g_syncvalue, 4, true, false);

			g_EVALUE = 0.0f;
			m_Unit = 5.09f;
			m_TX = 5.09f;
			m_Price = g_EVALUE;
			}

		  //////////////
		 //START COIN//
		//
		if(g_EVALUE <= 0.0f)
			{
			float f_StorageWealth = 50.0f;
			m_TX = 5.09;
			m_Price = (f_StorageWealth + m_TX) / f_Circulationamt;
			g_EVALUE = m_Price;
			}

		   ////////////////////
		  // Equation
		 // Storage Wealth
		// Value/Worth
		if(g_TX != m_TX)
			{
			m_TX = g_TX;
			m_Price = ((g_EVALUE * f_Circulationamt) + m_TX) / f_Circulationamt;
			g_EVALUE = m_Price;

			char* f_coinupvalue[3];
			char* f_coinupfield[3];
			char* f_coinupwhere[1];

			f_coinupvalue[0] = (char*)malloc(sizeof(char) * 1024);
			f_coinupfield[0] = (char*)malloc(sizeof(char) * 1024);
			f_coinupvalue[1] = (char*)malloc(sizeof(char) * 1024);
			f_coinupfield[1] = (char*)malloc(sizeof(char) * 1024);
			f_coinupvalue[2] = (char*)malloc(sizeof(char) * 1024);
			f_coinupfield[2] = (char*)malloc(sizeof(char) * 1024);
			f_coinupwhere[0] = (char*)malloc(sizeof(char) * 1024);

			sprintf(f_coinupfield[0], "price");
			sprintf(f_coinupfield[1], "tx");
			sprintf(f_coinupfield[2], "unit");
			sprintf(f_coinupvalue[0], "%f", m_Price);
			sprintf(f_coinupvalue[1], "%f", m_TX);
			sprintf(f_coinupvalue[2], "%f", m_Unit);
			sprintf(f_coinupwhere[0], "assetofficeid = %i", 3);

			g_SafeL->Update("coin", (const char **)f_coinupfield, (const char **)f_coinupvalue, 3, f_coinupwhere[0], true, false);
			g_TX = m_TX;
			}
		}
	else
		{
		m_RefreshEValueCNT--;
		if(m_RefreshEValueCNT <= 0)
			{
			m_RefreshEValueCNT = 50;
			m_RefreshEValueLVL = 2;
			}
		}

	return m_Price;
}

CKey* Wallet::GetNextChangeKey(void)
{
	CKey* f_Key = m_vec_Key[1];

	int f_counter = m_ChangeIdx;
	m_ChangeIdx++;

	for(int f_helly = 1; f_helly < m_vec_Key.size(); f_helly++)
		{
		if(m_vec_Key[f_helly]->m_prefix == 2)
			{
			f_counter--;
			}

		if(f_counter < 0)
			{
			f_Key = m_vec_Key[f_helly];
			}
		}

	return f_Key;
}

bool Wallet::acFindChangeAdr(std::string f_Adr)
{
	for(int f_helly = 0; f_helly < m_vec_Key.size(); f_helly++)
		{
		if(m_vec_Key[f_helly]->m_prefix == 2)
			{
			if(m_vec_Key[f_helly]->m_owner.compare(f_Adr) == 0)
				{
				return true;
				}
			}
		}

	return false;
}

int Wallet::GetGold(void)
{
	return m_Bank_egold;
}

void Wallet::SetGold(int f_eGold)
{
	m_Bank_egold += f_eGold;

	Update();
}

std::string Wallet::GetAdr(void)
{
	if(m_Bank_Adr.length() > 0)
		{
		return m_Bank_Adr;
		}
	else
		{
		return "3zhELmtXkGDpA1ZfuHRzprE2eZzA";
		}
}

};