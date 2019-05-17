#include "Accessor.h"
#include "CSVLoader.h"
#include "DBConvolution.h"
#include "MySQLAccessor.h"
#include "SQLServerAccessor.h"

using namespace std;
using namespace dbaccessor;

//������
class Test
{
public:
	Data getNRecordData(string fileName);				
	/*
	*���ܣ��������ļ��л�ȡ�����N����¼
	*���룺�����ļ���
	*������洢��N����¼��Data��
	*/

	bool testInsertNRecord(Accessor &databaseName);		
	/*
	*���ܣ���������N����¼�Ƿ�ɹ�
	*���룺���ݿ�������
	*���������ͨ�����true��ʧ�����false
	*/

	bool testDeleteNRecord(Accessor &databaseName);		
	/*
	*���ܣ�����ɾ��N����¼�Ƿ�ɹ�
	*���룺���ݿ�������
	*���������ͨ�����true��ʧ�����false
	*/

	bool testUpdateNRecord(Accessor &databaseName);		
	/*
	*���ܣ����Ը���N����¼�Ƿ�ɹ�
	*���룺���ݿ�������
	*���������ͨ�����true��ʧ�����false
	*/

};

