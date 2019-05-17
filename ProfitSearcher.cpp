#include "ProfitSearcher.h"
#include "CSVLoader.h"
#include <stdlib.h>
#include<time.h>

#define RANUM 5		//�����A�Ĵ�С
#define RBNUM 10	//�����B�Ĵ�С
#define N  9999		//����ΪС�������4λ
#define Pm 0.5		//���췢���ĸ���
#define Pi 0.5		//���뷢���ĸ���

//���캯��
ProfitSearcher::ProfitSearcher(ProfitArguments Arguments):databaseName(Arguments.databaseName)
{
	this->tableName=Arguments.tableName;
	this->RAfield=Arguments.RAfield;
	this->RBfield=Arguments.RBfield;
	this->alpha=Arguments.alpha;
	this->beta=Arguments.beta;
	this->setBuyPoint=Arguments.setBuyPoint;
	this->setSellPoint=Arguments.setSellPoint;

	this->intialWealth=Arguments.intialWealth;
	this->buypointField=Arguments.buypointField;
	this->sellpointField=Arguments.sellpointField;
	this->closeField=Arguments.closeField;
	this->setWealth=Arguments.setWealth;
	this->setStock=Arguments.setStock;
	this->precision=Arguments.precision;
}

//��ȡ������ɵĵ�һ����Ⱥ��Kernel
vector<float> ProfitSearcher::getKernel_0(int Num, int LOW, int HIGH)
{
	vector<float> Kernel;
	float randomNum, Sum = 0;

	for (int i = 0; i < Num ; i++)
	{
		randomNum = rand() % HIGH + LOW;
		Sum += randomNum;
		Kernel.push_back(randomNum);
	}

	for (int j = 0; j < Kernel.size(); j++)
		Kernel[j] /= Sum;			//��Num��������ֱ�����ܺͣ�ʹKernel���������֮��Ϊ1

	return Kernel;
}

//��ȡ�ߵ��񵴷ֲ�����β����ֵ�ϴ�ĵ�һ����Ⱥ��Kernel
vector<float> ProfitSearcher::getKernel_1(int Num, int LOW, int HIGH)
{
	vector<float> Kernel;
	float randomNum, Sum = 0;

	//��һ������
	randomNum = rand() % HIGH;
	Sum += randomNum;
	Kernel.push_back(randomNum);

	for (int i = 1; i < Num - 1; i++)
	{
		if (i % 2 == 0)		//�±�Ϊż���ķ������������������0~HIGH
		{
			randomNum = rand() % HIGH;
			Sum += randomNum;
			Kernel.push_back(randomNum);
		}

		if (i % 2 != 0)		//�±�Ϊ�����ķ������������������0~LOW
		{
			randomNum = rand() % LOW;
			Sum += randomNum;
			Kernel.push_back(randomNum);
		}
	}

	//���һ������ֵҪ�Ƚϴ�
	randomNum = rand() % HIGH + LOW;
	Sum += randomNum;
	Kernel.push_back(randomNum);

	for (int j = 0; j < Kernel.size(); j++)
		Kernel[j] /= Sum;			//��Num��������ֱ�����ܺͣ�ʹKernel���������֮��Ϊ1

	return Kernel;
}

//ѡ��������������̶Ĳ��ԣ�ʹ�ñ���ѡ��ѡ��һ��Agent
int ProfitSearcher::propSelect(int Generation)
{
	float m = 0.0;
	//srand((unsigned)time(NULL));		 //������������ӣ�ʹÿ�β�����������в�ͬ
	float randNum = rand() % (N + 1) / (float)(N + 1);		 //rΪ0��1�������,����ΪС�������4λ
	
	for (int i = 0; i < Adaptability[Generation].size(); i++)
	{ 
		//�������������m~m+P[i]������Ϊѡ����i�����i��ѡ�еĸ�����P[i]
		m = m + Adaptability[Generation][i];
		if (randNum <= m) 
			return i;
	}

	return 0;
}

