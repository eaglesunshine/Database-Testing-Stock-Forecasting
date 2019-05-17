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
		* @brief ����һ����¼
		* @param map һ����¼
		*/
		void push(map<string, string> & map);

		/*
		* @brief ��ӡ���м�¼
		*/
		void printAll();

		/*
		* @brief ������м�¼
		*/
		void clear();

		/*
		* @brief ɾ��ָ���±������
		* @param i �±�
		*/
		void erase(int i);

		/*
		* @brief ��ѯ��¼����
		* @return   ���ؼ�¼����
		*/
		int size();


		/*@brief ��ȡĳһ����¼.
		* @param i  Ҫ��ȡ�ڼ�����¼
		* @return   Data_Record���͵����ݣ����Ը����ֶλ�ȡ�����Ϣ��
		*/
		Data_Record operator[](size_t i);

	protected:
		vector<map<string, string> > _data;
	};
}

#endif // !_ACCESSOR_DATA_H_