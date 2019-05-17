#include <iostream>
#include "SQLServerAccessor.h"
#include "MySQLAccessor.h"
#include "CSVLoader.h"
#include "DBConvolution.h"
#include "Test.h"
#include "BuyPointer.h"
#include "ProfitSearcher.h"

using namespace std;
using namespace dbaccessor;



int main()
{
	//cout << "now testing SQL Server" << endl << endl;

	////连接远程的SQL Server
	//SQLServerAccessor db1("sqlserverdbconfig.ini");
	////MySQLAccessor db1("mysqldbconfig2.ini");

	////创建测试类
	//Test testDB1;

	////测试增加N条记录
	//cout << "testInsertNRecord: ";
	//cout << (testDB1.testInsertNRecord(db1) ? "passed" : "failure") << endl;

	////测试删除N条记录
	//cout << "testDeleteNRecord: ";
	//cout << (testDB1.testDeleteNRecord(db1) ? "passed" : "failure") << endl;

	////测试更新N条记录
	//cout << "testUpdateNRecord: ";
	//cout << (testDB1.testUpdateNRecord(db1) ? "passed" : "failure") << endl << endl;

	////删除旧表
	//if (db1.findTable("BA") == true)
	//	db1.deleteTable("BA");

	////导入数据
	//CSVLoader::importCSV(db1, "BA.csv");

	////负责传参的结构体信息
	//struct ProfitArguments Arguments1(db1);
	//Arguments1.tableName = "BA";
	//Arguments1.RAfield = "agv5";
	//Arguments1.RBfield = "agv10";
	//Arguments1.alpha = 1.0;
	//Arguments1.beta = 1.0;

	//Arguments1.intialWealth = 10000;
	//Arguments1.buypointField = "buypoint";
	//Arguments1.sellpointField = "sellpoint";
	//Arguments1.closeField = "Close";

	////计算最大收益率
	//ProfitSearcher Profit1(Arguments1);
	//cout << "now computing the profit: " << endl;
	//cout << (Profit1.RandomConv(5, 10, 50, 300, 3, 1e-6) ? "passed" : "failure") <<endl<<endl;


	cout << "=========================================================================" << endl;

	cout << "now testing Mysql" << endl << endl;

	//连接远程的Mysql
	MySQLAccessor db2("mysqldbconfig2.ini");

	////创建测试类
	//Test testDB2;

	////测试增加N条记录
	//cout << "testInsertNRecord: ";
	//cout << (testDB2.testInsertNRecord(db2) ? "passed" : "failure") << endl;

	////测试删除N条记录
	//cout << "testDeleteNRecord: ";
	//cout << (testDB2.testDeleteNRecord(db2) ? "passed" : "failure") << endl;

	////测试更新N条记录
	//cout << "testUpdateNRecord: ";
	//cout << (testDB2.testUpdateNRecord(db2) ? "passed" : "failure") << endl << endl;

	//删除旧表
	if (db2.findTable("BA01") == true)
		db2.deleteTable("BA01");

	//导入数据
	CSVLoader::importCSV(db2, "BA01.csv");

	//负责传参的结构体信息
	struct ProfitArguments Arguments2(db2);
	Arguments2.tableName = "BA01";
	Arguments2.RAfield = "agv5";
	Arguments2.RBfield = "agv10";
	Arguments2.alpha = 0.942735;
	Arguments2.beta = 1.01947;

	Arguments2.intialWealth = 10000;
	Arguments2.buypointField = "buypoint";
	Arguments2.sellpointField = "sellpoint";
	Arguments2.closeField = "Close";

	//计算最大收益率
	ProfitSearcher Profit2(Arguments2);
	cout << "now computing the profit: " << endl;
	cout << (Profit2.RandomConv(5, 10, 50, 300, 3, 1e-6) ? "passed" : "failure") << endl << endl;

	/*cout << "now training the alpha and beta: " << endl;
	Profit2.trainAB(1000, 0.001);*/

	system("pause");
	return 0;

}