//�����������Fa��ǰһ����Ma�ĺ�һ����н���ϲ�
vector<float> ProfitSearcher::crossKernel(int Generation, int AorB, int Fa, int Ma)
{
	vector<float> Kernel;

	//srand(time(NULL));
	//float randNum = rand() % (N + 1) / (float)(N + 1);
	int kernelSize, num;
	//��2�����尴�������Pcִ�н������,�������Kernel_A
	if (AorB == 0)
	{
		kernelSize = vectorKernel_A[Generation][Fa].size();
		for (num = 0; num < (int)(kernelSize / 2); num++)
			Kernel.push_back(vectorKernel_A[Generation][Fa][num]);

		for (; num < kernelSize - 1; num++)
			Kernel.push_back(vectorKernel_A[Generation][Ma][num]);
	}

	//��2�����尴�������Pcִ�н������,�������Kernel_B
	if (AorB == 1)
	{
		kernelSize = vectorKernel_B[Generation][Fa].size();
		for (num = 0; num < (int)(kernelSize / 2); num++)
			Kernel.push_back(vectorKernel_B[Generation][Fa][num]);

		for (; num < kernelSize - 1; num++)
			Kernel.push_back(vectorKernel_B[Generation][Ma][num]);
	}

	//�������һ������ֵ����֤������֮����Ϊ1
	float lastElem = 1.0;
	for (num = 0; num < kernelSize - 1; num++)
		lastElem = lastElem - Kernel[num];

	Kernel.push_back(lastElem);

	//���Kernel����֮�ʹ���1���������⴦��
	if (lastElem < 0)
	{
		lastElem = abs(lastElem);
		float sumTemp = 0;
		for (num = 0; num < kernelSize - 1; num++)
			sumTemp += Kernel[num];
		sumTemp += lastElem;

		vector<float> tempKernel;
		for (num = 0; num < kernelSize - 1; num++)
		{
			float temp = Kernel[num] / sumTemp;
			tempKernel.push_back(temp);
		}

		tempKernel.push_back(lastElem / sumTemp);
		Kernel.assign(tempKernel.begin(), tempKernel.end());
	}

	return Kernel;
}

//���������Kernel�е��������λ���ϵķ�������
void ProfitSearcher::variateKernel(vector<float> &Kernel)
{
	//���ѡȡ����λ�ã���Ҫ������������������λ��
	int Pos;
	int SK = Kernel.size();
	//srand(time(NULL));
	Pos = rand() % SK;
	
	//��Posλ���ϵķ�����������
	float randNum = rand() % (N + 1) / (float)(N + 1);
	randNum = randNum / 4;		//���������ֲ��и�������Ҫ��0~0.25֮��
	Kernel[Pos] = randNum;

	//�������Posλ�ú�ķ���֮��
	float SUM = 0;
	for (int i = 0; i < Kernel.size(); i++)
		SUM += Kernel[i];

	//���¸�����������֤������֮��Ϊ1
	for (int i = 0; i < Kernel.size(); i++)
		Kernel[i] = Kernel[i] / SUM;
	
}

//��������η�ת������ʵ�������ƶ�Kλ
void ProfitSearcher::Reverse(vector<float> &KP, int p, int q)
{
	for (; p < q; p++, q--)
		swap(KP[p], KP[q]);
}

//�����������Kernel�е�ĳ�������Ƭ����λ����һ�������λ��
void ProfitSearcher::insertKernel(vector<float> &Kernel)
{
	int Pos1, Pos2, Pos3;
	int Start, End;

	//�������Ƭ�ε�����λ���±�
	int SK = Kernel.size();
	Pos1 = rand() % SK;
	Pos2 = Pos1;

	while (Pos2 == Pos1)
	{
		Pos2 = rand() % SK;
	}

	//��ȡ���Ƭ�εĿ�ʼλ�ú���ֹλ��
	if (Pos1 < Pos2)
	{
		Start = Pos1;
		End = Pos2;
	}
	if (Pos1 > Pos2)
	{
		Start = Pos2;
		End = Pos1;
	}

	//�����ƶ�Ŀ��λ��
	Pos3 = Start;
	while (Pos3 == Start)
	{
		Pos3 = rand() % SK;
	}

	//��Kernel�еĴ�Start��Endλ�õ�Ƭ����λ��Ŀ��λ��Pos3
	int Distance = abs(Pos3 - Start);
	vector<float> Temp;
	if (Start < Pos3)	//�����Ƭ�������ƶ���Ŀ��λ��Pos3
	{
		int num;
		//��Start��Last�ĵķ�������
		for (num = Start; num < Kernel.size(); num++)
			Temp.push_back(Kernel[num]);
		//��Tempѭ������Distance��λ��
		int n = Temp.size();
		Reverse(Temp, 0, n - Distance - 1);
		Reverse(Temp, n - Distance, n - 1);
		Reverse(Temp, 0, n - 1);

		//����Start��Last�ĵķ����ķ���
		int i = 0;
		for (num = Start; num < Kernel.size(); num++)
			Kernel[num] = Temp[i++];
	}

	if (Start > Pos3)	//�����Ƭ�������ƶ���Ŀ��λ��Pos3
	{
		int num;
		//��First��End�ĵķ�������
		for (num = 0; num < End; num++)
			Temp.push_back(Kernel[num]);
		//��Tempѭ������Distance��λ��
		int n = Temp.size();
		Reverse(Temp, 0, Distance - 1);
		Reverse(Temp, Distance, n - 1);
		Reverse(Temp, 0, n - 1);

		//����First��End�ĵķ����ķ���
		int i = 0;
		for (num = 0; num < End; num++)
			Kernel[num] = Temp[i++];
	}

}

