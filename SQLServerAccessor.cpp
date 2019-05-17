#include "SQLServerAccessor.h"


string SQLServerAccessor::buildConnStr(ConnInfo & info)
{
	//DSN=sqlserver;UID=sa;PWD=519519519;APP=Microsoft® Windows® Operating System;
	//WSID=LYS-PC;Network=DBMSSOCN;Address=172.31.73.118
	ostringstream osStr;
	osStr << "DSN=" << info.provider << ';';

	if (info.port != 0)
	{
		osStr << "Address=" << info.ip << ',' << info.port << ';';
	}
	else
		osStr << "Address=" << info.ip << ';';

	osStr << "Database=" << info.dataBase << ';';
	osStr << "uid=" << info.userName << ';';
	osStr << "pwd=" << info.passWord << ';';
	return osStr.str();
}

SQLServerAccessor::SQLServerAccessor(ConnInfo conninfo) :Accessor(conninfo)
{
	connString = buildConnStr(connInfo);
}

SQLServerAccessor::SQLServerAccessor(string filePath) : Accessor(filePath)
{
	connString = buildConnStr(connInfo);
}

bool SQLServerAccessor::findTable(const string &tableName)
{
	return Accessor::findTable("select 1 from  sysobjects where  id = object_id('" + tableName + "')");
}

bool SQLServerAccessor::modifyTableName(const string &old_tableName, const string &new_tableName)
{
	return execute("sp_rename '" + old_tableName + "'  , '" + new_tableName + "'") == -1 ? true : false;
}

bool SQLServerAccessor::addTableKey(const string & tableName, Table_Key & tableParam)
{
	bool result = false;
	ostringstream osStr;
	vector<string> sql;

	auto endIter = tableParam.end();
	for (auto iter = tableParam.begin(); iter != endIter; )
	{
		osStr.str("");
		osStr.clear();
		osStr << "alter table [" << tableName << "] add ";
		osStr << " [" << iter->first << "] " << iter->second.first << ' ' << iter->second.second;

		sql.push_back(osStr.str());
		++iter;
	}
	transExecute(sql)  ? result = true : result = false;
	return result;

}

bool SQLServerAccessor::deleteTable(const string & tableName)
{
	return execute("drop table [" + tableName + "]") == -1 ? true : false;
}

int SQLServerAccessor::buildTable(const string & sql)
{
	string tableName = getTableNameFromSql(sql);
	int result = -1;//发生异常

	if (!findTable(tableName))
	{

		if (execute(sql) == -1 ? true : false)
			result = 0;
		else
			result = 2;//未知错误

	}
	else
		result = 1;//表已存在
	return result;
}

int SQLServerAccessor::buildTable(const string &tableName, Table_Key &tableParam)
{
	//"create table test111 (num int not null primary key)"
	//typedef map<PARAM_TYPE,pair<IsNotNull, IsPrimaryKey>>  Table_Param;
	int result = -1;//发生异常

	if (!findTable(tableName))
	{
		ostringstream osStr;
		osStr << "create table [" << tableName << "] ( ";
		auto endIter = tableParam.end();

		for (auto iter = tableParam.begin(); iter != endIter; )
		{
			osStr << " [" << iter->first << "] " << iter->second.first << ' ' << iter->second.second << ' ';
			++iter;

			if (iter != endIter)
				osStr << ", ";

		}
		osStr << " )";


		if (execute(osStr.str()) == -1 ? true : false)
			result = 0;//成功
		else
			result = 3;//未知错误

	}
	else
		result = 1;//表已存在

	return result;

}

bool SQLServerAccessor::transInsertNRecord_Data2(const string & tableName, Data & data)
{
	return Accessor::transInsertNRecord_Data("select top 1 * from [" + tableName + "]", data);
}

bool SQLServerAccessor::transInsertNRecord_Data(const string &tableName, Data &data)
{
	vector<string> sql;
	ostringstream osStr, valueStr;
	for (int i = 0; i < data.size(); ++i)
	{
		osStr.str("");
		osStr.clear();
		valueStr.str("");
		valueStr.clear();
		osStr << "insert into [" << tableName << "] (";
		valueStr << " values (";
		Record record = data[i].getRecord();
		auto endit = record.end();
		for (auto it = record.begin(); it != endit; )
		{
			osStr << "[" << it->first << "]";
			valueStr << "'" << it->second << "'";
			++it;
			if (it != endit)
			{
				osStr << ",";
				valueStr << ",";
			}

		}
		valueStr << ")";
		osStr << ") ";
		sql.push_back(osStr.str() + valueStr.str());
	}
	if (sql.size() > 0)
		return Accessor::transExecute(sql);
	return false;

}

bool SQLServerAccessor::transDeleteNRecord_Data(const string &tableName, Data &data)
{
	vector<string> sql;
	ostringstream osStr;
	for (int i = 0; i < data.size(); ++i)
	{
		osStr.str("");
		osStr.clear();
		osStr << "delete from [" << tableName << "] where ";
		//valueStr << " value('";
		Record record = data[i].getRecord();
		auto endit = record.end();
		for (auto it = record.begin(); it != endit; )
		{
			osStr << "( [" << it->first << "]= ";
			osStr << "'" << it->second << "')";
			++it;
			if (it != endit)
			{
				osStr << " and ";
			}



		}
		sql.push_back(osStr.str());
	}
	if (sql.size() > 0)
		return Accessor::transExecute(sql);
	return false;

}



