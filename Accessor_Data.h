#pragma once
#ifndef _ACCESSOR_DATA_H_
#define _ACCESSOR_DATA_H_


#include "Accessor_DataRecord.h"
#include <vector>
#include <map>
#include <iostream>
using namespace std;
namespace dbaccessor
{

	class Data
	{
	public:
		/*
		* @brief 保存一条记录
		* @param map 一条记录
		*/
		void push(map<string, string> & map);

		/*
		* @brief 打印所有记录
		*/
		void printAll();

		/*
		* @brief 清空所有记录
		*/
		void clear();

		/*
		* @brief 删除指定下标的向量
		* @param i 下标
		*/
		void erase(int i);

		/*
		* @brief 查询记录条数
		* @return   返回记录条数
		*/
		int size();


		/*@brief 获取某一条记录.
		* @param i  要获取第几条记录
		* @return   Data_Record类型的数据，可以根据字段获取相关信息，
		*/
		Data_Record operator[](size_t i);

	protected:
		vector<map<string, string> > _data;
	};
}

#endif // !_ACCESSOR_DATA_H_