//�Ӹ�����Ⱥ��ѡ������������н��桢����ĵ��µ�Kernel
vector<float> ProfitSearcher::getKernel_2(int Generation,int AorB)
{
	vector<float> Kernel;
	float randNum;

	//ѡ��
	int Fa, Ma;		//ѡ���ĸ��塢ĸ����±�
	Fa = propSelect(Generation);		//�������̶Ĳ��ԣ�ʹ�ñ���ѡ��ѡ��һ��Agent
	Ma = propSelect(Generation);

	//����
	Kernel = crossKernel(Generation, AorB, Fa, Ma);
	
	//����
	randNum = rand() % (N + 1) / (float)(N + 1);		
	if (randNum < Pm)		//���������ִ�б������
		variateKernel(Kernel);
	
	//����
	randNum = rand() % (N + 1) / (float)(N + 1);		
	if (randNum < Pi)		//���������ִ�в������
		insertKernel(Kernel);

	return Kernel;
}

//��鲢�ж��Ƿ񴴽��ֶ�RAfield��RBfield
bool ProfitSearcher::createABfiled()
{
	//����ֶ�RAfield��RBfield�Ƿ����
	Data dataSetRAfield, dataSetRBfield;
	bool isRAfieldIn = databaseName.selectRecord("select "+ RAfield +" from " + tableName + " ", dataSetRAfield);
	bool isRBfieldIn = databaseName.selectRecord("select " + RBfield + " from " + tableName + " ", dataSetRBfield);

	//����RAfield
	if (!isRAfieldIn)	//�������������������򴴽�buypoint
	{
		Accessor::Table_Key t_param1;
		t_param1[RAfield] = make_pair("decimal(20,6)", "");
		if (!(databaseName.addTableKey(tableName, t_param1)))	//�ж��Ƿ񴴽��ɹ�
			return false;
	}

	//����RBfield
	if (!isRBfieldIn)		//�������������������򴴽�sellpoint
	{
		Accessor::Table_Key t_param2;
		t_param2[RBfield] = make_pair("decimal(20,6)", "");
		if (!(databaseName.addTableKey(tableName, t_param2)))	//�ж��Ƿ񴴽��ɹ�
			return false;
	}

	return true;
}

//�����һ�����ɵ����н���ֶ�
void ProfitSearcher::clearLastResult()
{
	databaseName.updateRecord("update " + tableName + " set " + RAfield + "=null ");
	databaseName.updateRecord("update " + tableName + " set " + RBfield + "=null ");
	databaseName.updateRecord("update " + tableName + " set " + buypointField + "=null ");
	databaseName.updateRecord("update " + tableName + " set " + sellpointField + "=null ");
	databaseName.updateRecord("update " + tableName + " set wealth=null ");
	databaseName.updateRecord("update " + tableName + " set stock=null ");
}

//��ÿһ�������������ҳ���
int ProfitSearcher::findMaxProfit(int Generation)
{
	float Max = vectorProfit[Generation][0];
	int best = 0;
	int num;
	for (num = 1; num < vectorProfit[Generation].size(); num++)
	{
		if (Max < vectorProfit[Generation][num])
		{
			Max = vectorProfit[Generation][num];
			best = num;
		}
	}
	return best;
}

