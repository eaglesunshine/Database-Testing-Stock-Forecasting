#pragma once
#include "Accessor.h"

//����buypoint��sellpoint����
class BuyPointer
{
public:
	bool isNum(string str);
	/*
	*���ܣ��ж��Ƿ�Ϊ����
	*���룺��Ҫ�жϵ��ֶ�ֵ
	*�������Ϊ�����ַ����true���������false
	*/

	bool createBSfiled(Accessor &databaseName, string tableName, bool setBuyPoint, bool setSellPoint);
	/*
	*���ܣ���鲢�����ֶ�buypoint��sellpoint
	*���룺���ݿ����������������Ƿ�����buypoint���Ƿ�����sellpoint
	*��������������������ֶξ����true���������false
	*/

	bool setBuySellPoint(Accessor &databaseName, string tableName, string RAfield, string RBfield, float alpha, float beta, bool setBuyPoint = true, bool setSellPoint = true);
	/*
	*���ܣ�����BuyPoint��SellPoint
	*���룺���ݿ������������������ο�ֵ�ֶ���������ο�ֵ�ֶ�����Ȩ��1��Ȩ��2���Ƿ�����buypoint���Ƿ�����sellpoint
	*���������BuyPoint��SellPoint�ɹ������true
	*/
};