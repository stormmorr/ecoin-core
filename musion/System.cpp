/*

Contract - osirem.com
Copyright OSIREM LTD (C) 2016
www.osirem.com www.qage.org www.geopomp.com

This source is proprietary, and cannot be used, in part or in full without
the express permission of the original author. The original author retain the
rights to use, modify, and/or relicense this code without notice.

*/

#include "pch.h"

#include "System.h"

using namespace ecoin;

namespace ecoin
{

	System::System()
		{
		Pos = new Variable("Pos", MuCode_System, MuVar_Float3);
		Posx = new Variable("Posx", MuCode_System, MuVar_Float);
		Posy = new Variable("Posy", MuCode_System, MuVar_Float);
		Posz = new Variable("Posz", MuCode_System, MuVar_Float);
		Color = new Variable("Color", MuCode_System, MuVar_Float3);
		Matrix1 = new Variable("Matrix1", MuCode_System, MuVar_Float4);
		Matrix2 = new Variable("Matrix2", MuCode_System, MuVar_Float4);
		Matrix3 = new Variable("Matrix3", MuCode_System, MuVar_Float4);
		Matrix4 = new Variable("Matrix4", MuCode_System, MuVar_Float4);
		nonce = new Variable("nonce", MuCode_System, MuVar_Float);
		}

	System::~System()
		{
		delete Pos;
		delete Posx;
		delete Posy;
		delete Posz;
		delete Color;
		delete Matrix1;
		delete Matrix2;
		delete Matrix3;
		delete Matrix4;
		delete nonce;
		}

	Variable* System::av_Var(std::string f_String)
		{
		if(f_String.compare("Pos") == 0)
			{
			return Pos;
			}
		else if (f_String.compare("Posx") == 0)
			{
			return Posx;
			}
		else if (f_String.compare("Posy") == 0)
			{
			return Posy;
			}
		else if(f_String.compare("Posz") == 0)
			{
			return Posz;
			}
		else if(f_String.compare("Color") == 0)
			{
			return Color;
			}
		else if(f_String.compare("Matrix1") == 0)
			{
			return Matrix1;
			}
		else if (f_String.compare("Matrix2") == 0)
			{
			return Matrix2;
			}
		else if(f_String.compare("Matrix3") == 0)
			{
			return Matrix3;
			}
		else if(f_String.compare("Matrix4") == 0)
			{
			return Matrix4;
			}
		else if(f_String.compare("nonce") == 0)
			{
			return nonce;
			}

		return Pos;
		}

};