//����ÿһ����Ⱥ����Ӧ��ֵ,��ת���ɸ���ֵ
void ProfitSearcher::adaptCompute(int Generation)
{
	//����Ⱥ��ĸ���������M����ô��ôһ����Xi��ѡ�еĸ���Ϊf(Xi)/( f(X1) + f(X2) + ����.. + f(Xn) ) 
	float profitSum = 0;
	int i;
	for (i = 0; i < vectorProfit[Generation].size(); i++)
		profitSum = profitSum + vectorProfit[Generation][i] + 1;	//��Ϊ�����ʿ���Ϊ��ֵ�����Զ�ÿһ��������+1�����

	float Adapt;
	vector<float> NPadapt;
	for (i = 0; i < vectorProfit[Generation].size(); i++)
	{
		Adapt = (vectorProfit[Generation][i] + 1) / profitSum;
		NPadapt.push_back(Adapt);
	}

	Adaptability.push_back(NPadapt);
}

//������ȺPop����һ��
bool ProfitSearcher::generatePop(int lengthKernel_A, int lengthKernel_B, int Generation,int limitNumber)
{
	float profit = 1;
	DBConvolution DBConv(databaseName);
	BuyPointer setPoint;
	ProfitComp Profit;

	vector<vector<float>> PopKernel_A;
	vector<vector<float>> PopKernel_B;
	vector<float> NProfit;

	//��鲢�ж��Ƿ񴴽��ֶ�RAfield��RBfield
	if (!(createABfiled()))		//�ж��Ƿ��������ֶ�RAfield��RBfield
		return false;

	cout << "now computing Pop " << Generation << ": " << endl;

	int num = 0;	//�����ɵ�Agent����
	while (num < limitNumber)
	{
		//�����һ�����ɵ����н���ֶΣ�����Ӱ���´μ���
		clearLastResult();
		
		vector<float> Kernel_A, Kernel_B;

		if (Generation == 0)	//��һ����Kernel���������
		{
			//�������Kernel_A��Kernel_B���ߵ��񵴷ֲ�
			Kernel_A = getKernel_1(lengthKernel_A, 200, 600);  
			Kernel_B = getKernel_1(lengthKernel_B, 200, 600);

			////�������Kernel_A��Kernel_B������ֲ�
			//Kernel_A = getKernel_0(lengthKernel_A, 0, 1000);
			//Kernel_B = getKernel_0(lengthKernel_B, 0, 1000);
		}

		if (Generation > 0)	//��һ���Ժ��Kernel��ͨ�����桢�������ɵ�
		{
			//�������Kernel_A��Kernel_B�������Ŵ��㷨��ͨ���������õ�
			Kernel_A = getKernel_2(Generation-1, 0);
			Kernel_B = getKernel_2(Generation-1, 1);
		}

		//�洢��һ���ĵ�num��Agent��Kernelֵ
		PopKernel_A.push_back(Kernel_A);
		PopKernel_B.push_back(Kernel_B);
		
		//�����Ǽ��㲢�����һ���ĵ�num��Agent��Profitֵ

		//�����ֶ�RAfield��RBfield
		DBConv.conv(Kernel_A, tableName, closeField, RAfield);
		DBConv.conv(Kernel_B, tableName, closeField, RBfield);

		//�����ֶ�buypoint��sellpoint
		setPoint.setBuySellPoint(databaseName, tableName, RAfield, RBfield, alpha, beta, setBuyPoint, setSellPoint);

		//����ʹ�õ�ǰ������µ�������
		profit = Profit.compProfit(databaseName, intialWealth, tableName, buypointField, sellpointField, closeField, setWealth, setStock, precision);

		//�洢Profitֵ
		NProfit.push_back(profit);	
		
		//��ӡ���
		string stringKernel_A = transform(Kernel_A);
		string stringKernel_B = transform(Kernel_B);
		cout << num << ": the Kernel_A is " << stringKernel_A << ", the Kernel_B is " << stringKernel_B << ", the profit is " << profit << endl;

		++num;
	}

	
	////���þ�Ӣ���ԣ���ÿһ���е����Ž�ԭ�ⲻ���ĸ��Ƶ���һ����
	//if (Generation > 0)
	//{
	//	PopKernel_A.push_back(maxKernel_A[Generation - 1]);
	//	PopKernel_B.push_back(maxKernel_B[Generation - 1]);
	//	NProfit.push_back(maxProfit[Generation - 1]);
	//	string stringKernel_A = transform(maxKernel_A[Generation - 1]);
	//	string stringKernel_B = transform(maxKernel_B[Generation - 1]);
	//	cout << num << ": the Kernel_A is " << stringKernel_A << ", the Kernel_B is " << stringKernel_B << ", the profit is " << maxProfit[Generation - 1] << endl;
	//}

	//�洢��һ�������Kernel��profit
	vectorKernel_A.push_back(PopKernel_A);
	vectorKernel_B.push_back(PopKernel_B);
	vectorProfit.push_back(NProfit);

	//������һ����Ⱥ����Ӧ��ֵ
	adaptCompute(Generation);

	//��ÿһ�������������ҳ���
	int bestProfitPos;
	bestProfitPos = findMaxProfit(Generation);
	maxProfit.push_back(vectorProfit[Generation][bestProfitPos]);
	maxKernel_A.push_back(vectorKernel_A[Generation][bestProfitPos]);
	maxKernel_B.push_back(vectorKernel_B[Generation][bestProfitPos]);

	cout << endl;
	cout << "the bestProfit in this Pop is " << maxProfit[Generation] << endl;

	cout << endl;

	return true;
}

