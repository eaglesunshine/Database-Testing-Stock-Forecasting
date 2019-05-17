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
	static const int keyTypeTryCount = 5;//csv创建表时读取几条来猜测数据类型
protected:
	/*
	* @brief 判断数据是否DateTime类型
	* @param str 尝试转换DateTime的字符串
	* @return 是否符合
	*/
	static bool isDateTime(const string str);

	/*
	* @brief 判断数据是否Date类型
	* @param str 尝试转换Date的字符串
	* @return 是否符合
	*/
	static bool isDate(const string str);

	/*
	* @brief 判断数据是否Double类型
	* @param str 尝试转换Double的字符串
	* @return 是否符合
	*/
	static bool isDouble(const string str);

	/*
	* @brief 判断数据是否int类型
	* @param str 尝试转换int的字符串
	* @return 是否符合
	*/
	static bool isInt(const string str);

	/*
	* @brief 判断数据是何种类型方法1------对比全部类型
	* @param str 尝试转换的字符串
	* @return 符合类型
	*/
	static string isWhatType(string value);

	/*
	* @brief 判断数据是何种类型方法2----------对比指定类型
	* @param str 尝试转换的字符串
			 type 尝试转换的类型
	* @return 是否符合类型
	*/
	static bool isWhatType(string value, string type);

	/*
	* @brief 获取此数据精确到小数点第几位
	* @param value 为double类型的字符串
	* @return 精确度
	*/
	static int numOfPrecision(const string &value);

	/*
	* @brief 寻找容器里匹配次数最多的类型
	* @param matchTime 为匹配每种数据类型的次数
	* @return 数据匹配类型
	*/
	static string maxMatchKey(map<string, int> matchTime);

	/*
	* @brief 检查Data是否符合数据类型，不符合则删除，避免插入错误
	* @param tableKey 表配置，里面保存了各个字段的数据类型
	* @return 返回检查出来的错误数
	*/
	static int checkData(Accessor::Table_Key &tableKey, dbaccessor::Data &data);

	/*
	* @brief 把CSV导入到Data类里
	* @param filePath CSV文件的地址
			 data 要保存到的data类
	* @return 是否成功
	*/
	static bool loadToData(const string filePath, dbaccessor::Data &data);

	/*
	* @brief 创建表
	* @param acc 操作数据库的类句柄
			 tableName 创建的表名
			 tableKey 表配置
	* @return 是否创建成功
	*/
	static bool createTable(Accessor &acc, const string &tableName, Accessor::Table_Key tableKey);

	/*
	* @brief 将data导入到数据库
	* @param acc 操作数据库的类句柄
			 tableName 创建的表名
			 data 数据集
	* @return 是否执行成功
	*/
	static bool importToDataBase(Accessor &acc, const string tableName, dbaccessor::Data &data);

	/*
	* @brief 从CSV文件名获取表名
	* @param fileName CSV文件名
	* @return 表名
	*/
	static string getTableNameFromFileName(const string fileName);

	/*
	* @brief 从CSV地址获取CSV文件名
	* @param fileName CSV文件地址
	* @return CSV文件名
	*/
	static string getFileNameFromFilePath(const string filePath);

	/*
	* @brief 尝试从data里猜测字段类型
	* @param data 数据集
	* @return 表配置
	*/
	static Accessor::Table_Key tryBuildTableConfig(dbaccessor::Data &data);

	/*
	* @brief 将表导出为CSV文件
	* @param tableName 表名
	* @return 是否执行成功
	*/
	static bool exportToCSV(const string &tableName, Data &data);

	/*
	* @brief 导出字段名至CSV文件
	* @param outFile CSV文件的输出流
			 data 数据集
	* @return 是否导出成功
	*/
	static bool exportKeyName(ofstream &outFile, dbaccessor::Data &data);

	/*
	* @brief 导出字段值至CSV文件
	* @param outFile CSV文件的输出流
			 data 数据集
	* @return 是否导出成功
	*/
	static bool exportKeyValue(ofstream &outFile, dbaccessor::Data &data);

	/*
	* @brief 创建CSV文件
	* @param outFile CSV文件的输出流
			 tableName 表名
	* @return 是否创建成功
	*/
	static bool createCSVFile(ofstream &outFile, const string &tableName);

	/*
	* @brief 判断文件是否存在
	* @param filePath 文件地址
	* @return 是否存在
	*/
	static bool isFileExist(const string &filePath);
public:

	/*
	* @brief 将CSV文件导入数据库
	* @param acc 操作数据库的类句柄
			 filePath CSV文件地址
	* @return 是否导入成功
	*/
	static bool importCSV(Accessor &acc, const string filePath);

	/*
	* @brief 将数据库指定的表导出为CSV文件
	* @param acc 操作数据库的类句柄
			 tableName 表名
	* @return 是否导出成功
	*/
	static bool exportCSV(Accessor &acc, const string tableName);

};

#endif // !_CSVLOADER_H_