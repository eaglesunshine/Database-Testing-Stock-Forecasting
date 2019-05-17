#pragma once
#ifndef _CSVLOADER_H_
#define _CSVLOADER_H_
#include "Accessor_Data.h"
#include "Accessor.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <time.h>
#include <iomanip>

using namespace std;
using namespace dbaccessor;
class CSVLoader
{
private:
	static const int keyTypeTryCount = 5;//csv������ʱ��ȡ�������²���������
protected:
	/*
	* @brief �ж������Ƿ�DateTime����
	* @param str ����ת��DateTime���ַ���
	* @return �Ƿ����
	*/
	static bool isDateTime(const string str);

	/*
	* @brief �ж������Ƿ�Date����
	* @param str ����ת��Date���ַ���
	* @return �Ƿ����
	*/
	static bool isDate(const string str);

	/*
	* @brief �ж������Ƿ�Double����
	* @param str ����ת��Double���ַ���
	* @return �Ƿ����
	*/
	static bool isDouble(const string str);

	/*
	* @brief �ж������Ƿ�int����
	* @param str ����ת��int���ַ���
	* @return �Ƿ����
	*/
	static bool isInt(const string str);

	/*
	* @brief �ж������Ǻ������ͷ���1------�Ա�ȫ������
	* @param str ����ת�����ַ���
	* @return ��������
	*/
	static string isWhatType(string value);

	/*
	* @brief �ж������Ǻ������ͷ���2----------�Ա�ָ������
	* @param str ����ת�����ַ���
			 type ����ת��������
	* @return �Ƿ��������
	*/
	static bool isWhatType(string value, string type);

	/*
	* @brief ��ȡ�����ݾ�ȷ��С����ڼ�λ
	* @param value Ϊdouble���͵��ַ���
	* @return ��ȷ��
	*/
	static int numOfPrecision(const string &value);

	/*
	* @brief Ѱ��������ƥ�������������
	* @param matchTime Ϊƥ��ÿ���������͵Ĵ���
	* @return ����ƥ������
	*/
	static string maxMatchKey(map<string, int> matchTime);

	/*
	* @brief ���Data�Ƿ�����������ͣ���������ɾ��������������
	* @param tableKey �����ã����汣���˸����ֶε���������
	* @return ���ؼ������Ĵ�����
	*/
	static int checkData(Accessor::Table_Key &tableKey, dbaccessor::Data &data);

	/*
	* @brief ��CSV���뵽Data����
	* @param filePath CSV�ļ��ĵ�ַ
			 data Ҫ���浽��data��
	* @return �Ƿ�ɹ�
	*/
	static bool loadToData(const string filePath, dbaccessor::Data &data);

	/*
	* @brief ������
	* @param acc �������ݿ������
			 tableName �����ı���
			 tableKey ������
	* @return �Ƿ񴴽��ɹ�
	*/
	static bool createTable(Accessor &acc, const string &tableName, Accessor::Table_Key tableKey);

	/*
	* @brief ��data���뵽���ݿ�
	* @param acc �������ݿ������
			 tableName �����ı���
			 data ���ݼ�
	* @return �Ƿ�ִ�гɹ�
	*/
	static bool importToDataBase(Accessor &acc, const string tableName, dbaccessor::Data &data);

	/*
	* @brief ��CSV�ļ�����ȡ����
	* @param fileName CSV�ļ���
	* @return ����
	*/
	static string getTableNameFromFileName(const string fileName);

	/*
	* @brief ��CSV��ַ��ȡCSV�ļ���
	* @param fileName CSV�ļ���ַ
	* @return CSV�ļ���
	*/
	static string getFileNameFromFilePath(const string filePath);

	/*
	* @brief ���Դ�data��²��ֶ�����
	* @param data ���ݼ�
	* @return ������
	*/
	static Accessor::Table_Key tryBuildTableConfig(dbaccessor::Data &data);

	/*
	* @brief ������ΪCSV�ļ�
	* @param tableName ����
	* @return �Ƿ�ִ�гɹ�
	*/
	static bool exportToCSV(const string &tableName, Data &data);

	/*
	* @brief �����ֶ�����CSV�ļ�
	* @param outFile CSV�ļ��������
			 data ���ݼ�
	* @return �Ƿ񵼳��ɹ�
	*/
	static bool exportKeyName(ofstream &outFile, dbaccessor::Data &data);

	/*
	* @brief �����ֶ�ֵ��CSV�ļ�
	* @param outFile CSV�ļ��������
			 data ���ݼ�
	* @return �Ƿ񵼳��ɹ�
	*/
	static bool exportKeyValue(ofstream &outFile, dbaccessor::Data &data);

	/*
	* @brief ����CSV�ļ�
	* @param outFile CSV�ļ��������
			 tableName ����
	* @return �Ƿ񴴽��ɹ�
	*/
	static bool createCSVFile(ofstream &outFile, const string &tableName);

	/*
	* @brief �ж��ļ��Ƿ����
	* @param filePath �ļ���ַ
	* @return �Ƿ����
	*/
	static bool isFileExist(const string &filePath);
public:

	/*
	* @brief ��CSV�ļ��������ݿ�
	* @param acc �������ݿ������
			 filePath CSV�ļ���ַ
	* @return �Ƿ���ɹ�
	*/
	static bool importCSV(Accessor &acc, const string filePath);

	/*
	* @brief �����ݿ�ָ���ı���ΪCSV�ļ�
	* @param acc �������ݿ������
			 tableName ����
	* @return �Ƿ񵼳��ɹ�
	*/
	static bool exportCSV(Accessor &acc, const string tableName);

};

#endif // !_CSVLOADER_H_