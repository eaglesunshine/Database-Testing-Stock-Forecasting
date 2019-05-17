#include "ProfitSearcher.h"
#include "CSVLoader.h"
#include <stdlib.h>
#include<time.h>

#define RANUM 5		//卷积核A的大小
#define RBNUM 10	//卷积核B的大小
#define N  9999		//精度为小数点后面4位
#define Pm 0.5		//变异发生的概率
#define Pi 0.5		//插入发生的概率

//构造函数
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

//获取随机生成的第一代种群的Kernel
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
		Kernel[j] /= Sum;			//将Num个随机数分别除以总和，使Kernel满足各分量之和为1

	return Kernel;
}

//获取高低振荡分布且首尾分量值较大的第一代种群的Kernel
vector<float> ProfitSearcher::getKernel_1(int Num, int LOW, int HIGH)
{
	vector<float> Kernel;
	float randomNum, Sum = 0;

	//第一个分量
	randomNum = rand() % HIGH;
	Sum += randomNum;
	Kernel.push_back(randomNum);

	for (int i = 1; i < Num - 1; i++)
	{
		if (i % 2 == 0)		//下标为偶数的分量随机数生成区间是0~HIGH
		{
			randomNum = rand() % HIGH;
			Sum += randomNum;
			Kernel.push_back(randomNum);
		}

		if (i % 2 != 0)		//下标为奇数的分量随机数生成区间是0~LOW
		{
			randomNum = rand() % LOW;
			Sum += randomNum;
			Kernel.push_back(randomNum);
		}
	}

	//最后一个分量值要比较大
	randomNum = rand() % HIGH + LOW;
	Sum += randomNum;
	Kernel.push_back(randomNum);

	for (int j = 0; j < Kernel.size(); j++)
		Kernel[j] /= Sum;			//将Num个随机数分别除以总和，使Kernel满足各分量之和为1

	return Kernel;
}

//选择操作：采用轮盘赌策略，使用比例选择法选出一个Agent
int ProfitSearcher::propSelect(int Generation)
{
	float m = 0.0;
	//srand((unsigned)time(NULL));		 //设置随机数种子，使每次产生的随机序列不同
	float randNum = rand() % (N + 1) / (float)(N + 1);		 //r为0至1的随机数,精度为小数点后面4位
	
	for (int i = 0; i < Adaptability[Generation].size(); i++)
	{ 
		//产生的随机数在m~m+P[i]间则认为选中了i，因此i被选中的概率是P[i]
		m = m + Adaptability[Generation][i];
		if (randNum <= m) 
			return i;
	}

	return 0;
}

