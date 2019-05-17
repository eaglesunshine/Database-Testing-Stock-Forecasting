#include "CSVLoader.h"

bool CSVLoader::isDateTime(const string str)
{
	int year, month, day, hour, minute, second;// ����ʱ��ĸ���int��ʱ������
	int result = sscanf_s(str.c_str(), "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);// ��string�洢������ʱ�䣬ת��Ϊint��ʱ������

	if (result > 3)
		return true;

	result = sscanf_s(str.c_str(), "%d/%d/%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);// ��string�洢������ʱ�䣬ת��Ϊint��ʱ������

	if (result > 3)
		return true;

	return false;
}

bool CSVLoader::isDate(const string str)
{
	int year, month, day, hour, minute, second;// ����ʱ��ĸ���int��ʱ������
	int result = sscanf_s(str.c_str(), "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);// ��string�洢������ʱ�䣬ת��Ϊint��ʱ������

	if (result == 3)
		return true;

	result = sscanf_s(str.c_str(), "%d/%d/%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);// ��string�洢������ʱ�䣬ת��Ϊint��ʱ������

	if (result == 3)
		return true;

	return false;
}

bool CSVLoader::isDouble(const string str)
{
	double temp;

	try
	{
		temp = stod(str);
		//instr >> temp;
	}
	catch (const std::exception&)
	{
		return false;
	}

	ostringstream osStr;
	int precision = numOfPrecision(str);
	osStr << fixed << setprecision(precision) << temp;

	if (str == osStr.str())
		return true;

	return false;
}

bool CSVLoader::isInt(const string str)
{
	int temp;

	try
	{
		temp = stoi(str);
	}
	catch (const std::exception&)
	{
		return false;
	}

	ostringstream osStr;
	osStr << temp;

	if (str == osStr.str())
		return true;

	return false;
}

string CSVLoader::isWhatType(string value)
{

	if (isDateTime(value))
	{
		return "datetime";
	}

	if (isDate(value))
	{
		return "date";
	}

	if (isInt(value))
	{
		return "int";
	}

	if (isDouble(value))
	{
		return "decimal(20,6)";
	}

	return "string";
}

bool CSVLoader::isWhatType(string value, string type)
{

	if (type == "string")
		return true;

	if (type == "datetime"&&isDateTime(value))
	{
		return true;
	}

	if (type == "date"&&isDate(value))
	{
		return true;
	}

	if (type == "int"&&isInt(value))
	{
		return true;
	}

	if (type == "decimal(20,6)"&&isDouble(value))
	{

		if (stod(value) > 0)
			return true;

		return false;
	}

	return false;
}

int CSVLoader::numOfPrecision(const string & value)
{
	basic_string <char>::size_type indexCh1a;

	if ((indexCh1a = value.find('.')) == string::npos)
		return 0;//(int)value.length() - 1;

	return (int)value.length() - (int)indexCh1a - 1;
}

string CSVLoader::maxMatchKey(map<string, int> matchTime)
{
	string maxStr = "string";
	int maxCount = 0;

	for (auto it = matchTime.begin(); it != matchTime.end(); ++it)
	{

		if (it->second > maxCount)
		{
			maxStr = it->first;
			maxCount = it->second;
		}

	}

	return maxStr;
}

int CSVLoader::checkData(Accessor::Table_Key &tableKey, dbaccessor::Data & data)
{
	int errorCount = 0;
	auto it = tableKey.begin();
	auto endit = tableKey.end();

	for (int i = 0; i < data.size(); )
	{

		for (it = tableKey.begin(); it != endit;)
		{

			if (!isWhatType(data[i][it->first], it->second.first))
			{
				data.erase(i);
				++errorCount;
				break;
			}

			if (++it == endit)
				++i;

		}

	}

	return errorCount;
}

bool CSVLoader::loadToData(const string filePath, dbaccessor::Data &data)
{
	try
	{
		ifstream fin(filePath); //���ļ�������
		string line;
		vector<string> fields; //��������
		map<string, string> mpRow; //��¼����

		while (getline(fin, line))   //���ж�ȡ�����з���\n�����֣������ļ�β��־eof��ֹ��ȡ
		{
			istringstream sin(line); //�������ַ���line���뵽�ַ�����istringstream��
			string field;//��ǰ��ֵ
			int field_num = 0;//��ǰ�ǵڼ���

			if (fields.size() == 0)//��һ��Ϊ�ֶ�������ȡ�ֶ���
			{

				while (getline(sin, field, ',')) //���ַ�����sin�е��ַ����뵽field�ַ����У��Զ���Ϊ�ָ���
				{

					if (field == "")
						fields.push_back("UnknowName" + to_string(fields.size() + 1));
					else
						fields.push_back(field); //���ոն�ȡ���ַ�����ӵ�����fields��

				}

			}
			else//��ȡ��¼
			{
				mpRow.clear();

				while (getline(sin, field, ',')) //���ַ�����sin�е��ַ����뵽field�ַ����У��Զ���Ϊ�ָ���
				{
					mpRow[fields[field_num++]] = field; //���ոն�ȡ���ַ�����ӵ�map��
				}

				data.push(mpRow);
			}

		}
		fin.close();
	}
	catch (const std::exception&)
	{
		data.clear();
		return false;
	}

	return true;
}

bool CSVLoader::createTable(Accessor & acc, const string &tableName, Accessor::Table_Key tableKey)
{
	return acc.buildTable(tableName, tableKey);;
}

bool CSVLoader::importToDataBase(Accessor & acc, const string tableName, dbaccessor::Data & data)
{
	return acc.transInsertNRecord_Data(tableName, data);
}

string CSVLoader::getTableNameFromFileName(const string fileName)
{
	return fileName.substr(0, fileName.rfind("."));
}

string CSVLoader::getFileNameFromFilePath(const string filePath)
{

	basic_string <char>::size_type indexCh1a;
	static const basic_string <char>::size_type npos = -1;
	indexCh1a = filePath.find_last_of('\\');

	if (indexCh1a == npos)
		return filePath;
	else
	{
		return string(&(filePath.c_str())[indexCh1a + 1]);
	}

}

Accessor::Table_Key CSVLoader::tryBuildTableConfig(dbaccessor::Data & data)
{
	Accessor::Table_Key config;
	vector<string> keyName;

	if (data.size() > 0)
	{

		for (auto it = data[0].getRecord().begin(); it != data[0].getRecord().end(); ++it)
		{
			keyName.push_back(it->first);
		}

		for (int i = 0; i < keyName.size(); ++i)
		{
			map<string, int> matchTime;
			string str;

			for (int j = 0; j < data.size() && j < keyTypeTryCount; ++j)
			{
				str = isWhatType(data[j][keyName[i]]);

				if (matchTime.find(str) != matchTime.end())
					++matchTime[str];
				else
					matchTime[str] = 0;

			}
			str = maxMatchKey(matchTime);

			if (str == "datetime" || str == "date")
				config[keyName[i]] = make_pair(str, "primary key not null");
			else
				config[keyName[i]] = make_pair(str, "");

		}

	}

	return config;
}

bool CSVLoader::exportToCSV(const string &tableName, Data &data)
{
	ofstream outFile;
	//Data data;
	bool result = false;
	result = createCSVFile(outFile, tableName);
	result && (result = exportKeyName(outFile, data));
	result && (result = exportKeyValue(outFile, data));

	try
	{
		outFile.close();
	}
	catch (const std::exception&)
	{

	}

	return result;
}

bool CSVLoader::exportKeyName(ofstream & outFile, dbaccessor::Data & data)
{

	if (data.size() > 0)
	{
		auto endit = data[0].getRecord().end();

		for (auto it = data[0].getRecord().begin(); it != endit; )
		{
			outFile << it->first;
			++it;

			if (it != endit)
				outFile << ",";

		}

		outFile << endl;
		return true;
	}

	return false;
}

bool CSVLoader::exportKeyValue(ofstream & outFile, dbaccessor::Data & data)
{
	bool result = false;
	int size = data.size();
	int i = 0;

	for (i = 0; result = outFile.good() && i < size; i++)
	{
		auto endit = data[i].getRecord().end();

		for (auto it = data[i].getRecord().begin(); it != endit; )
		{
			outFile <<"\""<< it->second << "\"";
			++it;

			if (it != endit)
				outFile << ",";

		}
		outFile << endl;
	}

	if (i == size)
		result = true;

	return result;
}

bool CSVLoader::createCSVFile(ofstream & outFile, const string & tableName)
{
	const int tryTime = 20;
	int countTime = 0;

	if (isFileExist(tableName + ".csv"))
	{
		int backupId = 0;
		ostringstream osStr;

		do
		{
			++backupId;
			osStr.str("");
			osStr.clear();
			osStr << tableName << "_" << backupId << ".csv";
		} while (isFileExist(osStr.str()) && ++countTime < tryTime);

		outFile.open(osStr.str(), ios::out); // ��ģʽ��ʡ��
	}
	else
		outFile.open(tableName + ".csv", ios::out); // ��ģʽ��ʡ��

	if (!outFile)
		return false;

	return true;
}

bool CSVLoader::isFileExist(const string & filePath)
{
	ifstream inFile;
	inFile.open(filePath);

	if (!inFile)
		return false;

	inFile.close();
	return true;
}

bool CSVLoader::importCSV(Accessor & acc, const string filePath)
{
	bool result = false;
	dbaccessor::Data data;

	if (loadToData(filePath, data))
	{
		string fileName = getFileNameFromFilePath(filePath);
		string tableName = getTableNameFromFileName(fileName);
		Accessor::Table_Key tableKey = tryBuildTableConfig(data);
		checkData(tableKey, data);

		if (createTable(acc, tableName, tableKey) == 0)
			importToDataBase(acc, tableName, data) ? result = true : NULL;

	}
	return result;

}

bool CSVLoader::exportCSV(Accessor & acc, const string tableName)
{
	dbaccessor::Data data;

	if (acc.selectRecord("select * from " + tableName, data))
		if (exportToCSV(tableName, data))
			return true;

	return false;
}




