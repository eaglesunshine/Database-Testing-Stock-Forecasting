#pragma once
#include "Accessor.h"

//设置buypoint、sellpoint的类
class BuyPointer
{
public:
	bool isNum(string str);
	/*
	*功能：判断是否为数字
	*输入：需要判断的字段值
	*输出：若为数字字符输出true，否则输出false
	*/

	bool createBSfiled(Accessor &databaseName, string tableName, bool setBuyPoint, bool setSellPoint);
	/*
	*功能：检查并创建字段buypoint、sellpoint
	*输入：数据库连接名，表名，是否设置buypoint，是否设置sellpoint
	*输出：表中生成了两个字段就输出true，否则输出false
	*/

	bool setBuySellPoint(Accessor &databaseName, string tableName, string RAfield, string RBfield, float alpha, float beta, bool setBuyPoint = true, bool setSellPoint = true);
	/*
	*功能：设置BuyPoint、SellPoint
	*输入：数据库连接名，表名，买点参考值字段名，卖点参考值字段名，权重1，权重2，是否设置buypoint，是否设置sellpoint
	*输出：设置BuyPoint、SellPoint成功就输出true
	*/
};