//交叉操作：将Fa的前一半与Ma的后一半进行交叉合并
vector<float> ProfitSearcher::crossKernel(int Generation, int AorB, int Fa, int Ma)
{
	vector<float> Kernel;

	//srand(time(NULL));
	//float randNum = rand() % (N + 1) / (float)(N + 1);
	int kernelSize, num;
	//对2个个体按交叉概率Pc执行交叉操作,这里针对Kernel_A
	if (AorB == 0)
	{
		kernelSize = vectorKernel_A[Generation][Fa].size();
		for (num = 0; num < (int)(kernelSize / 2); num++)
			Kernel.push_back(vectorKernel_A[Generation][Fa][num]);

		for (; num < kernelSize - 1; num++)
			Kernel.push_back(vectorKernel_A[Generation][Ma][num]);
	}

	//对2个个体按交叉概率Pc执行交叉操作,这里针对Kernel_B
	if (AorB == 1)
	{
		kernelSize = vectorKernel_B[Generation][Fa].size();
		for (num = 0; num < (int)(kernelSize / 2); num++)
			Kernel.push_back(vectorKernel_B[Generation][Fa][num]);

		for (; num < kernelSize - 1; num++)
			Kernel.push_back(vectorKernel_B[Generation][Ma][num]);
	}

	//计算最后一个分量值，保证各分量之和仍为1
	float lastElem = 1.0;
	for (num = 0; num < kernelSize - 1; num++)
		lastElem = lastElem - Kernel[num];

	Kernel.push_back(lastElem);

	//如果Kernel分量之和大于1，则需另外处理
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

//变异操作：Kernel中的随机两个位置上的分量互换
void ProfitSearcher::variateKernel(vector<float> &Kernel)
{
	//随机选取变异位置，即要发生分量交换的两个位置
	int Pos;
	int SK = Kernel.size();
	//srand(time(NULL));
	Pos = rand() % SK;
	
	//将Pos位置上的分量重新生成
	float randNum = rand() % (N + 1) / (float)(N + 1);
	randNum = randNum / 4;		//根据向量分布有各分量主要在0~0.25之间
	Kernel[Pos] = randNum;

	//计算更新Pos位置后的分量之和
	float SUM = 0;
	for (int i = 0; i < Kernel.size(); i++)
		SUM += Kernel[i];

	//更新各个分量，保证各分量之和为1
	for (int i = 0; i < Kernel.size(); i++)
		Kernel[i] = Kernel[i] / SUM;
	
}

//经典的三次反转方法：实现数组移动K位
void ProfitSearcher::Reverse(vector<float> &KP, int p, int q)
{
	for (; p < q; p++, q--)
		swap(KP[p], KP[q]);
}

//插入操作：将Kernel中的某个随机的片段移位到另一个随机的位置
void ProfitSearcher::insertKernel(vector<float> &Kernel)
{
	int Pos1, Pos2, Pos3;
	int Start, End;

	//生成随机片段的两个位置下标
	int SK = Kernel.size();
	Pos1 = rand() % SK;
	Pos2 = Pos1;

	while (Pos2 == Pos1)
	{
		Pos2 = rand() % SK;
	}

	//获取随机片段的开始位置和终止位置
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

	//生成移动目标位置
	Pos3 = Start;
	while (Pos3 == Start)
	{
		Pos3 = rand() % SK;
	}

	//将Kernel中的从Start到End位置的片段移位到目标位置Pos3
	int Distance = abs(Pos3 - Start);
	vector<float> Temp;
	if (Start < Pos3)	//将随机片段向右移动到目标位置Pos3
	{
		int num;
		//将Start到Last的的分量读出
		for (num = Start; num < Kernel.size(); num++)
			Temp.push_back(Kernel[num]);
		//将Temp循环右移Distance个位置
		int n = Temp.size();
		Reverse(Temp, 0, n - Distance - 1);
		Reverse(Temp, n - Distance, n - 1);
		Reverse(Temp, 0, n - 1);

		//更新Start到Last的的分量的分量
		int i = 0;
		for (num = Start; num < Kernel.size(); num++)
			Kernel[num] = Temp[i++];
	}

	if (Start > Pos3)	//将随机片段向左移动到目标位置Pos3
	{
		int num;
		//将First到End的的分量读出
		for (num = 0; num < End; num++)
			Temp.push_back(Kernel[num]);
		//将Temp循环左移Distance个位置
		int n = Temp.size();
		Reverse(Temp, 0, Distance - 1);
		Reverse(Temp, Distance, n - 1);
		Reverse(Temp, 0, n - 1);

		//更新First到End的的分量的分量
		int i = 0;
		for (num = 0; num < End; num++)
			Kernel[num] = Temp[i++];
	}

}

//从父代种群中选出两个个体进行交叉、变异的到新的Kernel
vector<float> ProfitSearcher::getKernel_2(int Generation,int AorB)
{
	vector<float> Kernel;
	float randNum;

	//选择
	int Fa, Ma;		//选出的父体、母体的下标
	Fa = propSelect(Generation);		//采用轮盘赌策略，使用比例选择法选出一个Agent
	Ma = propSelect(Generation);

	//交叉
	Kernel = crossKernel(Generation, AorB, Fa, Ma);
	
	//变异
	randNum = rand() % (N + 1) / (float)(N + 1);		
	if (randNum < Pm)		//按变异概率执行变异操作
		variateKernel(Kernel);
	
	//插入
	randNum = rand() % (N + 1) / (float)(N + 1);		
	if (randNum < Pi)		//按插入概率执行插入操作
		insertKernel(Kernel);

	return Kernel;
}

//检查并判断是否创建字段RAfield、RBfield
bool ProfitSearcher::createABfiled()
{
	//检查字段RAfield、RBfield是否存在
	Data dataSetRAfield, dataSetRBfield;
	bool isRAfieldIn = databaseName.selectRecord("select "+ RAfield +" from " + tableName + " ", dataSetRAfield);
	bool isRBfieldIn = databaseName.selectRecord("select " + RBfield + " from " + tableName + " ", dataSetRBfield);

	//创建RAfield
	if (!isRAfieldIn)	//当两个条件都成立，则创建buypoint
	{
		Accessor::Table_Key t_param1;
		t_param1[RAfield] = make_pair("decimal(20,6)", "");
		if (!(databaseName.addTableKey(tableName, t_param1)))	//判断是否创建成功
			return false;
	}

	//创建RBfield
	if (!isRBfieldIn)		//当两个条件都成立，则创建sellpoint
	{
		Accessor::Table_Key t_param2;
		t_param2[RBfield] = make_pair("decimal(20,6)", "");
		if (!(databaseName.addTableKey(tableName, t_param2)))	//判断是否创建成功
			return false;
	}

	return true;
}

//清空上一次生成的运行结果字段
void ProfitSearcher::clearLastResult()
{
	databaseName.updateRecord("update " + tableName + " set " + RAfield + "=null ");
	databaseName.updateRecord("update " + tableName + " set " + RBfield + "=null ");
	databaseName.updateRecord("update " + tableName + " set " + buypointField + "=null ");
	databaseName.updateRecord("update " + tableName + " set " + sellpointField + "=null ");
	databaseName.updateRecord("update " + tableName + " set wealth=null ");
	databaseName.updateRecord("update " + tableName + " set stock=null ");
}

//把每一代的最优收益找出来
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

//计算每一代种群的适应度值,并转换成概率值
void ProfitSearcher::adaptCompute(int Generation)
{
	//假设群体的个体总数是M，那么那么一个体Xi被选中的概率为f(Xi)/( f(X1) + f(X2) + …….. + f(Xn) ) 
	float profitSum = 0;
	int i;
	for (i = 0; i < vectorProfit[Generation].size(); i++)
		profitSum = profitSum + vectorProfit[Generation][i] + 1;	//因为收益率可能为负值，所以对每一个收益率+1再求和

	float Adapt;
	vector<float> NPadapt;
	for (i = 0; i < vectorProfit[Generation].size(); i++)
	{
		Adapt = (vectorProfit[Generation][i] + 1) / profitSum;
		NPadapt.push_back(Adapt);
	}

	Adaptability.push_back(NPadapt);
}

//生成种群Pop的新一代
bool ProfitSearcher::generatePop(int lengthKernel_A, int lengthKernel_B, int Generation,int limitNumber)
{
	float profit = 1;
	DBConvolution DBConv(databaseName);
	BuyPointer setPoint;
	ProfitComp Profit;

	vector<vector<float>> PopKernel_A;
	vector<vector<float>> PopKernel_B;
	vector<float> NProfit;

	//检查并判断是否创建字段RAfield、RBfield
	if (!(createABfiled()))		//判断是否生成了字段RAfield、RBfield
		return false;

	cout << "now computing Pop " << Generation << ": " << endl;

	int num = 0;	//对生成的Agent计数
	while (num < limitNumber)
	{
		//清空上一次生成的运行结果字段，否则影响下次计算
		clearLastResult();
		
		vector<float> Kernel_A, Kernel_B;

		if (Generation == 0)	//第一代的Kernel是随机生成
		{
			//随机生成Kernel_A、Kernel_B，高低振荡分布
			Kernel_A = getKernel_1(lengthKernel_A, 200, 600);  
			Kernel_B = getKernel_1(lengthKernel_B, 200, 600);

			////随机生成Kernel_A、Kernel_B，随机分布
			//Kernel_A = getKernel_0(lengthKernel_A, 0, 1000);
			//Kernel_B = getKernel_0(lengthKernel_B, 0, 1000);
		}

		if (Generation > 0)	//第一代以后的Kernel是通过交叉、变异生成的
		{
			//随机生成Kernel_A、Kernel_B，采用遗传算法，通过交叉变异得到
			Kernel_A = getKernel_2(Generation-1, 0);
			Kernel_B = getKernel_2(Generation-1, 1);
		}

		//存储新一代的第num个Agent的Kernel值
		PopKernel_A.push_back(Kernel_A);
		PopKernel_B.push_back(Kernel_B);
		
		//以下是计算并储存第一代的第num个Agent的Profit值

		//设置字段RAfield、RBfield
		DBConv.conv(Kernel_A, tableName, closeField, RAfield);
		DBConv.conv(Kernel_B, tableName, closeField, RBfield);

		//设置字段buypoint、sellpoint
		setPoint.setBuySellPoint(databaseName, tableName, RAfield, RBfield, alpha, beta, setBuyPoint, setSellPoint);

		//计算使用当前卷积核下的收益率
		profit = Profit.compProfit(databaseName, intialWealth, tableName, buypointField, sellpointField, closeField, setWealth, setStock, precision);

		//存储Profit值
		NProfit.push_back(profit);	
		
		//打印输出
		string stringKernel_A = transform(Kernel_A);
		string stringKernel_B = transform(Kernel_B);
		cout << num << ": the Kernel_A is " << stringKernel_A << ", the Kernel_B is " << stringKernel_B << ", the profit is " << profit << endl;

		++num;
	}

	
	////采用精英策略，将每一代中的最优解原封不动的复制到下一代中
	//if (Generation > 0)
	//{
	//	PopKernel_A.push_back(maxKernel_A[Generation - 1]);
	//	PopKernel_B.push_back(maxKernel_B[Generation - 1]);
	//	NProfit.push_back(maxProfit[Generation - 1]);
	//	string stringKernel_A = transform(maxKernel_A[Generation - 1]);
	//	string stringKernel_B = transform(maxKernel_B[Generation - 1]);
	//	cout << num << ": the Kernel_A is " << stringKernel_A << ", the Kernel_B is " << stringKernel_B << ", the profit is " << maxProfit[Generation - 1] << endl;
	//}

	//存储新一代个体的Kernel和profit
	vectorKernel_A.push_back(PopKernel_A);
	vectorKernel_B.push_back(PopKernel_B);
	vectorProfit.push_back(NProfit);

	//计算新一代种群的适应度值
	adaptCompute(Generation);

	//把每一代的最优收益找出来
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

//创建存结果的表与相应字段
void ProfitSearcher::createResultTable()
{
	//删除旧表
	if (databaseName.findTable("Result") == true)
		databaseName.deleteTable("Result");

	//创建空表
	Accessor::Table_Key t_param;
	t_param["ID"] = make_pair("varchar(22)", "");
	t_param["TableName"] = make_pair("varchar(22)","");
	t_param["CloseFiled"] = make_pair("varchar(22)", "");
	t_param["RAfiled"] = make_pair("varchar(22)", "");
	t_param["RBfiled"] = make_pair("varchar(22)", "");
	t_param["Kernel_A"] = make_pair("varchar(200)", "");
	t_param["Kernel_B"] = make_pair("varchar(200)", "");
	t_param["Profit"] = make_pair("decimal(20,6)", "");
	databaseName.buildTable("Result", t_param);	//创建表Result
}

//创建存Kernel的表与相应字段
void ProfitSearcher::createKernelTable()
{
	//删除旧表
	if (databaseName.findTable("Kernel_A") == true)
		databaseName.deleteTable("Kernel_A");
	if (databaseName.findTable("Kernel_B") == true)
		databaseName.deleteTable("Kernel_B");

	//创建空表
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

	databaseName.buildTable("Kernel_A", t_param);	//创建表Kernel_A
	databaseName.buildTable("Kernel_B", t_param);	//创建表Kernel_B 

	/*for (int num = 1; num < 51; num++)
	{ 
		databaseName.insertRecord("insert into Kernel_A (ID) values (" + to_string(num) + ")");
		databaseName.insertRecord("insert into Kernel_B (ID) values (" + to_string(num) + ")");
	}*/
}

//将Kernel转换成可插入数据库的string类型
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

//繁殖结束后，找出所有个体中最优的50个个体
void ProfitSearcher::findBestAgent(int limtGeneration)
{
	int num, i, j;
	
	//读取所有个体的Profit、Kernel
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

	//找出收益最大的50个个体

	//采用选择排序，对所有个体按收益从大到小排列
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

	//把排完序的序列的前50个个体的Kernel、Profit插入到数据库中
	//创建存结果的表与相应字段
	createResultTable();
	//把Kernel_A、Kernel_B单独存两个表，用于导出Kernel分量作图查看分布
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
		
		//存储一行记录，即一个Kernel_A
		rec1["ID1"]= to_string(sumKernel_A[i][0]);
		rec1["ID2"] = to_string(sumKernel_A[i][1]);
		rec1["ID3"] = to_string(sumKernel_A[i][2]);
		rec1["ID4"] = to_string(sumKernel_A[i][3]);
		rec1["ID5"] = to_string(sumKernel_A[i][4]);
		dataSet1.push(rec1);

		//存储一行记录，即一个Kernel_B
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

	//将50条Kernel存入数据库
	databaseName.transInsertNRecord_Data("Kernel_A", dataSet1);
	databaseName.transInsertNRecord_Data("Kernel_B", dataSet2);
}

//训练alpha、beta
bool ProfitSearcher::trainAB(int limitNum, float Rate)
{
	float profit = 1;
	DBConvolution DBConv(databaseName);
	BuyPointer setPoint;
	ProfitComp Profit;

	vector<float> vecAlpha;		//存储alpha
	vector<float> vecBeta;		//存储beta
	vector<float> vecProfit;	//存储profit

	//生成固定的Kernel向量
	vector<float> Kernel_A, Kernel_B;
	int i;
	for (i = 0; i < 10; i++)
	{
		Kernel_B.push_back(0.1);
		if (i < 5)
			Kernel_A.push_back(0.2);
	}

	//训练limitNum次alpha、beta，即跑limitNum次不同alpha、beta下的收益计算
	int num = 0;
	float alpha, beta;
	int isGood = 0;
	srand((unsigned)time(NULL));	//设置随机函数种子
	while (num < limitNum)
	{
		//清空上一次生成的运行结果字段，否则影响下次计算
		clearLastResult();

		if (num == 0)		//令alpha=beta=1
		{
			alpha = 1.0;
			beta = 1.0;
		}

		if (num > 0 && isGood==0)	//若上次收益小于0，则随机生成alpha、beta是0.99到1.01之间的小数
		{
			alpha = 0.99 + 0.02*(rand() % (N + 1) / (float)(N + 1));
			beta = 0.99 + 0.02*(rand() % (N + 1) / (float)(N + 1));
		}

		if (num > 0 && isGood == 1)	//若上次收益大于0，则随机生成的alpha、beta在上次较好的参数附近
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

		//检查并判断是否创建字段RAfield、RBfield
		if (!(createABfiled()))		//判断是否生成了字段RAfield、RBfield
			return false;

		//设置字段RAfield、RBfield
		DBConv.conv(Kernel_A, tableName, closeField, RAfield);
		DBConv.conv(Kernel_B, tableName, closeField, RBfield);

		//设置字段buypoint、sellpoint
		setPoint.setBuySellPoint(databaseName, tableName, RAfield, RBfield, alpha, beta, setBuyPoint, setSellPoint);

		//计算使用当前卷积核下的收益率
		profit = Profit.compProfit(databaseName, intialWealth, tableName, buypointField, sellpointField, closeField, setWealth, setStock, precision);

		//判断这次收益是否是好的收益
		if (profit > 0)
			isGood = 1;
		if(profit <= 0)
			isGood = 0;

		//存储记录
		vecAlpha.push_back(alpha);
		vecBeta.push_back(beta);
		vecProfit.push_back(profit);

		//打印输出
		cout << num << ": the alpha is " << alpha << ", the beta is " << beta << ", the profit is " << profit << endl;

		++num;
	}

	//打印输出最好的alpha、beta
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

//计算最大的收益率
bool ProfitSearcher::RandomConv(int lengthKernel_A, int lengthKernel_B, int initialNumber, int limitNumber, int limtGeneration, float error)
{
	//设置随机函数种子
	srand((unsigned)time(NULL)); 

	//随机生成第一代
	generatePop(lengthKernel_A, lengthKernel_B, 0, initialNumber);

	//两代间的最优收益率之差
	float Gap=10;

	int Gerention = 1;	//繁殖代数
	//Gap<error && 
	while (Gerention < limtGeneration)	 //当繁殖代数超过限制时，繁殖结束
	{
		//生成第Gerention代
		generatePop(lengthKernel_A, lengthKernel_B, Gerention, limitNumber);

		//计算两代间的最优收益率之差
		Gap = abs(maxProfit[Gerention] - maxProfit[Gerention - 1]);

		++Gerention;
	}

	//繁殖结束后，找出所有个体中最优的50个个体
	findBestAgent(limtGeneration);

	//导出结果文件：Result.csv, Kernel_A.csv, Kernel_B.csv
	CSVLoader::exportCSV(databaseName, "Result");
	CSVLoader::exportCSV(databaseName, "Kernel_A");
	CSVLoader::exportCSV(databaseName, "Kernel_B");

	return true;
}