#pragma once

#ifndef _MYSQLACCESSOR_H_
#define _MYSQLACCESSOR_H_


#include "Accessor.h"
#include "ConnInfo.h"
using namespace dbaccessor;



class MySQLAccessor :public Accessor
{
protected:
	virtual string buildConnStr(ConnInfo &info);
public:

	MySQLAccessor(ConnInfo info);

	MySQLAccessor(string filePath);

	/*
* @brief 查询表是否存在
* @param tableName 需要查询的表名
* @return 是否存在
*/
	virtual bool findTable(const string &tableName);


	virtual bool transDeleteNRecord_Data(const string &tableName, Data &data);

	/*
		* @brief 重命名表
		* @param old_tableName 需要修改的表名
				  new_tableName 重命名后的表名
		* @return 是否修改成功
		*/
	bool modifyTableName(const string &old_tableName, const string &new_tableName);

	/*
		* @brief 删除表
		* @param tableName 需要删除的表名
		* @return 是否删除成功
		*/
	virtual bool deleteTable(const string &tableName);

	/*
* @brief 设置时域
* @param zone时域
* @return 是否设置成功
*/
	bool setTimeZone(int zone);

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

	/*
		* @brief 添加表字段
		* @param old_tableName 需要修改的表名
				  new_tableName 重命名后的表名
		* @return 是否修改成功
		*/
	bool addTableKey(const string &tableName, Table_Key &tableParam);

	/*
* @brief 获取时域
* @return 时域字符串,如"SYSTEM"为系统时域;"+08:00"为东八区时域
*/
	string getTimeZone();


};

#endif // !_MYSQLACCESSOR_H_