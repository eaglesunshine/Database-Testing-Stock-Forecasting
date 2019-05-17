#pragma once
#ifndef _ACCESSOR_DATARECORD_H_
#define _ACCESSOR_DATARECORD_H_


#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <algorithm>
using namespace std;
namespace dbaccessor
{
	class Data_Record
	{
	public:
		/**
		 * @brief 构造函数.
		 * @param record  一条记录数据集
		 */
		Data_Record(map<string, string> &record);

		/**
		 * @brief 打印集合里所有元素
		 */
		void printALL();

		/**
		 * @brief 获取数据，s一般是指数据表的某个字段名
		 * @param s 要获取的字段
		 * @return  符合查询条件的记录的s字段名
		 */
		const string& operator[](const string &s);

		/**
		 * @brief 获取集合
		 * @return 返回集合
		 */
		map<string, string>& getRecord();

		/**
		 * @brief 获取集合大小
		 * @return 返回集合大小
		 */
		int getSize();
	protected:
		map<string, string> &_record;
	};
}

#endif // !_ACCESSOR_DATARECORD_H_