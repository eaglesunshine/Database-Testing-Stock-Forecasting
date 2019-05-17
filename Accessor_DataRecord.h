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
		 * @brief ���캯��.
		 * @param record  һ����¼���ݼ�
		 */
		Data_Record(map<string, string> &record);

		/**
		 * @brief ��ӡ����������Ԫ��
		 */
		void printALL();

		/**
		 * @brief ��ȡ���ݣ�sһ����ָ���ݱ��ĳ���ֶ���
		 * @param s Ҫ��ȡ���ֶ�
		 * @return  ���ϲ�ѯ�����ļ�¼��s�ֶ���
		 */
		const string& operator[](const string &s);

		/**
		 * @brief ��ȡ����
		 * @return ���ؼ���
		 */
		map<string, string>& getRecord();

		/**
		 * @brief ��ȡ���ϴ�С
		 * @return ���ؼ��ϴ�С
		 */
		int getSize();
	protected:
		map<string, string> &_record;
	};
}

#endif // !_ACCESSOR_DATARECORD_H_