#include <iostream>
#include "SQLServerAccessor.h"
#include "MySQLAccessor.h"
#include "CSVLoader.h"
#include "DBConvolution.h"
using namespace std;
using namespace dbaccessor;
int main()
{
	//struct ConnInfo info;
	//info.ip = "172.31.73.118";
	//info.dataBase = "db_oohomework2";
	//info.userName = "sa";
	//info.passWord = "519519519";
	//info.provider = "sqlserver";
	//SQLServerAccessor db(info);
	//从配置文件读取数据
	SQLServerAccessor db("sqlserverdbconfig.ini");
	//卷积测试
	vector<int> aa;
	aa.push_back(2);
	aa.push_back(3);
	aa.push_back(4);
	DBConvolution con(db);
	cout << (con.conv(aa, "conntest", "value3", "conn3") ? "PASS" : "FAIL") << endl;

	//csv导入测试
	//cout << (CSVLoader::importCSV(db, "D:\\Visual Studio Documents\\Visual Studio 2017\\Projects\\MyDBAccessor_Project\\DBAccessor_Project\\BA.csv") ? "PASS" : "FAIL") << endl;

	//导出CSV文件
	cout << (CSVLoader::exportCSV(db, "test") ? "PASS" : "FAIL") << endl;

	cout << (db.findTable("test") ? "PASS" : "FAIL") << endl;

	cout << ((db.buildTable("create table test111 (num int not null primary key,str varchar(10))") == 0) ? "PASS" : "FAIL") << endl;

	Accessor::Table_Key t_param;

	t_param["num"] = make_pair("int", "not null primary key");
	t_param["num11"] = make_pair("char(22)", "");
	cout << ((db.buildTable("test00", t_param) == 0) ? "PASS" : "FAIL") << endl;

	cout << (db.modifyTableName("test00", "test202") ? "PASS" : "FAIL") << endl;

	cout << (db.deleteTable("test202") ? "PASS" : "FAIL") << endl;

	cout << (db.deleteTable("test111") ? "PASS" : "FAIL") << endl;

	if (db.insertRecord("insert into  test(num,str) values('1','pppppp')") == 1)
		cout << "PASS" << endl;
	else
		cout << "FAIL" << endl;

	if (db.updateRecord("update test set str='123' where num='1' ") == 1)//
		cout << "PASS" << endl;
	else
		cout << "FAIL" << endl;

	//if (db.insertRecord("insert into  test(num,str,f) values('444','pppppp','3.210')") == 1)
	//	cout << "PASS" << endl;
	//else
	//	cout << "FAIL" << endl;

	if (db.deleteRecord("delete from  test where str='123'") == 1)
		cout << "PASS" << endl;
	else
		cout << "FAIL" << endl;
	{
		Accessor::Record rec;
		Data data22;
		rec["num"] = "22255115";
		rec["str"] = "22";
		data22.push(rec);
		rec["num"] = "111122111";
		rec["str"] = "2222";
		data22.push(rec);
		cout << (db.transInsertNRecord_Data("test", data22) ? "PASS" : "FAIL") << endl;
		cout << (db.transDeleteNRecord_Data("test", data22) ? "PASS" : "FAIL") << endl;
	}


	Data data;
	cout << (db.selectRecord("select * from  test ", data) ? "PASS" : "FAIL") << endl;
	data.printAll();

	cout << "=========================================================================" << endl;

	//struct ConnInfo info2;
	//info2.ip = "172.31.73.118";
	//info2.dataBase = "db_oohomework2";
	//info2.userName = "admin";
	//info2.passWord = "519519519";
	//info2.provider = "mysql";
	MySQLAccessor db2("mysqldbconfig.ini");
	//CSVLoader::importCSV(db2, "D:\\Visual Studio Documents\\Visual Studio 2017\\Projects\\MyDBAccessor_Project\\DBAccessor_Project\\BA.csv");
	//db.connect("Server=172.31.73.118;Database=sys;uid=admin;pwd=519519519;DSN=mysql_odbc;");
	//db2.connect(info2);
	//导出CSV文件
	cout << (CSVLoader::exportCSV(db2, "test") ? "PASS" : "FAIL") << endl;
	//db2.findTable("test");

	cout << (db2.findTable("test") ? "PASS" : "FAIL") << endl;

	cout << ((db2.buildTable("create table test111 (num int not null primary key,str varchar(10))") == 0) ? "PASS" : "FAIL") << endl;

	Accessor::Table_Key t_param2;
	t_param2["num"] = make_pair("int", "not null primary key");
	t_param2["str"] = make_pair("char(22)", "");


	cout << ((db2.buildTable("test00", t_param2) == 0) ? "PASS" : "FAIL") << endl;
	if (db2.insertRecord("insert into  test00(num,str) values('1','pppppp')") == 1)
		cout << "PASS" << endl;
	else
		cout << "FAIL" << endl;
	Data data222;
	cout << (db2.selectRecord("select * from  test00 ", data222) ? "PASS" : "FAIL") << endl;
	data222.printAll();
	//cout << ((db2.buildTable("create table test00 (num int not null primary key,str varchar(10))") == 0) ? "PASS" : "FAIL") << endl;

	cout << (db2.modifyTableName("test00", "test202") ? "PASS" : "FAIL") << endl;

	cout << (db2.deleteTable("test202") ? "PASS" : "FAIL") << endl;

	cout << (db2.deleteTable("test111") ? "PASS" : "FAIL") << endl;

	if (db2.insertRecord("insert into  test(num,str) values('1','pppppp')") == 1)
		cout << "PASS" << endl;
	else
		cout << "FAIL" << endl;

	if (db2.updateRecord("update test set str='123' where num='1'") == 1)
		cout << "PASS" << endl;
	else
		cout << "FAIL" << endl;

	//if (db2.insertRecord("insert into  test(num,str,f) values('444','pppppp','3.210')") == 1)
	//	cout << "PASS" << endl;
	//else
	//	cout << "FAIL" << endl;
	if (db2.deleteRecord("delete from  test where str='123'") == 1)
		cout << "PASS" << endl;
	else
		cout << "FAIL" << endl;



	{
		Accessor::Record rec;
		Data data22;
		rec["num"] = "22255115";
		rec["str"] = "22";
		data22.push(rec);
		rec["num"] = "111122111";
		rec["str"] = "2222";
		data22.push(rec);
		cout << (db2.transInsertNRecord_Data("test", data22) ? "PASS" : "FAIL") << endl;
		cout << (db2.transDeleteNRecord_Data("test", data22) ? "PASS" : "FAIL") << endl;
	}
	Data data2;


	db2.selectRecord("select * from  test ", data2);
	data2.printAll();
	cout << db2.getTimeZone() << endl;//system为系统时区
	db2.setTimeZone(-5);//-5:00
	cout << db2.getTimeZone() << endl;
	cout << (db2.selectRecord("select * from  test ", data2) ? "PASS" : "FAIL") << endl;
	data2.printAll();

	system("pause");
	return 0;
}