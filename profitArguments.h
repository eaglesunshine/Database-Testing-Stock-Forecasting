#pragma once
#include <iostream>
#include "Accessor.h"

using namespace std;

//��ProfitSearch��Ա�������ݲ����Ľṹ��
struct ProfitArguments
{
	Accessor &databaseName;
	string tableName;
	string RAfield;
	string RBfield;
	float alpha;
	float beta;
	bool setBuyPoint = true;
	bool setSellPoint = true;

	float intialWealth;
	string buypointField;
	string sellpointField;
	string closeField;
	bool setWealth = true;
	bool setStock = true;
	int precision = 6;
	ProfitArguments(Accessor &acc):databaseName(acc) {}
};