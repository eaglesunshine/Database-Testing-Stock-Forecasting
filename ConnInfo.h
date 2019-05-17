#pragma once
#include <iostream>
using namespace std;

namespace dbaccessor
{
	/*
	* @brief ���ݿ����ýӿ�
	*/
	struct ConnInfo
	{
		string provider;//ODBC�ṩ����
		string dataBase;//���ݿ���
		string ip;//������ַ
		int port;//�˿ں�
		string passWord;//����
		string userName;//�û���
		unsigned int connectTimeout;//���ӳ�ʱʱ��
		unsigned int tryTime;
		/*
		* @brief ���캯��
		*/
		ConnInfo() :port(0), connectTimeout(30), tryTime(1) {}

		/*
		* @brief ���캯��2
		* @param _provider:ODBC�ṩ����
		*        _ip: ������ַ
		*        _userName:�û���
		*        _passWord:����
		*        _dataBase:���ݿ�����
		*/
		ConnInfo(string  _provider, string _ip, string _userName, string _passWord, string _dataBase) :
			provider(_provider), ip(_ip), userName(_userName), passWord(_passWord), dataBase(_dataBase),
			port(0), connectTimeout(30), tryTime(1)
		{
		}
	};
}