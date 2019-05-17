#include "ProfitComp.h"
#include <iostream>
#include <iomanip>
using namespace::std;

//ģ�庯������string���ͱ���ת��Ϊ���õ���ֵ���� 
template <class Type>
Type stringToNum(const string& str)
{
	istringstream iss(str);
	Type num;
	iss >> num;
	return num;
}

//�жϴ������ļ�����������Ƿ�Ϊ����
bool ProfitComp::isNum(string str)
{
	stringstream sin(str);
	float d;
	char c;
	if (!(sin >> d))
	{
		return false;
	}
	if (sin >> c)
	{
		return false;
	}
	return true;
}

//��鲢�ж��Ƿ񴴽��ֶ�wealth��stock
bool ProfitComp::createWSfiled(Accessor &databaseName, string tableName, bool setWealth, bool setStock)
{
	//����ֶ�wealth��stock�Ƿ����
	Data dataSetWealth, dataSetStock;
	bool isWealthIn = databaseName.selectRecord("select wealth from " + tableName + "", dataSetWealth);
	bool isStockIn = databaseName.selectRecord("select stock from " + tableName + "", dataSetStock);

	//����wealth�ֶ� 
	if (setWealth && !isWealthIn)	//����wealth�ֶ�
	{
		Accessor::Table_Key t_param1;
		t_param1["wealth"] = make_pair("decimal(20,6)", "");
		if (!(databaseName.addTableKey(tableName, t_param1)))	//�ж��Ƿ񴴽��ɹ�
			return 0;
	}

	//����stock�ֶ� 
	if (setStock && !isStockIn)	//����wealth�ֶ�
	{
		Accessor::Table_Key t_param2;
		t_param2["stock"] = make_pair("decimal(20,6)", "");
		if (!(databaseName.addTableKey(tableName, t_param2)))	//�ж��Ƿ񴴽��ɹ�
			return 0;
	}

	return true;
}

//�����ֶ�wealth��stock��ֵ������ͳ������
float ProfitComp::incomeComp(Accessor &databaseName, float intialWealth, string tableName, string closeField, string buypointField, string sellpointField)
{
	//static int count = 0;
	
	//���ó�ʼ���𡢳�ʼ�ɷ�
	float Wealth, Stock, Income;
	float stockIncrement, wealthIncrement, Price;
	Wealth = intialWealth;
	Stock = 0;

	//��ѯ���м�¼
	Data dataSetSeclect;
	databaseName.selectRecord("select * from " + tableName + " order by CAST(Date AS DATE) ASC,Date", dataSetSeclect);

	//���õ�һ����¼���ֶ�wealth��stock��ֵ
	//vector<string> sql;
	//string sqlLine;
	//sqlLine = "update " + tableName + " set wealth='" + to_string(Wealth) + "' where Date='" + dataSetSeclect[0]["Date"] + "' ";
	//sql.push_back("update " + tableName + " set wealth='" + to_string(Wealth) + "' where Date='" + dataSetSeclect[0]["Date"] + "' ");
	//sqlLine = "update " + tableName + " set stock='0' where Date='" + dataSetSeclect[0]["Date"] + "' ";
	//sql.push_back("update " + tableName + " set stock='0' where Date='" + dataSetSeclect[0]["Date"] + "' ");
	//databaseName.updateRecord("update " + tableName + " set wealth='" + to_string(Wealth) + "' where Date='" + dataSetSeclect[0]["Date"] + "' ");
	//databaseName.updateRecord("update " + tableName + " set stock='0' where Date='" + dataSetSeclect[0]["Date"] + "' ");

	//���ó����һ�����ʣ�¼�¼���ֶ�wealth��stock��ֵ
	size_t num = 1;
	while (num < dataSetSeclect.size() - 1)
	{
		Price = stringToNum<float>(dataSetSeclect[num][closeField]);

		if (dataSetSeclect[num][buypointField] == "1" && Wealth > 1000)	//����ɷ�
		{
			stockIncrement = 1000 / Price;
			Stock = Stock + stockIncrement;
			Wealth = Wealth - 1000;
			//sqlLine = "update " + tableName + " set stock='" + to_string(stockIncrement) + "' where Date='" + dataSetSeclect[num]["Date"] + "'";
			//sql.push_back("update " + tableName + " set stock='" + to_string(stockIncrement) + "' where Date='" + dataSetSeclect[num]["Date"] + "'");
			//sqlLine = "update " + tableName + " set wealth='" + to_string(-1000) + "' where Date='" + dataSetSeclect[num]["Date"] + "'";
			//sql.push_back("update " + tableName + " set wealth='" + to_string(-1000) + "' where Date='" + dataSetSeclect[num]["Date"] + "'");
			//databaseName.updateRecord("update " + tableName + " set stock='" + to_string(stockIncrement) + "' where Date='" + dataSetSeclect[num]["Date"] + "'");			//�ɷ���
			//databaseName.updateRecord("update " + tableName + " set wealth='" + to_string(-1000) + "' where Date='" + dataSetSeclect[num]["Date"] + "'");
		}

		if (dataSetSeclect[num][sellpointField] == "1")			//�����ɷ�
		{
			wealthIncrement = Stock * Price;
			Stock = 0;
			Wealth = Wealth + wealthIncrement;
			//sqlLine = "update " + tableName + " set stock='0' where Date='" + dataSetSeclect[num]["Date"] + "'";
			//sql.push_back("update " + tableName + " set stock='0' where Date='" + dataSetSeclect[num]["Date"] + "'");
			//sqlLine = "update " + tableName + " set wealth='" + to_string(wealthIncrement) + "' where Date='" + dataSetSeclect[num]["Date"] + "'";
			//sql.push_back("update " + tableName + " set wealth='" + to_string(wealthIncrement) + "' where Date='" + dataSetSeclect[num]["Date"] + "'");
			//databaseName.updateRecord("update " + tableName + " set stock='0' where Date='" + dataSetSeclect[num]["Date"] + "'");
			//databaseName.updateRecord("update " + tableName + " set wealth='" + to_string(wealthIncrement) + "' where Date='" + dataSetSeclect[num]["Date"] + "'");
		}

		++num;
	}

	//����ͳ������Income
	if (Stock > 0)	//�������һ����¼���������йɷ�
	{
		Price = stringToNum<float>(dataSetSeclect[num][closeField]);
		wealthIncrement = Stock * Price;
		Stock = 0;
		Wealth = Wealth + wealthIncrement;
		//sqlLine = "update " + tableName + " set stock='0' where Date='" + dataSetSeclect[num]["Date"] + "'";
		//sql.push_back("update " + tableName + " set stock='0' where Date='" + dataSetSeclect[num]["Date"] + "'");
		//sqlLine = "update " + tableName + " set wealth='" + to_string(wealthIncrement) + "' where Date='" + dataSetSeclect[num]["Date"] + "'";
		//sql.push_back("update " + tableName + " set wealth='" + to_string(wealthIncrement) + "' where Date='" + dataSetSeclect[num]["Date"] + "'");
		//databaseName.updateRecord("update " + tableName + " set stock='0' where Date='" + dataSetSeclect[num]["Date"] + "'");
		//databaseName.updateRecord("update " + tableName + " set wealth='" + to_string(wealthIncrement) + "' where Date='" + dataSetSeclect[num]["Date"] + "'");
	}

	Income = (Wealth - intialWealth) / intialWealth;

	//databaseName.transExecute(sql);

	return Income;
}

