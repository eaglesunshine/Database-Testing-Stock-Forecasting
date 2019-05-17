#pragma once
#ifndef _DECONVOLUTION_H_
#define _DECONVOLUTION_H_
#include <vector>
#include "Accessor.h"
#include "MySQLAccessor.h"
#include "SQLServerAccessor.h"
using namespace std;
using namespace dbaccessor;

class DBConvolution
{

	Accessor &accessor;//���ݿ�������

public:
	/*
	* @brief ���캯��
	* @param acc ���ݿ�������
	*/
	DBConvolution(Accessor &acc) :accessor(acc) {}

	/*
	* @brief ��ָ�������ѯ����
	* @param tableName ����
			 data ��������
	* @return �Ƿ�ɹ�
	*/
	bool getData(const string & tableName, Data & data);

	/*
	* @brief ģ������ Ŀǰ֧��int float double����
			 �������
	* @param data ���ݼ�
			 patternVector �����
			 fieldName �����������
			 updateData ���µ�����map
	* @return �Ƿ�ɹ�
	*/
	template<class KeyType>
	bool calConv(Data &data, vector<KeyType> &patternVector, const string  &fieldName, map<string, string> &updateData);

	/*
	* @brief ��������MySQL��sql���
	* @param tableName ����
			 resultFieldName �������
			 updateData �������ݼ�
			 updateSql sql��伯
	*/
	void buildMySQLUpdateSql(const string  &tableName, const string  & resultFieldName, map<string, string> &updateData, vector<string> &updateSql);

	/*
	* @brief ��������SQLServer��sql���
	* @param tableName ����
			 resultFieldName �������
			 updateData �������ݼ�
			 updateSql sql��伯
	*/
	void buildSQLServerUpdateSql(const string  &tableName, const string  & resultFieldName, map<string, string> &updateData, vector<string> &updateSql);

	/*
	* @brief ģ������ Ŀǰ֧��int float double����
			 �������
	* @param patternVector �����
			 tableName ����
			 fieldName �����������
			 resultFieldName �������
	* @return �Ƿ�ɹ�
	*/
	template<class KeyType>
	bool conv(vector<KeyType> patternVector, const string  &tableName, const string  &fieldName, const string  & resultFieldName);
};

template<class KeyType>
bool DBConvolution::conv(vector<KeyType> patternVector, const string & tableName, const string & fieldName, const string & resultFieldName)
{
	KeyType type;

	if (typeid(type).name() == typeid((int)0).name() ||
		typeid(type).name() == typeid((double)0).name() ||
		typeid(type).name() == typeid((float)0).name())
	{
		Data data;
		//string sttt=typeid(accessor).name();
		getData(tableName, data);
		map<string, string> updateData;
		vector<string> updateSql;
		calConv(data, patternVector, fieldName, updateData);
		if (!strcmp(typeid(accessor).name(), "class SQLServerAccessor"))
			buildSQLServerUpdateSql(tableName, resultFieldName, updateData, updateSql);
		else if (!strcmp(typeid(accessor).name(), "class MySQLAccessor"))
			buildMySQLUpdateSql(tableName, resultFieldName, updateData, updateSql);
		if (updateSql.size() > 0)
			if (accessor.transExecute(updateSql))
				return true;
	}
	return false;
}

template<class KeyType>
bool DBConvolution::calConv(Data & data, vector<KeyType> &patternVector, const string & fieldName, map<string, string>& updateData)
{
	if (data.size() > 0 && patternVector.size() > 0)
	{
		int vecSize = (int)patternVector.size();
		int dataSize = (int)data.size();
		stringstream strstream;
		KeyType sum = 0;
		KeyType value = 0;

		for (int dataCusor = 0; dataCusor < dataSize; ++dataCusor)
		{
			sum = 0;
			for (int patternCusor = 0; patternCusor < vecSize; ++patternCusor)
			{
				strstream.str("");
				strstream.clear();
				if (dataCusor - (vecSize - 1) + patternCusor < 0)
				{
					strstream << data[0][fieldName];
				}
				else
				{
					strstream << data[dataCusor - vecSize + 1 + patternCusor][fieldName];
				}
				strstream >> value;
				sum += value * patternVector[patternCusor];
			}
			strstream.str("");
			strstream.clear();
			strstream << sum;
			updateData[data[dataCusor]["Date"]] = strstream.str();
		}
		return true;
	}
	else
		return false;
}

#endif // !_DECONVOLUTION_H_