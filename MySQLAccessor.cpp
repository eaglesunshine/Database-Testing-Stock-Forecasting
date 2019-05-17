#include "MySQLAccessor.h"

string MySQLAccessor::buildConnStr(ConnInfo & info)
{
	//DSN=mysql;SERVER=172.31.73.118;UID=admin;
	//PWD=519519519;DATABASE=db_oohomework2;PORT=3306
	ostringstream osStr;
	osStr << "DSN=" << info.provider << ';';
	osStr << "Server=" << info.ip << ';';
	if (info.port != 0)
	{
		osStr << "port=" << info.port << ';';
	}
	osStr << "Database=" << info.dataBase << ';';
	osStr << "uid=" << info.userName << ';';
	osStr << "pwd=" << info.passWord << ';';
	return osStr.str();
	return string();
}

MySQLAccessor::MySQLAccessor(ConnInfo info) :Accessor(info)
{
	connString = buildConnStr(connInfo);
}

MySQLAccessor::MySQLAccessor(string filePath) : Accessor(filePath)
{
	connString = buildConnStr(connInfo);
}

bool MySQLAccessor::findTable(const string &tableName)
{
	return Accessor::findTable("select TABLE_NAME from INFORMATION_SCHEMA.TABLES where TABLE_SCHEMA='"
		+ connInfo.dataBase + "' and TABLE_NAME='" + tableName + "'");
}



bool MySQLAccessor::modifyTableName(const string &old_tableName, const string &new_tableName)
{
	return execute("alter table `" + old_tableName + "`  rename to `" + new_tableName + "`") == 0 ? true : false;
}

bool MySQLAccessor::deleteTable(const string & tableName)
{
	return execute("drop table `" + tableName + "`") == 0 ? true : false;
}

bool MySQLAccessor::setTimeZone(int zone)
{
	string sql;

	if (zone >= -12 && zone < 0)
		sql = "set @@session.time_zone= \'" + to_string(zone) + ":00\'";
	else if (zone >= 0 && zone <= 12)
		sql = "set @@session.time_zone= \'+" + to_string(zone) + ":00\'";
	else
		return false;

	return execute(sql);
}

int MySQLAccessor::buildTable(const string & sql)
{
	string tableName = getTableNameFromSql(sql);
	int result = -1;//发生异常

	if (!findTable(tableName))
	{

		if (execute(sql) == 0 ? true : false)
			result = 0;
		else
			result = 2;//未知错误

	}
	else
		result = 1;//表已存在
	return result;
}

int MySQLAccessor::buildTable(const string &tableName, Table_Key &tableParam)
{
	//"create table test111 (num int not null primary key)"
	//typedef map<PARAM_TYPE,pair<IsNotNull, IsPrimaryKey>>  Table_Param;
	int result = -1;//发生异常

	if (!findTable(tableName))
	{
		ostringstream osStr;
		osStr << "create table `" << tableName << "` ( `";
		auto endIter = tableParam.end();

		for (auto iter = tableParam.begin(); iter != endIter; )
		{
			osStr << iter->first << "` " << iter->second.first << ' ' << iter->second.second << ' ';
			++iter;

			if (iter != endIter)
				osStr << ", `";

		}
		osStr << " )";


		if (execute(osStr.str()) == 0 ? true : false)
			result = 0;
		else
			result = 3;//未知错误

	}
	else
		result = 1;//表已存在

	return result;

}

bool MySQLAccessor::transInsertNRecord_Data2(const string & tableName, Data & data)
{
	return Accessor::transInsertNRecord_Data("select * from `" + tableName + "` limit 0", data);;
}

bool MySQLAccessor::transInsertNRecord_Data(const string & tableName, Data & data)
{
	vector<string> sql;
	ostringstream osStr, valueStr;
	for (int i = 0; i < data.size(); ++i)
	{
		osStr.str("");
		osStr.clear();
		valueStr.str("");
		valueStr.clear();
		osStr << "insert into `" << tableName << "` (";
		valueStr << " values (";
		Record record = data[i].getRecord();
		auto endit = record.end();
		for (auto it = record.begin(); it != endit; )
		{
			osStr << "`" << it->first << "`";
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

bool MySQLAccessor::addTableKey(const string & tableName, Table_Key & tableParam)
{
	bool result = false;
	ostringstream osStr;
	osStr << "alter table `" << tableName << "` add ( ";
	auto endIter = tableParam.end();
	for (auto iter = tableParam.begin(); iter != endIter; )
	{
		osStr << " `" << iter->first << "` " << iter->second.first << ' ' << iter->second.second << ' ';
		++iter;
		if (iter != endIter)
			osStr << ", ";
	}
	osStr << " )";
	execute(osStr.str()) == 0 ? result = true : result = false;
	return result;
}

string MySQLAccessor::getTimeZone()
{
	string sql = "SELECT @@session.time_zone;";
	string result = "";
	resetAutoReleaseConn();

	if (open(sql) == true)
	{
		Data data;
		getRecord(data);

		if (data.size() > 0)
			result = data[0]["@@session.time_zone"];

	}

	setAutoReleaseConn();
	return result;
}

bool MySQLAccessor::transDeleteNRecord_Data(const string &tableName, Data &data)
{
	vector<string> sql;
	ostringstream osStr;
	for (int i = 0; i < data.size(); ++i)
	{
		osStr.str("");
		osStr.clear();
		osStr << "delete from `" << tableName << "` where ";
		//valueStr << " value('";
		Record record = data[i].getRecord();
		auto endit = record.end();
		for (auto it = record.begin(); it != endit; )
		{
			osStr << "( `" << it->first << "`= ";
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