//������֤����verifyIncome
float ProfitComp::verifyIncomeComp(Accessor &databaseName, float intialWealth,string tableName)
{
	float verifyIncome;

	//��ȡ�ֶ�wealth
	Data dataSet;
	databaseName.selectRecord("select wealth from " + tableName + " order by CAST(Date AS DATE) ASC,Date", dataSet);

	//���ֶ�wealth���
	float sumWealth = 0;
	for (int num = 0; num < dataSet.size(); num++)
	{
		if (isNum(dataSet[num]["wealth"]))		//�ж�wealth�ֶβ�ΪNULL����Ϊ����
			sumWealth += stringToNum<float>(dataSet[num]["wealth"]);	//��wealth�ֶ����
	}

	verifyIncome = (sumWealth - intialWealth) / intialWealth;
	return verifyIncome;
}

//��ӡ�����������
void ProfitComp::printIncome(float Income, float verifyIncome, float error, int precision)
{

	cout << "the income is " << fixed << setprecision(precision) << Income ;
	cout << ",the verifyIncome is " << fixed << setprecision(precision) << verifyIncome;
	cout << ",the error is " << error << endl;

}

//��������
float ProfitComp::compProfit(Accessor &databaseName, float intialWealth, string tableName, string buypointField, string sellpointField, string closeField, bool setWealth, bool setStock, int precision)
{
	//��鲢�ж��Ƿ񴴽��ֶ�wealth��stock
	//���ж��Ƿ��������ֶ�buypoint��sellpoint
	if (!(createWSfiled(databaseName, tableName, setWealth, setStock)))		
		return 0;

	float Income, verifyIncome;

	//�����ֶ�wealth��stock��ֵ������ͳ������
	Income = incomeComp(databaseName, intialWealth, tableName, closeField, buypointField, sellpointField);

	if (setWealth == false)
		return 0;

	////������֤����verifyIncome
	//verifyIncome = verifyIncomeComp(databaseName, intialWealth, tableName);
	//
	////�������error
	//float error = Income - verifyIncome;
	//
	////��ӡ�����������
	//printIncome(Income, verifyIncome, error, precision);
	
	return Income;
}