//���������ı�����Ӧ�ֶ�
void ProfitSearcher::createResultTable()
{
	//ɾ���ɱ�
	if (databaseName.findTable("Result") == true)
		databaseName.deleteTable("Result");

	//�����ձ�
	Accessor::Table_Key t_param;
	t_param["ID"] = make_pair("varchar(22)", "");
	t_param["TableName"] = make_pair("varchar(22)","");
	t_param["CloseFiled"] = make_pair("varchar(22)", "");
	t_param["RAfiled"] = make_pair("varchar(22)", "");
	t_param["RBfiled"] = make_pair("varchar(22)", "");
	t_param["Kernel_A"] = make_pair("varchar(200)", "");
	t_param["Kernel_B"] = make_pair("varchar(200)", "");
	t_param["Profit"] = make_pair("decimal(20,6)", "");
	databaseName.buildTable("Result", t_param);	//������Result
}

//������Kernel�ı�����Ӧ�ֶ�
void ProfitSearcher::createKernelTable()
{
	//ɾ���ɱ�
	if (databaseName.findTable("Kernel_A") == true)
		databaseName.deleteTable("Kernel_A");
	if (databaseName.findTable("Kernel_B") == true)
		databaseName.deleteTable("Kernel_B");

	//�����ձ�
	Accessor::Table_Key t_param;
	/*for(int num = 0; num < 20; num++)
		t_param[to_string(num)] = make_pair("varchar(22)", "");*/
	t_param["ID1"] = make_pair("varchar(22)", "");
	t_param["ID2"] = make_pair("varchar(22)", "");
	t_param["ID3"] = make_pair("varchar(22)", "");
	t_param["ID4"] = make_pair("varchar(22)", "");
	t_param["ID5"] = make_pair("varchar(22)", "");
	t_param["ID6"] = make_pair("varchar(22)", "");
	t_param["ID7"] = make_pair("varchar(22)", "");
	t_param["ID8"] = make_pair("varchar(22)", "");
	t_param["ID9"] = make_pair("varchar(22)", "");
	t_param["ID10"] = make_pair("varchar(22)", "");
	//t_param["ID"] = make_pair("varchar(22)", "");

	databaseName.buildTable("Kernel_A", t_param);	//������Kernel_A
	databaseName.buildTable("Kernel_B", t_param);	//������Kernel_B 

	/*for (int num = 1; num < 51; num++)
	{ 
		databaseName.insertRecord("insert into Kernel_A (ID) values (" + to_string(num) + ")");
		databaseName.insertRecord("insert into Kernel_B (ID) values (" + to_string(num) + ")");
	}*/
}

//��Kernelת���ɿɲ������ݿ��string����
string ProfitSearcher::transform(vector<float> Kernel)
{
	string result="(";
	for (int i = 0; i < Kernel.size(); i++)
	{
		result = result.append(to_string(Kernel[i]));
		if(i< (Kernel.size()-1))
			result = result.append(", ");
	}
	result = result.append(")");

	return result;
}

