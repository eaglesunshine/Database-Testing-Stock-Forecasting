#include "ProfitComp.h"
#include <iostream>
#include <iomanip>
using namespace::std;

//模板函数：将string类型变量转换为常用的数值类型 
template <class Type>
Type stringToNum(const string& str)
{
	istringstream iss(str);
	Type num;
	iss >> num;
	return num;
}

//判断从数据文件读入的数据是否为数字
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

//检查并判断是否创建字段wealth、stock
bool ProfitComp::createWSfiled(Accessor &databaseName, string tableName, bool setWealth, bool setStock)
{
	//检查字段wealth、stock是否存在
	Data dataSetWealth, dataSetStock;
	bool isWealthIn = databaseName.selectRecord("select wealth from " + tableName + "", dataSetWealth);
	bool isStockIn = databaseName.selectRecord("select stock from " + tableName + "", dataSetStock);

	//创建wealth字段 
	if (setWealth && !isWealthIn)	//创建wealth字段
	{
		Accessor::Table_Key t_param1;
		t_param1["wealth"] = make_pair("decimal(20,6)", "");
		if (!(databaseName.addTableKey(tableName, t_param1)))	//判断是否创建成功
			return 0;
	}

	//创建stock字段 
	if (setStock && !isStockIn)	//创建wealth字段
	{
		Accessor::Table_Key t_param2;
		t_param2["stock"] = make_pair("decimal(20,6)", "");
		if (!(databaseName.addTableKey(tableName, t_param2)))	//判断是否创建成功
			return 0;
	}

	return true;
}

//设置字段wealth、stock的值并计算统计收益
float ProfitComp::incomeComp(Accessor &databaseName, float intialWealth, string tableName, string closeField, string buypointField, string sellpointField)
{
	//static int count = 0;
	
	//设置初始本金、初始股份
	float Wealth, Stock, Income;
	float stockIncrement, wealthIncrement, Price;
	Wealth = intialWealth;
	Stock = 0;

	//查询所有记录
	Data dataSetSeclect;
	databaseName.selectRecord("select * from " + tableName + " order by CAST(Date AS DATE) ASC,Date", dataSetSeclect);

	//设置第一条记录的字段wealth、stock的值
	//vector<string> sql;
	//string sqlLine;
	//sqlLine = "update " + tableName + " set wealth='" + to_string(Wealth) + "' where Date='" + dataSetSeclect[0]["Date"] + "' ";
	//sql.push_back("update " + tableName + " set wealth='" + to_string(Wealth) + "' where Date='" + dataSetSeclect[0]["Date"] + "' ");
	//sqlLine = "update " + tableName + " set stock='0' where Date='" + dataSetSeclect[0]["Date"] + "' ";
	//sql.push_back("update " + tableName + " set stock='0' where Date='" + dataSetSeclect[0]["Date"] + "' ");
	//databaseName.updateRecord("update " + tableName + " set wealth='" + to_string(Wealth) + "' where Date='" + dataSetSeclect[0]["Date"] + "' ");
	//databaseName.updateRecord("update " + tableName + " set stock='0' where Date='" + dataSetSeclect[0]["Date"] + "' ");

	//设置除最后一条外的剩下记录的字段wealth、stock的值
	size_t num = 1;
	while (num < dataSetSeclect.size() - 1)
	{
		Price = stringToNum<float>(dataSetSeclect[num][closeField]);

		if (dataSetSeclect[num][buypointField] == "1" && Wealth > 1000)	//买进股份
		{
			stockIncrement = 1000 / Price;
			Stock = Stock + stockIncrement;
			Wealth = Wealth - 1000;
			//sqlLine = "update " + tableName + " set stock='" + to_string(stockIncrement) + "' where Date='" + dataSetSeclect[num]["Date"] + "'";
			//sql.push_back("update " + tableName + " set stock='" + to_string(stockIncrement) + "' where Date='" + dataSetSeclect[num]["Date"] + "'");
			//sqlLine = "update " + tableName + " set wealth='" + to_string(-1000) + "' where Date='" + dataSetSeclect[num]["Date"] + "'";
			//sql.push_back("update " + tableName + " set wealth='" + to_string(-1000) + "' where Date='" + dataSetSeclect[num]["Date"] + "'");
			//databaseName.updateRecord("update " + tableName + " set stock='" + to_string(stockIncrement) + "' where Date='" + dataSetSeclect[num]["Date"] + "'");			//股份数
			//databaseName.updateRecord("update " + tableName + " set wealth='" + to_string(-1000) + "' where Date='" + dataSetSeclect[num]["Date"] + "'");
		}

		if (dataSetSeclect[num][sellpointField] == "1")			//卖出股份
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

	//计算统计收益Income
	if (Stock > 0)	//处理最后一条记录，卖出所有股份
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

//计算验证收益verifyIncome
float ProfitComp::verifyIncomeComp(Accessor &databaseName, float intialWealth,string tableName)
{
	float verifyIncome;

	//读取字段wealth
	Data dataSet;
	databaseName.selectRecord("select wealth from " + tableName + " order by CAST(Date AS DATE) ASC,Date", dataSet);

	//对字段wealth求和
	float sumWealth = 0;
	for (int num = 0; num < dataSet.size(); num++)
	{
		if (isNum(dataSet[num]["wealth"]))		//判断wealth字段不为NULL，需为数字
			sumWealth += stringToNum<float>(dataSet[num]["wealth"]);	//对wealth字段求和
	}

	verifyIncome = (sumWealth - intialWealth) / intialWealth;
	return verifyIncome;
}

//打印输出收益和误差
void ProfitComp::printIncome(float Income, float verifyIncome, float error, int precision)
{

	cout << "the income is " << fixed << setprecision(precision) << Income ;
	cout << ",the verifyIncome is " << fixed << setprecision(precision) << verifyIncome;
	cout << ",the error is " << error << endl;

}

//计算收益
float ProfitComp::compProfit(Accessor &databaseName, float intialWealth, string tableName, string buypointField, string sellpointField, string closeField, bool setWealth, bool setStock, int precision)
{
	//检查并判断是否创建字段wealth、stock
	//并判断是否生成了字段buypoint、sellpoint
	if (!(createWSfiled(databaseName, tableName, setWealth, setStock)))		
		return 0;

	float Income, verifyIncome;

	//设置字段wealth、stock的值并计算统计收益
	Income = incomeComp(databaseName, intialWealth, tableName, closeField, buypointField, sellpointField);

	if (setWealth == false)
		return 0;

	////计算验证收益verifyIncome
	//verifyIncome = verifyIncomeComp(databaseName, intialWealth, tableName);
	//
	////计算误差error
	//float error = Income - verifyIncome;
	//
	////打印输出收益和误差
	//printIncome(Income, verifyIncome, error, precision);
	
	return Income;
}