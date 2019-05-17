#include "BuyPointer.h"
#include <math.h>

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
bool BuyPointer::isNum(string str)
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

//检查并判断是否创建字段buypoint、sellpoint
bool BuyPointer::createBSfiled(Accessor &databaseName, string tableName, bool setBuyPoint, bool setSellPoint)
{
	//检查字段buypoint、sellpoint是否存在
	Data dataSetSellPoint, dataSetBuyPoint;
	bool isBuyPointIn = databaseName.selectRecord("select buypoint from " + tableName + " ", dataSetSellPoint);
	bool isSellPointIn = databaseName.selectRecord("select sellpoint from " + tableName + " ", dataSetBuyPoint);

	//创建buypoint
	if (!isBuyPointIn && setBuyPoint)	//当两个条件都成立，则创建buypoint
	{
		Accessor::Table_Key t_param1;
		t_param1["buypoint"] = make_pair("int", "");
		bool is = databaseName.addTableKey(tableName, t_param1);
		if (!(is))	//判断是否创建成功
			return false;
	}

	//创建sellpoint
	if (!isSellPointIn && setSellPoint)		//当两个条件都成立，则创建sellpoint
	{
		Accessor::Table_Key t_param2;
		t_param2["sellpoint"] = make_pair("int", "");
		if (!(databaseName.addTableKey(tableName, t_param2)))	//判断是否创建成功
			return false;
	}

	return true;
}

//设置BuyPoint、SellPoint
bool BuyPointer::setBuySellPoint(Accessor &databaseName, string tableName, string RAfield, string RBfield, float alpha, float beta, bool setBuyPoint, bool setSellPoint)
{
	//检查表tableName和字段RAfield、RBfield是否都存在
	Data dataSetRAfield, dataSetRBfield;
	bool isTableIn = databaseName.findTable(tableName);	
	bool isRAfieldIn = databaseName.selectRecord("select "+RAfield+" from "+tableName+"", dataSetRAfield);
	bool isRBfieldIn = databaseName.selectRecord("select "+RBfield+" from "+tableName+" ", dataSetRBfield);
	if (!(isTableIn && isRAfieldIn && isRBfieldIn))		//三者没有全部存在，则不再创建字段
		return false;

	//检查并判断是否创建字段buypoint、sellpoint
	if (!(createBSfiled(databaseName, tableName, setBuyPoint, setSellPoint)))		//判断是否生成了字段buypoint、sellpoint
		return false;

	//查询出所有记录
	Data dataSetSeclect;
	databaseName.selectRecord("select * from " + tableName + " order by CAST(Date AS DATE) ASC,Date", dataSetSeclect);	

	//逐条记录设置buyPoint、sellpoint
	vector<string> sql;
	for (int num = 0; num < dataSetSeclect.size(); num++)
	{
		double buyNum = stringToNum<float>(dataSetSeclect[num][RAfield]);
		double sellNum = stringToNum<float>(dataSetSeclect[num][RBfield]);

		////对于前(Num-1)天，不计算N日均线，不设置买点、卖点
		//if (!(isNum(dataSetSeclect[num][RAfield])) || !(isNum(dataSetSeclect[num][RBfield])))
		//	continue;

		//设置buypoint
		if (buyNum > (alpha * sellNum) && setBuyPoint == true)
		{
			string str = dataSetSeclect[num]["Date"];
			//string sqlLine = "update " + tableName + " set buypoint='1' where Date='" + str + "' ";
			sql.push_back("update " + tableName + " set buypoint='1' where Date='" + str + "' ");
			//databaseName.updateRecord("update " + tableName + " set buypoint='1' where Date='" + str + "' ");
			continue;
		}

		//设置SellPoint
		if (buyNum < (beta * sellNum) && setBuyPoint == true)
		{
			string str = dataSetSeclect[num]["Date"];
			//string sqlLine = "update " + tableName + " set sellpoint='1' where Date='" + str + "' ";
			sql.push_back("update " + tableName + " set sellpoint='1' where Date='" + str + "' ");
			//databaseName.updateRecord("update " + tableName + " set sellpoint='1' where Date='" + str + "' ");
		}
			
	}

	//一次性更新全部字段值
	databaseName.transExecute(sql);

	return true;
}