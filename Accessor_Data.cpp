#include "Accessor_Data.h"
using namespace dbaccessor;

void dbaccessor::Data::push(map<string, string> & map)
{
	_data.push_back(map);
}

void dbaccessor::Data::clear()
{
	_data.clear();
}

void dbaccessor::Data::erase(int i)
{
	_data.erase(_data.begin() + i);
}

int dbaccessor::Data::size()
{
	return (int)_data.size();
}

dbaccessor::Data_Record dbaccessor::Data::operator[](size_t i)
{
	return Data_Record(_data[i]);
}


void dbaccessor::Data::printAll()
{
	bool isFirstColumn = true;
	cout << "===================================================" << endl;

	for (size_t i = 0; i < this->size(); ++i)
	{

		for (auto it = (*this)[i].getRecord().begin(); isFirstColumn&& it != (*this)[i].getRecord().end(); ++it)
		{
			std::cout << (it->first) << "    ";
		}

		if (isFirstColumn)
		{
			isFirstColumn = false;
			cout << endl;
		}

		for (auto it = (*this)[i].getRecord().begin(); it != (*this)[i].getRecord().end(); ++it)
		{

			if (it->second == "")
				cout << "--    ";
			else
				cout << (it->second) << "    ";

		}

		cout << endl;
	}

	cout << "===================================================" << endl;
}