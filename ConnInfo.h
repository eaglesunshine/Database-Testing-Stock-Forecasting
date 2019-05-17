#pragma once
#include <iostream>
using namespace std;

namespace dbaccessor
{
	/*
	* @brief 数据库配置接口
	*/
	struct ConnInfo
	{
		string provider;//ODBC提供者名
		string dataBase;//数据库名
		string ip;//主机地址
		int port;//端口号
		string passWord;//密码
		string userName;//用户名
		unsigned int connectTimeout;//连接超时时间
		unsigned int tryTime;
		/*
		* @brief 构造函数
		*/
		ConnInfo() :port(0), connectTimeout(30), tryTime(1) {}

		/*
		* @brief 构造函数2
		* @param _provider:ODBC提供者名
		*        _ip: 主机地址
		*        _userName:用户名
		*        _passWord:密码
		*        _dataBase:数据库名称
		*/
		ConnInfo(string  _provider, string _ip, string _userName, string _passWord, string _dataBase) :
			provider(_provider), ip(_ip), userName(_userName), passWord(_passWord), dataBase(_dataBase),
			port(0), connectTimeout(30), tryTime(1)
		{
		}
	};
}