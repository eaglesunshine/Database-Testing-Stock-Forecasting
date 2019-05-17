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
	* @brief Accessor���ݿ������
	*/
	class Accessor
	{
	private:
		int tryTime = 3;
		bool autoReleaseConn = true;//�ֶ��ͷ�����״̬��־��falseΪ�ر��Զ��ͷţ�trueΪ�Զ��ͷ�
		void init();
	protected:
		typedef string KeyName;//����
		typedef string Key_Type;//������
		typedef string Key_Other;//����������
		typedef string Key_Value;//��ֵ
	public:

		typedef map<KeyName, pair<Key_Type, Key_Other>>  Table_Key;//���ݱ�����

		typedef map<string, string> Record;//��¼����
	protected:



		_ConnectionPtr pConnection;//����ָ��
		_RecordsetPtr	pRecordset;//���ݼ�ָ��
		ConnInfo connInfo;//���ݿ����ýӿ�
		string connString = "";

		/*
		* @brief ��ȡsql����е�table��
		* @param sql ����sql���
		* @return ����
		*/
		string getTableNameFromSql(const string &sql);


		/*
		* @brief ��ȡ����״̬
		* @return ����״̬ 0�Ͽ���1���ӣ�����鿴Դ����
		*/
		int getConnectState();

		/*
		* @brief adoִ��sql���(�������ݼ�)
		* @param sql sql���
		* @return �Ƿ�ִ�гɹ�
		*/
		bool open(const string &sql);



		/*
		* @brief ADO�ڲ��������ݿ�ʵ��
		* @param sql �����ַ���
		* @return �Ƿ����ӳɹ�
		*/
		bool _connect(const string &sql);



		/*
		* @brief ��ӡ��������Լ�����
		* @param e �쳣��
		*/
		void showErrorMesg(const _com_error &e);

		/*
		* @brief �Ѳ�ѯ���������ݼ�������Data��
		* @param data Data������
		*/
		void getRecord(Data &data);

		/*
		* @brief �����Զ��ͷ�����
		*/
		void setAutoReleaseConn();

		/*
		* @brief �����ֶ��ͷ�����
		*/
		void resetAutoReleaseConn();

		/*
		* @brief ��ѯ���������Ŀ
		*/
		int getRecordCount();

		/*
* @brief �Ͽ����ݿ�����
*/
		void disConnect();
		//* @brief �Ѳ�ѯ���������ݼ�������Data��
		//* @param data Data������
		//*/
		//KeyName tryGetPrimaryKey(map<Key_Value, Record> &data);

				/*
* @brief ͨ�������ַ����������ݿ�
* @param sql �����ַ���
* @return �Ƿ����ӳɹ�
*/
		bool connect(const string & sql);

		/*
		* @brief ͨ�����������������ݿ�
		* @param conn��������
		* @return �Ƿ����ӳɹ�
		*/
		bool connect();


		virtual string buildConnStr(ConnInfo &info);

	public:

		/*
* @brief ��ѯ���Ƿ����
* @param tableName ��ѯsql���
* @return �Ƿ����
*/
		virtual bool findTable(const string & sql);

		/*
		* @brief ���������¼���񷽷�
		* @param tableName �����¼�ı���
				  data ��������ݼ�
		* @return Ӱ������
		*/
		virtual bool transInsertNRecord_Data(const string &tableName, Data &data);

		/*
	* @brief ������
	* @param sql ����sql���
	* @return �Ƿ񽨱�ɹ�,-1Ϊʧ��;0Ϊ�ɹ�;1Ϊ���Ѵ���;2Ϊδ֪����,��鿴�������
	*/
		virtual int buildTable(const string &sql);

		virtual int buildTable(const string &tableName, Table_Key &tableParam) { return -1; }

		/*
		* @brief �����¼����
		* @param sql ����sql���
		* @return Ӱ������
		*/
		virtual int insertRecord(const string &sql);


		/*
		* @brief Accessor���캯��
		*/
		Accessor(ConnInfo info);

		Accessor(string filePath);

		/*
		* @brief Accessor��������
		*/
		~Accessor();

		/*
		* @brief �������±��¼���񷽷�1
		* @param tableName ���¼�¼�ı���
				  data ���µ����ݼ�
		* @return Ӱ������
		*/
		//virtual int transUpdateNRecord(const string &sql, map<Key_Value, Record> &data);

		/*
		* @brief ��ӱ��ֶ�
		* @param old_tableName ��Ҫ�޸ĵı���
				  new_tableName ��������ı���
		* @return �Ƿ��޸ĳɹ�
		*/
		virtual bool addTableKey(const string &tableName, Table_Key &tableParam);

		/*
		* @brief ����ִ��
		* @param sql sql��伯
		* @return ִ���Ƿ�ɹ�
		*/
		bool transExecute(const vector<string> &sql);


		/*
		* @brief ���±��¼
		* @param sql ���¼�¼sql���
		* @return Ӱ������
		*/
		virtual int updateRecord(const string &sql);

		/*
		* @brief ɾ����¼
		* @param sql ɾ����¼sql���
		* @return Ӱ������
		*/
		virtual  int deleteRecord(const string &sql);

		/*
		* @brief ��ѯ���¼
		* @param sql ��ѯsql���
				  data ���������data����
		*/
		virtual bool selectRecord(const string &sql, Data &data);

		/*
		* @brief ɾ����
		* @param tableName ��Ҫɾ���ı���
		* @return �Ƿ�ɾ���ɹ�
		*/
		virtual bool deleteTable(const string &tableName);

		/*
		* @brief adoִ��sql���(��ִ��)
		* @param sql sql���
		* @return �Ƿ�ִ�гɹ�
		*/
		int execute(const string &sql);

		virtual bool transDeleteNRecord_Data(const string &tableName, Data &data);

	};

}

#endif // !_ACCESSOR_H_