//��ֳ�������ҳ����и��������ŵ�50������
void ProfitSearcher::findBestAgent(int limtGeneration)
{
	int num, i, j;
	
	//��ȡ���и����Profit��Kernel
	for (num = 0; num < limtGeneration; num++)
	{
		int SIZE = vectorProfit[num].size();
		for (i = 0; i < SIZE; i++)
		{
			sumProfit.push_back(vectorProfit[num][i]);
			sumKernel_A.push_back(vectorKernel_A[num][i]);
			sumKernel_B.push_back(vectorKernel_B[num][i]);
		}
	}

	//�ҳ���������50������

	//����ѡ�����򣬶����и��尴����Ӵ�С����
	for (i = 0; i < sumProfit.size(); i++)
	{
		int MAX = i;
		for (j = i + 1; j < sumProfit.size(); j++)
		{
			if (sumProfit[MAX] < sumProfit[j])
				MAX = j;
		}

		if (MAX != i)
		{
			swap(sumProfit[MAX], sumProfit[i]);
			swap(sumKernel_A[MAX], sumKernel_A[i]);
			swap(sumKernel_B[MAX], sumKernel_B[i]);
		}
	}

	//������������е�ǰ50�������Kernel��Profit���뵽���ݿ���
	//���������ı�����Ӧ�ֶ�
	createResultTable();
	//��Kernel_A��Kernel_B���������������ڵ���Kernel������ͼ�鿴�ֲ�
	createKernelTable();
	Accessor::Record rec1,rec2;
	Data dataSet1,dataSet2;
	for (i = 0; i < 50; i++)
	{
		string id = to_string(i + 1);
		string Kernel_A = transform(sumKernel_A[i]);
		string Kernel_B = transform(sumKernel_B[i]);
		string profit = to_string(sumProfit[i]);
		databaseName.insertRecord("insert into Result (ID, TableName,CloseFiled,RAFiled,RBFiled,Kernel_A,Kernel_B,Profit) values ( '"+id+"', '" + tableName + "', '" + closeField + "', '" + RAfield + "', '" + RBfield + "', '" + Kernel_A + "', '" + Kernel_B + "', '" + profit + "')");
		
		//�洢һ�м�¼����һ��Kernel_A
		rec1["ID1"]= to_string(sumKernel_A[i][0]);
		rec1["ID2"] = to_string(sumKernel_A[i][1]);
		rec1["ID3"] = to_string(sumKernel_A[i][2]);
		rec1["ID4"] = to_string(sumKernel_A[i][3]);
		rec1["ID5"] = to_string(sumKernel_A[i][4]);
		dataSet1.push(rec1);

		//�洢һ�м�¼����һ��Kernel_B
		rec2["ID1"] = to_string(sumKernel_B[i][0]);
		rec2["ID2"] = to_string(sumKernel_B[i][1]);
		rec2["ID3"] = to_string(sumKernel_B[i][2]);
		rec2["ID4"] = to_string(sumKernel_B[i][3]);
		rec2["ID5"] = to_string(sumKernel_B[i][4]);
		rec2["ID6"] = to_string(sumKernel_B[i][5]);
		rec2["ID7"] = to_string(sumKernel_B[i][6]);
		rec2["ID8"] = to_string(sumKernel_B[i][7]);
		rec2["ID9"] = to_string(sumKernel_B[i][8]);
		rec2["ID10"] = to_string(sumKernel_B[i][9]);
		dataSet2.push(rec2);
	}

	//��50��Kernel�������ݿ�
	databaseName.transInsertNRecord_Data("Kernel_A", dataSet1);
	databaseName.transInsertNRecord_Data("Kernel_B", dataSet2);
}

