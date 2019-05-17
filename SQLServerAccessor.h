#pragma once
#ifndef _SQLSERVERACCESSOR_H_
#define _SQLSERVERACCESSOR_H_


#include "Accessor.h"
using namespace dbaccessor;

class SQLServerAccessor :public Accessor
{
protected:
	virtual string buildConnStr(ConnInfo &info);
public:

	SQLServerAccessor(ConnInfo conninfo);

	SQLServerAccessor(string filePath);

	/*
* @brief 查询表是否存在
* @param tableName 需要查询的表名
* @return 是否存在
*/
	virtual bool findTable(const string &tableName);

	/*
		* @brief 重命名表
		* @param old_tableName 需要修改的表名
				  new_tableName 重命名后的表名
		* @return 是否修改成功
		*/
	bool modifyTableName(const string &old_tableName, const string &new_tableName);

	/*
		* @brief 添加表字段
		* @param old_tableName 需要修改的表名
				  new_tableName 重命名后的表名
		* @return 是否修改成功
		*/
	bool addTableKey(const string &tableName, Table_Key &tableParam);


	/*
		* @brief 删除表
		* @param tableName 需要删除的表名
		* @return 是否删除成功
		*/
	virtual bool deleteTable(const string &tableName);

	/*
		* @brief 建表方法1
		* @param sql 建表sql语句
		* @return 是否建表成功,-1为失败;0为成功;1为表已存在;2为未知错误,需查看错误代码
		*/
	virtual int buildTable(const string &sql);

	/*
	* @brief 建表方法2
	* @param tableName 表名
			  tableParam 表配置
	* @return 是否建表成功,-1为失败;0为成功;1为表已存在;2为未知错误,需查看错误代码
	*/
	virtual int buildTable(const string &tableName, Table_Key &tableParam);



	/*
* @brief 批量插入记录方法
* @param tableName 需要插入的表名
		  data 插入的数据集
* @return 影响条数
*/
	virtual bool transInsertNRecord_Data(const string &tableName, Data &data);

	/*
* @brief 批量插入记录方法
* @param tableName 需要插入的表名
		  data 插入的数据集
* @return 影响条数
*/
	virtual bool transInsertNRecord_Data2(const string &tableName, Data &data);

	virtual bool transDeleteNRecord_Data(const string &tableName, Data &data);









};

#endif // !_SQLSERVERACCESSOR_H_