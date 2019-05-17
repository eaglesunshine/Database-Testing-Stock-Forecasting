#include "Accessor.h"
using namespace dbaccessor;
using namespace std;
#import "c:\Program Files\Common Files\System\ado\msado60.tlb" no_namespace rename("EOF","adoEOF")

dbaccessor::Accessor::Accessor(ConnInfo info)
{
	connInfo = info;
	connString = buildConnStr(connInfo);
	init();

}

dbaccessor::Accessor::Accessor(string filePath)
{
	ConfigFile::readConfig(filePath, connInfo);
	connString = buildConnStr(connInfo);//由于此时子类的构造函数还未执行，所以此虚函数指向父类的函数不指向子类的函数
	init();

}

dbaccessor::Accessor::~Accessor()
{
	disConnect();
	pConnection.Release();
	pRecordset.Release();
	::CoUninitialize();
}

bool dbaccessor::Accessor::_connect(const string &sql)
{
	bool result = false;

	try
	{
		disConnect();

		if (pConnection->GetState() == adStateClosed)
		{
			pConnection->ConnectionString = (_bstr_t)sql.c_str();
			pConnection->ConnectionTimeout = connInfo.connectTimeout;
			pConnection->CommandTimeout = connInfo.connectTimeout;
			pConnection->Open("", "", "", adConnectUnspecified);

			if (pConnection == NULL)
			{

				std::cerr << "Lind data ERROR!\n";
				result = false;
			}
			else
				result = true;

		}
		else
			result = false;

	}
	catch (const _com_error e)
	{
		showErrorMesg(e);
		result = false;
	}

	return result;
}

int dbaccessor::Accessor::buildTable(const string & sql)
{
	string tableName = getTableNameFromSql(sql);
	int result = -1;//发生异常

	if (!findTable(tableName))
	{

		if (execute(sql) == 0 ? true : false)
			result = 0;
		else
			result = 2;//未知错误

	}
	else
		result = 1;//表已存在

	return result;
}

string dbaccessor::Accessor::getTableNameFromSql(const string &sql)
{
	const char *_str = sql.c_str();
	char *save = NULL, *str = NULL, *ptr = NULL;
	ostringstream osStr;
	osStr << "";
	str = new char[sql.length() + 1];
	strcpy_s(str, sql.length() + 1, _str);
	ptr = strtok_s(str, " ", &save);

	while (ptr != NULL)
	{

		if (strcmp(ptr, "table") == 0)
		{
			ptr = strtok_s(NULL, " ", &save);
			if (ptr != NULL)
				osStr << ptr;
			break;
		}

		ptr = strtok_s(NULL, " ", &save);
	}

	delete(str);
	return osStr.str();
}

void dbaccessor::Accessor::showErrorMesg(const _com_error &e)
{
#ifdef DEBUG
	cerr << e.ErrorMessage() << endl;
	cerr << e.Description() << endl;
	//cerr << e.ErrorInfo() << endl;
	//cerr << e.HelpContext() << endl;
	//cerr << e.WCode() << endl;
	//cerr << ShowErrorMessage(e.Error()) << endl;
#endif
}

bool dbaccessor::Accessor::connect(const string &sql)
{
	bool result = false;
	int count = tryTime;

	while (count-- > 0 && !result)
	{
		result = _connect(sql);
	}

	return result;

}

bool dbaccessor::Accessor::connect()
{
	return connect(connString);
}

string dbaccessor::Accessor::buildConnStr(ConnInfo &info)
{
	//"Server=172.31.73.118;Database=sys;uid=admin;pwd=519519519;DSN=mysql_odbc;"
	ostringstream osStr;
	osStr << "DSN=" << info.provider << ';';
	osStr << "Server=" << info.ip << ';';
	osStr << "Database=" << info.dataBase << ';';
	osStr << "uid=" << info.userName << ';';
	osStr << "pwd=" << info.passWord << ';';
	return osStr.str();
}

void dbaccessor::Accessor::disConnect()
{

	if (pRecordset->GetState() != adRecOK)
		pRecordset->Close();

	if (pConnection)
		if (pConnection->State)//== adStateOpen)
			pConnection->Close();

}

