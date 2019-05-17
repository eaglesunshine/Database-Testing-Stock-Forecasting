#include "Accessor.h"

class ProfitComp
{
public:
	bool isNum(string str);
	/*
	*功能：判断从数据文件读入的数据是否为数字
	*输入：需要判断的字段值
	*输出：若为数字字符输出true，否则输出false
	*/

	bool createWSfiled(Accessor &databaseName, string tableName, bool setWealth, bool setStock);
	/*
	*功能：检查并判断是否创建字段wealth、stock
	*输入：数据库连接名，表名，是否设置wealth，是否设置stock
	*输出：若表中生成了两个字段就输出true，否则输出false
	*/

	float incomeComp(Accessor &databaseName, float intialWealth, string tableName, string closeField, string buypointField, string sellpointField);
	/*
	*功能：设置字段wealth、stock的值并计算统计收益Income
	*输入：数据库连接名，初始本金，表名，Close字段名，buypoint字段名，sellpoint字段名
	*输出：输出计算得到的统计收益Income
	*/

	float verifyIncomeComp(Accessor &databaseName, float intialWealth, string tableName);
	/*
	*功能：计算验证收益verifyIncome
	*输入：数据库连接名，初始本金，表名
	*输出：输出计算得到的验证收益verifyIncome
	*/

	void printIncome(float Income, float verifyIncome, float error, int precision);
	/*
	*功能：打印输出收益和误差
	*输入：统计收益，验证收益，误差，小数位数
	*输出：打印结果
	*/

	//计算收益
	float compProfit(Accessor &databaseName, float intialWealth, string tableName, string buypointField, string sellpointField, string closeField, bool setWealth = true, bool setStock = true, int precision = 6);
	/*
	*功能：计算收益
	*输入：数据库连接名，初始本金，表名，buypoint字段名，sellpoint字段名，Close字段名，是否设置wealth，是否设置stock
	*输出：输出统计收益值Income
	*/

};