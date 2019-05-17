#pragma once
#ifndef _CONFIGFILE_H_
#define _CONFIGFILE_H_
//#include "Accessor.h"
#include "ConnInfo.h"
#include <iostream>
#include <Windows.h>
#include <tchar.h>
using namespace dbaccessor;
using namespace std;

class ConfigFile
{
public:
	static void readConfig(const string &filePath, ConnInfo &info)
	{
		// 得到exe执行路径.  
		TCHAR tcExePath[MAX_PATH] = { 0 };
		::GetModuleFileName(NULL, tcExePath, MAX_PATH);
		// 设置ini路径到exe同一目录下  
	//_tcsrchr() 反向搜索获得最后一个'\\'的位置，并返回该位置的指针  
		TCHAR *pFind = _tcsrchr(tcExePath, '\\');
		if (pFind == NULL)
		{
			return;
		}
		*pFind = '\0';

		string szIniPath(tcExePath);
		szIniPath += "\\";
		szIniPath += filePath;
		LPTSTR lpPath = new char[MAX_PATH];
		LPTSTR cstr = new char[MAX_PATH];
		strcpy_s(lpPath, MAX_PATH, szIniPath.c_str());
		//cout << lpPath << endl;
		GetPrivateProfileString("Config", "provider", "", cstr, MAX_PATH, lpPath);
		info.provider = cstr;
		GetPrivateProfileString("Config", "ip", "", cstr, MAX_PATH, lpPath);
		info.ip = cstr;
		GetPrivateProfileString("Config", "password", "", cstr, MAX_PATH, lpPath);
		info.passWord = cstr;
		GetPrivateProfileString("Config", "username", "", cstr, MAX_PATH, lpPath);
		info.userName = cstr;
		GetPrivateProfileString("Config", "database", "", cstr, MAX_PATH, lpPath);
		info.dataBase = cstr;
		GetPrivateProfileString("Config", "type", "", cstr, MAX_PATH, lpPath);
		string type = cstr;
		if (type == "mysql")
			info.port = GetPrivateProfileInt("Config", "port", 3306, lpPath);
		else if (type == "sqlserver")
			info.port = GetPrivateProfileInt("Config", "port", 1433, lpPath);
		info.connectTimeout = GetPrivateProfileInt("Config", "connecttimeout", 30, lpPath);
		info.tryTime = GetPrivateProfileInt("Config", "trytime", 1, lpPath);
		delete[] lpPath;
		//return info;
	}
};














#endif // !_CONFIGFILE_H_