bool dbaccessor::Accessor::transInsertNRecord_Data(const string &sql, Data &data)
{
	bool update_State = false;
	int count = tryTime;
	bool result = false;
	resetAutoReleaseConn();

	while (count--)
	{
		open(sql);

		try {

			if (result && pRecordset != NULL && pRecordset->GetState() != adRecOK)//&&m_pRecordset->adoEOF)
			{
				pConnection->BeginTrans();

				for (size_t i = 0; i < data.size(); ++i)
				{
					Record record = data[i].getRecord();
					update_State = true;
					pRecordset->AddNew();

					for (auto it = record.begin(); it != record.end(); ++it)
						pRecordset->PutCollect(_variant_t((it->first).c_str()), _variant_t((it->second).c_str()));

				}

				pRecordset->Update();
				pConnection->CommitTrans();
				result = true;
				break;
			}


		}
		catch (_com_error e) {

			if (update_State)
				pRecordset->CancelUpdate();

			pConnection->RollbackTrans();
			showErrorMesg(e);
			result = false;
		}
	}
	setAutoReleaseConn();

	return result;
}


bool dbaccessor::Accessor::addTableKey(const string & tableName, Table_Key & tableParam)
{
	bool result = false;
	ostringstream osStr;
	osStr << "alter table " << tableName << " add ( ";
	auto endIter = tableParam.end();
	for (auto iter = tableParam.begin(); iter != endIter; )
	{
		osStr << " " << iter->first << " " << iter->second.first << ' ' << iter->second.second << ' ';
		++iter;
		if (iter != endIter)
			osStr << ", ";
	}
	osStr << " )";
	execute(osStr.str()) == -1 ? result = true : result = false;
	return result;
}

bool dbaccessor::Accessor::transExecute(const vector<string>& sql)
{
	int count = tryTime;

	while (count-- > 0 && (getConnectState() || connect()))
	{
		const char *AA;
		try {
			pConnection->BeginTrans();
			string str_sql;
			
			for (auto it = sql.begin(); it != sql.end(); ++it) 
			{
				AA = ((*it)).c_str();
				pConnection->Execute((_bstr_t)((string)(*it)).c_str(), NULL, adCmdText);
			}

			pConnection->CommitTrans();
			break;
		}
		catch (_com_error e) {
			pConnection->RollbackTrans();
			showErrorMesg(e);
		}
	}

	if (autoReleaseConn)
		disConnect();

	return true;
}

bool dbaccessor::Accessor::open(const string &sql)
{
	int result = false;

	int count = tryTime;

	while (count-- > 0 && (getConnectState() || connect()))
	{
		try
		{

			if (pRecordset->GetState() != adRecOK)
				pRecordset->Close();
			_variant_t RecordsAffected;
			//pRecordset->CursorLocation = adUseClient;
			pRecordset->Open((_bstr_t)sql.c_str(),                // 查询DemoTable表中所有字段 
				pConnection.GetInterfacePtr(),	// 获取库接库的IDispatch指针 
				adOpenStatic,
				adLockOptimistic,
				adCmdText);
			result = true;
			break;

		}
		catch (const _com_error e)
		{
			result = false;
			showErrorMesg(e);
		}
	}

	if (autoReleaseConn)
		disConnect();

	return result;

}

void dbaccessor::Accessor::init()
{
	::CoInitialize(NULL);//初始化OLE/COM库环境，为访问ADO接口做准备
//pConnection = NULL;
	pConnection.CreateInstance(__uuidof(Connection));
	//pRecordset = NULL;
	pRecordset.CreateInstance(__uuidof(Recordset));
	pRecordset->CursorType = adOpenStatic;
	pRecordset->CursorLocation = adUseClient;
	tryTime = connInfo.tryTime;
}

int dbaccessor::Accessor::getConnectState()
{
	int result = -1;

	try
	{
		result = pConnection->GetState();
	}
	catch (const _com_error e)
	{
		showErrorMesg(e);
	}

	return result;
}

