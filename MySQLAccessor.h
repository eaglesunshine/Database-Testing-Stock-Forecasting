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
* @brief ��ѯ���Ƿ����
* @param tableName ��Ҫ��ѯ�ı���
* @return �Ƿ����
*/
	virtual bool findTable(const string &tableName);


	virtual bool transDeleteNRecord_Data(const string &tableName, Data &data);

	/*
		* @brief ��������
		* @param old_tableName ��Ҫ�޸ĵı���
				  new_tableName ��������ı���
		* @return �Ƿ��޸ĳɹ�
		*/
	bool modifyTableName(const string &old_tableName, const string &new_tableName);

	/*
		* @brief ɾ����
		* @param tableName ��Ҫɾ���ı���
		* @return �Ƿ�ɾ���ɹ�
		*/
	virtual bool deleteTable(const string &tableName);

	/*
* @brief ����ʱ��
* @param zoneʱ��
* @return �Ƿ����óɹ�
*/
	bool setTimeZone(int zone);

	/*
		* @brief ������1
		* @param sql ����sql���
		* @return �Ƿ񽨱�ɹ�,-1Ϊʧ��;0Ϊ�ɹ�;1Ϊ���Ѵ���;2Ϊδ֪����,��鿴�������
		*/
	virtual int buildTable(const string &sql);

	/*
	* @brief ������2
	* @param tableName ����
			  tableParam ������
	* @return �Ƿ񽨱�ɹ�,-1Ϊʧ��;0Ϊ�ɹ�;1Ϊ���Ѵ���;2Ϊδ֪����,��鿴�������
	*/
	virtual int buildTable(const string &tableName, Table_Key &tableParam);

	/*
	* @brief ���������¼����
	* @param tableName ��Ҫ����ı���
			  data ��������ݼ�
	* @return Ӱ������
	*/
	virtual bool transInsertNRecord_Data(const string &tableName, Data &data);

	/*
* @brief ���������¼����
* @param tableName ��Ҫ����ı���
		  data ��������ݼ�
* @return Ӱ������
*/
	virtual bool transInsertNRecord_Data2(const string &tableName, Data &data);

	/*
		* @brief ��ӱ��ֶ�
		* @param old_tableName ��Ҫ�޸ĵı���
				  new_tableName ��������ı���
		* @return �Ƿ��޸ĳɹ�
		*/
	bool addTableKey(const string &tableName, Table_Key &tableParam);

	/*
* @brief ��ȡʱ��
* @return ʱ���ַ���,��"SYSTEM"Ϊϵͳʱ��;"+08:00"Ϊ������ʱ��
*/
	string getTimeZone();


};

#endif // !_MYSQLACCESSOR_H_