#pragma once
#ifndef _ACCESSOR_H_
#define _ACCESSOR_H_

#include "Accessor_Data.h"
#include "ConfigFile.h"
#include "ConnInfo.h"
#include <stdlib.h>
#include <string.h>
#include <sstream>
using namespace std;
#import "c:\Program Files\Common Files\System\ado\msado60.tlb" no_namespace rename("EOF","adoEOF")
//#define DEBUG

namespace dbaccessor
{


	/*
	* @brief Accessor数据库操作类
	*/
	class Accessor
	{
	private:
		int tryTime = 3;
		bool autoReleaseConn = true;//手动释放连接状态标志，false为关闭自动释放，true为自动释放
		void init();
	protected:
		typedef string KeyName;//键名
		typedef string Key_Type;//键类型
		typedef string Key_Other;//键其他属性
		typedef string Key_Value;//键值
	public:

		typedef map<KeyName, pair<Key_Type, Key_Other>>  Table_Key;//数据表配置

		typedef map<string, string> Record;//记录集合
	protected:



		_ConnectionPtr pConnection;//连接指针
		_RecordsetPtr	pRecordset;//数据集指针
		ConnInfo connInfo;//数据库配置接口
		string connString = "";

		/*
		* @brief 提取sql语句中的table名
		* @param sql 建表sql语句
		* @return 表名
		*/
		string getTableNameFromSql(const string &sql);


		/*
		* @brief 获取连接状态
		* @return 连接状态 0断开，1连接，其余查看源代码
		*/
		int getConnectState();

		/*
		* @brief ado执行sql语句(返回数据集)
		* @param sql sql语句
		* @return 是否执行成功
		*/
		bool open(const string &sql);



		/*
		* @brief ADO内部连接数据库实现
		* @param sql 连接字符串
		* @return 是否链接成功
		*/
		bool _connect(const string &sql);



		/*
		* @brief 打印错误代码以及描述
		* @param e 异常类
		*/
		void showErrorMesg(const _com_error &e);

		/*
		* @brief 把查询出来的数据集保存至Data类
		* @param data Data数据类
		*/
		void getRecord(Data &data);

		/*
		* @brief 设置自动释放连接
		*/
		void setAutoReleaseConn();

		/*
		* @brief 设置手动释放连接
		*/
		void resetAutoReleaseConn();

		/*
		* @brief 查询结果集的数目
		*/
		int getRecordCount();

		/*
* @brief 断开数据库连接
*/
		void disConnect();
		//* @brief 把查询出来的数据集保存至Data类
		//* @param data Data数据类
		//*/
		//KeyName tryGetPrimaryKey(map<Key_Value, Record> &data);

				/*
* @brief 通过连接字符串连接数据库
* @param sql 连接字符串
* @return 是否连接成功
*/
		bool connect(const string & sql);

		/*
		* @brief 通过连接配置连接数据库
		* @param conn连接配置
		* @return 是否连接成功
		*/
		bool connect();


		virtual string buildConnStr(ConnInfo &info);

	public:

		/*
* @brief 查询表是否存在
* @param tableName 查询sql语句
* @return 是否存在
*/
		virtual bool findTable(const string & sql);

		/*
		* @brief 批量插入记录事务方法
		* @param tableName 插入记录的表名
				  data 插入的数据集
		* @return 影响条数
		*/
		virtual bool transInsertNRecord_Data(const string &tableName, Data &data);

		/*
	* @brief 建表方法
	* @param sql 建表sql语句
	* @return 是否建表成功,-1为失败;0为成功;1为表已存在;2为未知错误,需查看错误代码
	*/
		virtual int buildTable(const string &sql);

		virtual int buildTable(const string &tableName, Table_Key &tableParam) { return -1; }

		/*
		* @brief 插入记录方法
		* @param sql 插入sql语句
		* @return 影响条数
		*/
		virtual int insertRecord(const string &sql);


		/*
		* @brief Accessor构造函数
		*/
		Accessor(ConnInfo info);

		Accessor(string filePath);

		/*
		* @brief Accessor析构函数
		*/
		~Accessor();

		/*
		* @brief 批量更新表记录事务方法1
		* @param tableName 更新记录的表名
				  data 更新的数据集
		* @return 影响条数
		*/
		//virtual int transUpdateNRecord(const string &sql, map<Key_Value, Record> &data);

		/*
		* @brief 添加表字段
		* @param old_tableName 需要修改的表名
				  new_tableName 重命名后的表名
		* @return 是否修改成功
		*/
		virtual bool addTableKey(const string &tableName, Table_Key &tableParam);

		/*
		* @brief 事务执行
		* @param sql sql语句集
		* @return 执行是否成功
		*/
		bool transExecute(const vector<string> &sql);


		/*
		* @brief 更新表记录
		* @param sql 更新记录sql语句
		* @return 影响条数
		*/
		virtual int updateRecord(const string &sql);

		/*
		* @brief 删除记录
		* @param sql 删除记录sql语句
		* @return 影响条数
		*/
		virtual  int deleteRecord(const string &sql);

		/*
		* @brief 查询表记录
		* @param sql 查询sql语句
				  data 结果保存至data集合
		*/
		virtual bool selectRecord(const string &sql, Data &data);

		/*
		* @brief 删除表
		* @param tableName 需要删除的表名
		* @return 是否删除成功
		*/
		virtual bool deleteTable(const string &tableName);

		/*
		* @brief ado执行sql语句(仅执行)
		* @param sql sql语句
		* @return 是否执行成功
		*/
		int execute(const string &sql);

		virtual bool transDeleteNRecord_Data(const string &tableName, Data &data);

	};

}

#endif // !_ACCESSOR_H_