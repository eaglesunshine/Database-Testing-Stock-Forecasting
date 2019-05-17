#include "Accessor.h"

class ProfitComp
{
public:
	bool isNum(string str);
	/*
	*���ܣ��жϴ������ļ�����������Ƿ�Ϊ����
	*���룺��Ҫ�жϵ��ֶ�ֵ
	*�������Ϊ�����ַ����true���������false
	*/

	bool createWSfiled(Accessor &databaseName, string tableName, bool setWealth, bool setStock);
	/*
	*���ܣ���鲢�ж��Ƿ񴴽��ֶ�wealth��stock
	*���룺���ݿ����������������Ƿ�����wealth���Ƿ�����stock
	*����������������������ֶξ����true���������false
	*/

	float incomeComp(Accessor &databaseName, float intialWealth, string tableName, string closeField, string buypointField, string sellpointField);
	/*
	*���ܣ������ֶ�wealth��stock��ֵ������ͳ������Income
	*���룺���ݿ�����������ʼ���𣬱�����Close�ֶ�����buypoint�ֶ�����sellpoint�ֶ���
	*������������õ���ͳ������Income
	*/

	float verifyIncomeComp(Accessor &databaseName, float intialWealth, string tableName);
	/*
	*���ܣ�������֤����verifyIncome
	*���룺���ݿ�����������ʼ���𣬱���
	*������������õ�����֤����verifyIncome
	*/

	void printIncome(float Income, float verifyIncome, float error, int precision);
	/*
	*���ܣ���ӡ�����������
	*���룺ͳ�����棬��֤���棬��С��λ��
	*�������ӡ���
	*/

	//��������
	float compProfit(Accessor &databaseName, float intialWealth, string tableName, string buypointField, string sellpointField, string closeField, bool setWealth = true, bool setStock = true, int precision = 6);
	/*
	*���ܣ���������
	*���룺���ݿ�����������ʼ���𣬱�����buypoint�ֶ�����sellpoint�ֶ�����Close�ֶ������Ƿ�����wealth���Ƿ�����stock
	*��������ͳ������ֵIncome
	*/

};