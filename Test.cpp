#include "Test.h"
#include<iostream>
#include <stdlib.h>
#include<time.h> 
#include <algorithm>
#include "MySQLAccessor.h"
#include "SQLServerAccessor.h"

#define NUM 5	 //���Եļ�¼����

using namespace std;
using namespace dbaccessor;

//�������ļ��л�ȡ�����N����¼
Data Test::getNRecordData(string fileName)			
{
	Accessor::Record rec;
	Data dataSet;

	//����N����������ҵ�������
	srand((unsigned)time(NULL));
	int number[NUM];
	for (int i = 0; i < NUM; i++) number[i] = rand() % 10000 + 1;	//���ɴ�1��10000������ͬ��NUM����
	sort(number, number + NUM);		//��������

	//��ȡ�����ļ�����ȡN��¼
	ifstream inFile(fileName, ios::in);		// ���ļ�
	string lineStr;
	int num = 0, count = 0;

	while (getline(inFile, lineStr))
	{
		stringstream ss(lineStr);
		string str;
		vector<string> lineArray;
		while (getline(ss, str, ','))	// ���ն��ŷָ�,��ȡһ��
		{
			lineArray.push_back(str);
		}

		if (count == 0)		//������һ��,��Ϊ��һ�����ֶ���
		{
			++count;
			continue;
		}

		if (count == number[num])	//�����N���������Ӧ�ļ�¼�У������dataSet
		{
			vector<string>::iterator iter = lineArray.begin();
			rec["Date"] = *iter;
			rec["Open"] = *(iter + 1);
			rec["High"] = *(iter + 2);
			rec["Low"] = *(iter + 3);
			rec["Close"] = *(iter + 4);
			rec["Adj Close"] = *(iter + 5);
			rec["Volume"] = *(iter + 6);
			dataSet.push(rec);
			++num;
		}
		
		if (num == NUM)	//�Ѿ��õ�N����¼��Ӧ��������
			break;

		++count;
	}

	return dataSet;
}

//��������N����¼�Ƿ�ɹ�
bool Test::testInsertNRecord(Accessor &databaseName)	
{
	//ɾ���ɱ�
	if (databaseName.findTable("BA") == true)
		databaseName.deleteTable("BA");

	//�����ձ�
	Accessor::Table_Key t_param2;
	t_param2["Date"] = make_pair("varchar(22)", "primary key");
	t_param2["Open"] = make_pair("decimal(20,6)", "");
	t_param2["High"] = make_pair("decimal(20,6)", "");
	t_param2["Low"] = make_pair("decimal(20,6)", "");
	t_param2["Close"] = make_pair("decimal(20,6)", "");
	t_param2["Adj Close"] = make_pair("decimal(20,6)", "");
	t_param2["Volume"] = make_pair("int", "");
	databaseName.buildTable("BA", t_param2);	//������BAtable

	//��ȡ�����NUM����¼
	Data dataSetInsert = getNRecordData("BA.csv");		

	//�����¼
	databaseName.transInsertNRecord_Data("BA", dataSetInsert);

	//��ѯ����֤��¼����ɹ�
	Data dataSetSeclect;
	for (int i = 0; i < dataSetInsert.size(); i++)
	{
		string str = dataSetInsert[i]["Date"];
		databaseName.selectRecord("select * from BA where Date='" +str+ "'", dataSetSeclect);	//��ѯ������¼
		if (!(dataSetInsert[i]["Open"] == dataSetSeclect[0]["Open"]
			&& dataSetInsert[i]["High"] == dataSetSeclect[0]["High"]
			&& dataSetInsert[i]["Low"] == dataSetSeclect[0]["Low"]
			&& dataSetInsert[i]["Close"] == dataSetSeclect[0]["Close"]
			&& dataSetInsert[i]["Adj Close"] == dataSetSeclect[0]["Adj Close"]
			&& dataSetInsert[i]["Volume"] == dataSetSeclect[0]["Volume"]))
			return false;	//��֤����ʧ�ܣ���˴β������ʧ��
	}

	return true;	//��֤���ݳɹ�����˴β�����Գɹ�
}

//����ɾ��N����¼�Ƿ�ɹ�
bool Test::testDeleteNRecord(Accessor &databaseName)	
{
	//ɾ���ɱ�
	if(databaseName.findTable("BA")==true)
		databaseName.deleteTable("BA");
	
	//�������ݣ����±�
	CSVLoader::importCSV(databaseName, "BA.csv");

	//��ȡ�����NUM����¼
	Data dataSetDelete = getNRecordData("BA.csv");

	//ɾ����¼
	databaseName.transDeleteNRecord_Data("BA", dataSetDelete);

	//��ѯ����֤��¼ɾ���ɹ�
	Data dataSetSeclect;
	for (int i = 0; i < dataSetDelete.size(); i++)
	{
		string str = dataSetDelete[i]["Date"];
		databaseName.selectRecord("select * from BA where Date='" + str + "'", dataSetSeclect);	//��ѯ������¼
		if (dataSetSeclect.size() > 0)
			return false;	//��ѯ��¼�ɹ���˵����¼δ��ɾ������˴�ɾ������ʧ��
	}

	return true;	//���޼�¼��˵��ȫ��ɾ������˴β�����Գɹ�
}

//���Ը���N����¼�Ƿ�ɹ�
bool Test::testUpdateNRecord(Accessor &databaseName)	
{
	//ɾ���ɱ�
	if (databaseName.findTable("BA") == true)
		databaseName.deleteTable("BA");

	//�������ݣ����±�
	CSVLoader::importCSV(databaseName, "BA.csv");

	//��ȡ�����NUM����¼
	Data dataSetUpdate = getNRecordData("Ba.csv");

	//���¼�¼
	vector<string> sql;
	for (int i = 0; i < dataSetUpdate.size(); i++)
	{
		string str = dataSetUpdate[i]["Date"];
		string sqlLine = "update BA set Volume = '0' where Date = '" + str + "'";
		sql.push_back(sqlLine);
	}
	databaseName.transExecute(sql);

	//��ѯ����֤��¼����ɹ�
	Data dataSetSeclect;
	for (int i = 0; i < dataSetUpdate.size(); i++)
	{
		string str = dataSetUpdate[i]["Date"];
		databaseName.selectRecord("select * from BA where Date='" + str + "'", dataSetSeclect);	//��ѯ������¼
		if ((dataSetUpdate[i]["Volume"] == dataSetSeclect[0]["Volume"]))
			return false;	//�ֶαȶ���ͬ��˵��δ���£���˴θ��²���ʧ��
	}

	databaseName.deleteTable("BA");	 //ɾ�������ĵı��������Ӱ��������������

	return true;	//�ֶαȶԾ���ͬ��˵���ֶξ������£���˴θ��²��Գɹ�
}
