#include "DBConvolution.h"

bool DBConvolution::getData(const string & tableName, Data & data)
{

	if (!strcmp(typeid(accessor).name(), "class SQLServerAccessor"))
	{
		accessor.selectRecord("select * from [" + tableName + "] order by [date] asc", data);
		return true;
	}

	if (!strcmp(typeid(accessor).name(), "class MySQLAccessor"))
	{
		accessor.selectRecord("select * from `" + tableName + "` order by `date` asc", data);
		return true;
	}

	return false;
}


void DBConvolution::buildMySQLUpdateSql(const string  &tableName, const string  & resultFieldName, map<string, string>& updateData, vector<string>& updateSql)
{

	for (auto it = updateData.begin(); it != updateData.end(); ++it)
		updateSql.push_back("UPDATE `" + tableName + "` SET `" + resultFieldName + "` = '" + it->second +
			"' WHERE `date` = '" + it->first + "'");

}

void DBConvolution::buildSQLServerUpdateSql(const string  &tableName, const string  & resultFieldName, map<string, string>& updateData, vector<string>& updateSql)
{

	for (auto it = updateData.begin(); it != updateData.end(); ++it)
		updateSql.push_back("UPDATE [" + tableName + "] SET [" + resultFieldName + "] = '" + it->second +
			"' WHERE [date] = '" + it->first + "'");

}
