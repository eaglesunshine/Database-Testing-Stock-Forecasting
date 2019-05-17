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

	Accessor &accessor;//数据库操作句柄

public:
	/*
	* @brief 构造函数
	* @param acc 数据库操作句柄
	*/
	DBConvolution(Accessor &acc) :accessor(acc) {}

	/*
	* @brief 从指定表里查询数据
	* @param tableName 表名
			 data 数据容器
	* @return 是否成功
	*/
	bool getData(const string & tableName, Data & data);

	/*
	* @brief 模板类型 目前支持int float double类型
			 卷积计算
	* @param data 数据集
			 patternVector 卷积核
			 fieldName 卷积向量列名
			 updateData 更新的数据map
	* @return 是否成功
	*/
	template<class KeyType>
	bool calConv(Data &data, vector<KeyType> &patternVector, const string  &fieldName, map<string, string> &updateData);

	/*
	* @brief 创建更新MySQL的sql语句
	* @param tableName 表名
			 resultFieldName 结果列名
			 updateData 更新数据集
			 updateSql sql语句集
	*/
	void buildMySQLUpdateSql(const string  &tableName, const string  & resultFieldName, map<string, string> &updateData, vector<string> &updateSql);

	/*
	* @brief 创建更新SQLServer的sql语句
	* @param tableName 表名
			 resultFieldName 结果列名
			 updateData 更新数据集
			 updateSql sql语句集
	*/
	void buildSQLServerUpdateSql(const string  &tableName, const string  & resultFieldName, map<string, string> &updateData, vector<string> &updateSql);

	/*
	* @brief 模板类型 目前支持int float double类型
			 卷积计算
	* @param patternVector 卷积核
			 tableName 表名
			 fieldName 卷积向量列名
			 resultFieldName 结果列名
	* @return 是否成功
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