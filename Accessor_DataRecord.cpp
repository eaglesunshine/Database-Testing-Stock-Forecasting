#include "Accessor_DataRecord.h"
using namespace dbaccessor;

dbaccessor::Data_Record::Data_Record(map<string, string> &record) :_record(record)
{

}

void dbaccessor::Data_Record::printALL()
{
	cout << "===================================================" << endl;

	for (map<string, string>::iterator it = _record.begin(); it != _record.end(); ++it)
	{
		std::cout << (it->first) << " \t";
	}

	for (auto it = _record.begin(); it != _record.end(); ++it)
	{
		if (it->second == "")
			cout << "--\t";
		else
			cout << (it->second) << "\t";
	}

	cout << endl;
	cout << "===================================================" << endl;
}

const string& dbaccessor::Data_Record::operator[](const string &s)
{
	//string str = s;
	//transform(str.begin(), str.end(), str.begin(), ::tolower);
	map<string, string>::const_iterator it = _record.find(s);

	//if (it != _record.end())
		return it->second;
	//return "";
}

map<string, string>& dbaccessor::Data_Record::getRecord()
{
	return _record;
}

int dbaccessor::Data_Record::getSize()
{
	return (int)_record.size();
}