//ѵ��alpha��beta
bool ProfitSearcher::trainAB(int limitNum, float Rate)
{
	float profit = 1;
	DBConvolution DBConv(databaseName);
	BuyPointer setPoint;
	ProfitComp Profit;

	vector<float> vecAlpha;		//�洢alpha
	vector<float> vecBeta;		//�洢beta
	vector<float> vecProfit;	//�洢profit

	//���ɹ̶���Kernel����
	vector<float> Kernel_A, Kernel_B;
	int i;
	for (i = 0; i < 10; i++)
	{
		Kernel_B.push_back(0.1);
		if (i < 5)
			Kernel_A.push_back(0.2);
	}

	//ѵ��limitNum��alpha��beta������limitNum�β�ͬalpha��beta�µ��������
	int num = 0;
	float alpha, beta;
	int isGood = 0;
	srand((unsigned)time(NULL));	//���������������
	while (num < limitNum)
	{
		//�����һ�����ɵ����н���ֶΣ�����Ӱ���´μ���
		clearLastResult();

		if (num == 0)		//��alpha=beta=1
		{
			alpha = 1.0;
			beta = 1.0;
		}

		if (num > 0 && isGood==0)	//���ϴ�����С��0�����������alpha��beta��0.99��1.01֮���С��
		{
			alpha = 0.99 + 0.02*(rand() % (N + 1) / (float)(N + 1));
			beta = 0.99 + 0.02*(rand() % (N + 1) / (float)(N + 1));
		}

		if (num > 0 && isGood == 1)	//���ϴ��������0����������ɵ�alpha��beta���ϴνϺõĲ�������
		{
			float ranNum = rand() % (N + 1) / (float)(N + 1);
			if(ranNum<0.5)
				alpha = alpha + Rate*(rand() % (N + 1) / (float)(N + 1));
			else
				alpha = alpha - Rate*(rand() % (N + 1) / (float)(N + 1));

			ranNum= rand() % (N + 1) / (float)(N + 1);
			if (ranNum < 0.5)
				beta = beta + Rate*(rand() % (N + 1) / (float)(N + 1));
			else
				beta = beta - Rate*(rand() % (N + 1) / (float)(N + 1));
		}

		//��鲢�ж��Ƿ񴴽��ֶ�RAfield��RBfield
		if (!(createABfiled()))		//�ж��Ƿ��������ֶ�RAfield��RBfield
			return false;

		//�����ֶ�RAfield��RBfield
		DBConv.conv(Kernel_A, tableName, closeField, RAfield);
		DBConv.conv(Kernel_B, tableName, closeField, RBfield);

		//�����ֶ�buypoint��sellpoint
		setPoint.setBuySellPoint(databaseName, tableName, RAfield, RBfield, alpha, beta, setBuyPoint, setSellPoint);

		//����ʹ�õ�ǰ������µ�������
		profit = Profit.compProfit(databaseName, intialWealth, tableName, buypointField, sellpointField, closeField, setWealth, setStock, precision);

		//�ж���������Ƿ��Ǻõ�����
		if (profit > 0)
			isGood = 1;
		if(profit <= 0)
			isGood = 0;

		//�洢��¼
		vecAlpha.push_back(alpha);
		vecBeta.push_back(beta);
		vecProfit.push_back(profit);

		//��ӡ���
		cout << num << ": the alpha is " << alpha << ", the beta is " << beta << ", the profit is " << profit << endl;

		++num;
	}

	//��ӡ�����õ�alpha��beta
	float bestProfit = vecProfit[0];
	int bestNum = 0;
	for (int i = 1; i < vecProfit.size(); i++)
	{
		if (bestProfit < vecProfit[i])
		{
			bestProfit = vecProfit[i];
			bestNum = i;
		}
	}

	cout << "OVER: the bestAlpha is " << vecAlpha[bestNum] << ", the bestBeta is " << vecBeta[bestNum] << ", the profit is " << bestProfit << endl;
	return true;
}

//��������������
bool ProfitSearcher::RandomConv(int lengthKernel_A, int lengthKernel_B, int initialNumber, int limitNumber, int limtGeneration, float error)
{
	//���������������
	srand((unsigned)time(NULL)); 

	//������ɵ�һ��
	generatePop(lengthKernel_A, lengthKernel_B, 0, initialNumber);

	//�����������������֮��
	float Gap=10;

	int Gerention = 1;	//��ֳ����
	//Gap<error && 
	while (Gerention < limtGeneration)	 //����ֳ������������ʱ����ֳ����
	{
		//���ɵ�Gerention��
		generatePop(lengthKernel_A, lengthKernel_B, Gerention, limitNumber);

		//���������������������֮��
		Gap = abs(maxProfit[Gerention] - maxProfit[Gerention - 1]);

		++Gerention;
	}

	//��ֳ�������ҳ����и��������ŵ�50������
	findBestAgent(limtGeneration);

	//��������ļ���Result.csv, Kernel_A.csv, Kernel_B.csv
	CSVLoader::exportCSV(databaseName, "Result");
	CSVLoader::exportCSV(databaseName, "Kernel_A");
	CSVLoader::exportCSV(databaseName, "Kernel_B");

	return true;
}