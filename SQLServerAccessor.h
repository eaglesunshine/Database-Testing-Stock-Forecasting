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
* @brief ��ѯ���Ƿ����
* @param tableName ��Ҫ��ѯ�ı���
* @return �Ƿ����
*/
	virtual bool findTable(const string &tableName);

	/*
		* @brief ��������
		* @param old_tableName ��Ҫ�޸ĵı���
				  new_tableName ��������ı���
		* @return �Ƿ��޸ĳɹ�
		*/
	bool modifyTableName(const string &old_tableName, const string &new_tableName);

	/*
		* @brief ��ӱ��ֶ�
		* @param old_tableName ��Ҫ�޸ĵı���
				  new_tableName ��������ı���
		* @return �Ƿ��޸ĳɹ�
		*/
	bool addTableKey(const string &tableName, Table_Key &tableParam);


	/*
		* @brief ɾ����
		* @param tableName ��Ҫɾ���ı���
		* @return �Ƿ�ɾ���ɹ�
		*/
	virtual bool deleteTable(const string &tableName);

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

	virtual bool transDeleteNRecord_Data(const string &tableName, Data &data);









};

#endif // !_SQLSERVERACCESSOR_H_