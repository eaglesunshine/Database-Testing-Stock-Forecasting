#include "Test.h"
#include<iostream>
#include <stdlib.h>
#include<time.h> 
#include <algorithm>
#include "MySQLAccessor.h"
#include "SQLServerAccessor.h"

#define NUM 5	 //测试的记录条数

using namespace std;
using namespace dbaccessor;

//从数据文件中获取随机的N条记录
Data Test::getNRecordData(string fileName)			
{
	Accessor::Record rec;
	Data dataSet;

	//生成N个随机数，且递增排列
	srand((unsigned)time(NULL));
	int number[NUM];
	for (int i = 0; i < NUM; i++) number[i] = rand() % 10000 + 1;	//生成从1到10000互不相同的NUM个数
	sort(number, number + NUM);		//递增排序

	//读取数据文件，获取N记录
	ifstream inFile(fileName, ios::in);		// 读文件
	string lineStr;
	int num = 0, count = 0;

	while (getline(inFile, lineStr))
	{
		stringstream ss(lineStr);
		string str;
		vector<string> lineArray;
		while (getline(ss, str, ','))	// 按照逗号分隔,读取一行
		{
			lineArray.push_back(str);
		}

		if (count == 0)		//跳过第一行,因为第一行是字段名
		{
			++count;
			continue;
		}

		if (count == number[num])	//如果是N个随机数对应的记录行，则存入dataSet
		{
			vector<string>::iterator iter = lineArray.begin();
			rec["Date"] = *iter;
			rec["Open"] = *(iter + 1);
			rec["High"] = *(iter + 2);
			rec["Low"] = *(iter + 3);
			rec["Close"] = *(iter + 4);
			rec["Adj Close"] = *(iter + 5);
			rec["Volume"] = *(iter + 6);
			dataSet.push(rec);
			++num;
		}
		
		if (num == NUM)	//已经得到N条记录，应跳出查找
			break;

		++count;
	}

	return dataSet;
}

//测试增加N条记录是否成功
bool Test::testInsertNRecord(Accessor &databaseName)	
{
	//删除旧表
	if (databaseName.findTable("BA") == true)
		databaseName.deleteTable("BA");

	//创建空表
	Accessor::Table_Key t_param2;
	t_param2["Date"] = make_pair("varchar(22)", "primary key");
	t_param2["Open"] = make_pair("decimal(20,6)", "");
	t_param2["High"] = make_pair("decimal(20,6)", "");
	t_param2["Low"] = make_pair("decimal(20,6)", "");
	t_param2["Close"] = make_pair("decimal(20,6)", "");
	t_param2["Adj Close"] = make_pair("decimal(20,6)", "");
	t_param2["Volume"] = make_pair("int", "");
	databaseName.buildTable("BA", t_param2);	//创建表BAtable

	//获取随机的NUM条记录
	Data dataSetInsert = getNRecordData("BA.csv");		

	//插入记录
	databaseName.transInsertNRecord_Data("BA", dataSetInsert);

	//查询并验证记录插入成功
	Data dataSetSeclect;
	for (int i = 0; i < dataSetInsert.size(); i++)
	{
		string str = dataSetInsert[i]["Date"];
		databaseName.selectRecord("select * from BA where Date='" +str+ "'", dataSetSeclect);	//查询单条记录
		if (!(dataSetInsert[i]["Open"] == dataSetSeclect[0]["Open"]
			&& dataSetInsert[i]["High"] == dataSetSeclect[0]["High"]
			&& dataSetInsert[i]["Low"] == dataSetSeclect[0]["Low"]
			&& dataSetInsert[i]["Close"] == dataSetSeclect[0]["Close"]
			&& dataSetInsert[i]["Adj Close"] == dataSetSeclect[0]["Adj Close"]
			&& dataSetInsert[i]["Volume"] == dataSetSeclect[0]["Volume"]))
			return false;	//验证数据失败，则此次插入测试失败
	}

	return true;	//验证数据成功，则此次插入测试成功
}

//测试删除N条记录是否成功
bool Test::testDeleteNRecord(Accessor &databaseName)	
{
	//删除旧表
	if(databaseName.findTable("BA")==true)
		databaseName.deleteTable("BA");
	
	//导入数据，建新表
	CSVLoader::importCSV(databaseName, "BA.csv");

	//获取随机的NUM条记录
	Data dataSetDelete = getNRecordData("BA.csv");

	//删除记录
	databaseName.transDeleteNRecord_Data("BA", dataSetDelete);

	//查询并验证记录删除成功
	Data dataSetSeclect;
	for (int i = 0; i < dataSetDelete.size(); i++)
	{
		string str = dataSetDelete[i]["Date"];
		databaseName.selectRecord("select * from BA where Date='" + str + "'", dataSetSeclect);	//查询单条记录
		if (dataSetSeclect.size() > 0)
			return false;	//查询记录成功，说明记录未被删除，则此次删除测试失败
	}

	return true;	//查无记录，说明全被删除，则此次插入测试成功
}

//测试更新N条记录是否成功
bool Test::testUpdateNRecord(Accessor &databaseName)	
{
	//删除旧表
	if (databaseName.findTable("BA") == true)
		databaseName.deleteTable("BA");

	//导入数据，建新表
	CSVLoader::importCSV(databaseName, "BA.csv");

	//获取随机的NUM条记录
	Data dataSetUpdate = getNRecordData("Ba.csv");

	//更新记录
	vector<string> sql;
	for (int i = 0; i < dataSetUpdate.size(); i++)
	{
		string str = dataSetUpdate[i]["Date"];
		string sqlLine = "update BA set Volume = '0' where Date = '" + str + "'";
		sql.push_back(sqlLine);
	}
	databaseName.transExecute(sql);

	//查询并验证记录插入成功
	Data dataSetSeclect;
	for (int i = 0; i < dataSetUpdate.size(); i++)
	{
		string str = dataSetUpdate[i]["Date"];
		databaseName.selectRecord("select * from BA where Date='" + str + "'", dataSetSeclect);	//查询单条记录
		if ((dataSetUpdate[i]["Volume"] == dataSetSeclect[0]["Volume"]))
			return false;	//字段比对相同，说明未更新，则此次更新测试失败
	}

	databaseName.deleteTable("BA");	 //删除被更改的表，否则可能影响后面计算收益率

	return true;	//字段比对均不同，说明字段均被更新，则此次更新测试成功
}