int dbaccessor::Accessor::execute(const string &sql)
{
	int result = -2;

	int count = tryTime;

	while (count-- > 0 && (getConnectState() || connect()))
	{
		try
		{
			_variant_t RecordsAffected;
			pConnection->Execute((_bstr_t)sql.c_str(), &RecordsAffected, adCmdText);
			//cout << (LPCTSTR)_bstr_t(RecordsAffected);;
			//cout << RecordsAffected.intVal << "," << RecordsAffected.lVal << endl;
			result = RecordsAffected.intVal;
			break;
		}
		catch (const _com_error e)
		{
			result = -2;
			showErrorMesg(e);
		}
	}

	if (autoReleaseConn)
		disConnect();

	return result;
}

bool dbaccessor::Accessor::transDeleteNRecord_Data(const string & tableName, Data & data)
{
	vector<string> sql;
	ostringstream osStr;
	for (int i = 0; i < data.size(); ++i)
	{
		osStr.str("");
		osStr.clear();
		osStr << "delete from " << tableName << " where ";
		//valueStr << " value('";
		Record record = data[i].getRecord();
		auto endit = record.end();
		for (auto it = record.begin(); it != endit; )
		{
			osStr << "( " << it->first << "= ";
			osStr << "'" << it->second << "')";
			++it;
			if (it != endit)
			{
				osStr << " and ";
			}



		}
		sql.push_back(osStr.str());
	}
	if (sql.size() > 0)
		return Accessor::transExecute(sql);
	return false;
}

bool dbaccessor::Accessor::findTable(const string &sql)
{
	bool result = false;
	resetAutoReleaseConn();

	if (open(sql.c_str()) == true)
	{
		if (getRecordCount() > 0)
		{
			result = true;
		}

	}

	setAutoReleaseConn();
	return result;
}

int dbaccessor::Accessor::insertRecord(const string &sql)
{
	return execute(sql);
}

int dbaccessor::Accessor::updateRecord(const string &sql)
{
	return execute(sql);
}

int dbaccessor::Accessor::deleteRecord(const string &sql)
{
	return execute(sql);
}

bool dbaccessor::Accessor::selectRecord(const string &sql, Data &data)
{
	bool result = false;
	data.clear();
	resetAutoReleaseConn();

	if (open(sql) == true)
	{
		getRecord(data);
		result = true;
	}

	setAutoReleaseConn();
	return result;

}

bool dbaccessor::Accessor::deleteTable(const string & tableName)
{
	return execute("drop table " + tableName + "") == -1 ? true : false;
}

void dbaccessor::Accessor::getRecord(Data &data)
{
	vector<string> vtFields;
	int fieldsCount = pRecordset->GetFields()->Count;

	try
	{

		for (int i = 0; i < fieldsCount; ++i)
		{
			BSTR str;
			pRecordset->GetFields()->GetItem((long)i)->get_Name(&str);
			//LPCTSTR _str = (LPCTSTR)str;
			vtFields.push_back((string)((_bstr_t)str));
		}

		map<string, string> mpRow;
		//pRecordset->MoveFirst();

		while (!pRecordset->adoEOF)
		{
			_variant_t var;
			mpRow.clear();
			int ii = (int)pRecordset->GetRecordCount();

			for (size_t i = 0; i < vtFields.size(); i++)
			{

				var = pRecordset->GetCollect(vtFields[i].c_str());

				if (var.vt != VT_NULL)
				{
					//string str= (string)_bstr_t(var);
					//string sstr = vtFields[i];
					//mpRow[sstr] = str;
					mpRow[vtFields[i]] = (string)_bstr_t(var);
				}

				else
					mpRow[vtFields[i]] = "";

			}

			data.push(mpRow);
			pRecordset->MoveNext();
		}

	}
	catch (const _com_error e)
	{
		showErrorMesg(e);
	}


}

void dbaccessor::Accessor::setAutoReleaseConn()
{
	autoReleaseConn = true;
	disConnect();
}

void dbaccessor::Accessor::resetAutoReleaseConn()
{
	autoReleaseConn = false;
}

int dbaccessor::Accessor::getRecordCount()
{
	return (int)pRecordset->GetRecordCount();
}
//
//Accessor::KeyName Accessor::tryGetPrimaryKey(map<Key_Value, Record> &data)
//{
//	string str = "";
//
//	for (auto it = data.begin(); it != data.end(); ++it)
//	{
//		for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2)
//		{
//
//			if (it2->second == it->first)
//			{
//				str = it2->first;
//				break;
//			}
//
//		}
//	}
//
//	return str;
//}
