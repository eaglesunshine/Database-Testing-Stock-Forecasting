#include "BuyPointer.h"
#include <math.h>

//ģ�庯������string���ͱ���ת��Ϊ���õ���ֵ���� 
template <class Type>
Type stringToNum(const string& str)
{
	istringstream iss(str);
	Type num;
	iss >> num;
	return num;
}

//�жϴ������ļ�����������Ƿ�Ϊ����
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

//��鲢�ж��Ƿ񴴽��ֶ�buypoint��sellpoint
bool BuyPointer::createBSfiled(Accessor &databaseName, string tableName, bool setBuyPoint, bool setSellPoint)
{
	//����ֶ�buypoint��sellpoint�Ƿ����
	Data dataSetSellPoint, dataSetBuyPoint;
	bool isBuyPointIn = databaseName.selectRecord("select buypoint from " + tableName + " ", dataSetSellPoint);
	bool isSellPointIn = databaseName.selectRecord("select sellpoint from " + tableName + " ", dataSetBuyPoint);

	//����buypoint
	if (!isBuyPointIn && setBuyPoint)	//�������������������򴴽�buypoint
	{
		Accessor::Table_Key t_param1;
		t_param1["buypoint"] = make_pair("int", "");
		bool is = databaseName.addTableKey(tableName, t_param1);
		if (!(is))	//�ж��Ƿ񴴽��ɹ�
			return false;
	}

	//����sellpoint
	if (!isSellPointIn && setSellPoint)		//�������������������򴴽�sellpoint
	{
		Accessor::Table_Key t_param2;
		t_param2["sellpoint"] = make_pair("int", "");
		if (!(databaseName.addTableKey(tableName, t_param2)))	//�ж��Ƿ񴴽��ɹ�
			return false;
	}

	return true;
}

//����BuyPoint��SellPoint
bool BuyPointer::setBuySellPoint(Accessor &databaseName, string tableName, string RAfield, string RBfield, float alpha, float beta, bool setBuyPoint, bool setSellPoint)
{
	//����tableName���ֶ�RAfield��RBfield�Ƿ񶼴���
	Data dataSetRAfield, dataSetRBfield;
	bool isTableIn = databaseName.findTable(tableName);	
	bool isRAfieldIn = databaseName.selectRecord("select "+RAfield+" from "+tableName+"", dataSetRAfield);
	bool isRBfieldIn = databaseName.selectRecord("select "+RBfield+" from "+tableName+" ", dataSetRBfield);
	if (!(isTableIn && isRAfieldIn && isRBfieldIn))		//����û��ȫ�����ڣ����ٴ����ֶ�
		return false;

	//��鲢�ж��Ƿ񴴽��ֶ�buypoint��sellpoint
	if (!(createBSfiled(databaseName, tableName, setBuyPoint, setSellPoint)))		//�ж��Ƿ��������ֶ�buypoint��sellpoint
		return false;

	//��ѯ�����м�¼
	Data dataSetSeclect;
	databaseName.selectRecord("select * from " + tableName + " order by CAST(Date AS DATE) ASC,Date", dataSetSeclect);	

	//������¼����buyPoint��sellpoint
	vector<string> sql;
	for (int num = 0; num < dataSetSeclect.size(); num++)
	{
		double buyNum = stringToNum<float>(dataSetSeclect[num][RAfield]);
		double sellNum = stringToNum<float>(dataSetSeclect[num][RBfield]);

		////����ǰ(Num-1)�죬������N�վ��ߣ���������㡢����
		//if (!(isNum(dataSetSeclect[num][RAfield])) || !(isNum(dataSetSeclect[num][RBfield])))
		//	continue;

		//����buypoint
		if (buyNum > (alpha * sellNum) && setBuyPoint == true)
		{
			string str = dataSetSeclect[num]["Date"];
			//string sqlLine = "update " + tableName + " set buypoint='1' where Date='" + str + "' ";
			sql.push_back("update " + tableName + " set buypoint='1' where Date='" + str + "' ");
			//databaseName.updateRecord("update " + tableName + " set buypoint='1' where Date='" + str + "' ");
			continue;
		}

		//����SellPoint
		if (buyNum < (beta * sellNum) && setBuyPoint == true)
		{
			string str = dataSetSeclect[num]["Date"];
			//string sqlLine = "update " + tableName + " set sellpoint='1' where Date='" + str + "' ";
			sql.push_back("update " + tableName + " set sellpoint='1' where Date='" + str + "' ");
			//databaseName.updateRecord("update " + tableName + " set sellpoint='1' where Date='" + str + "' ");
		}
			
	}

	//һ���Ը���ȫ���ֶ�ֵ
	databaseName.transExecute(sql);

	return true;
}