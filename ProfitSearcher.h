#include "Accessor.h"
#include "DBConvolution.h"
#include "BuyPointer.h"
#include "ProfitComp.h"
#include "profitArguments.h"

class ProfitSearcher
{
	Accessor &databaseName;
	string tableName;
	string RAfield;
	string RBfield;
	float alpha;
	float beta;
	bool setBuyPoint;
	bool setSellPoint;

	float intialWealth;
	string buypointField;
	string sellpointField;
	string closeField;
	bool setWealth=true;
	bool setStock=true;
	int precision=6;

	//使用vectorKernel、vectorProfit分别存储每一代各个Agent的Kernel、Profit
	vector<vector<vector<float>>> vectorKernel_A;		//最小元素是第i代第j个Agent的Kernel_A的第k个分量值
	vector<vector<vector<float>>> vectorKernel_B;		//最小元素是第i代第j个Agent的Kernel_B的第k个分量值
	vector<vector<float>> vectorProfit;					//最小元素是第i代第j个Agent的Profit值

	vector<float> maxProfit;							//每一代中最好的收益率
	vector<vector<float>> maxKernel_A;
	vector<vector<float>> maxKernel_B;

	vector<vector<float>> Adaptability;		//最小元素表示第i代第j个Agent的适应度值
	
	vector<float> sumProfit;			//读取所有个体的收益
	vector<vector<float>> sumKernel_A;	//读取所有个体的Kernel_A
	vector<vector<float>> sumKernel_B;	//读取所有个体的Kernel_B

	//繁殖结束后，所有个体中最优的50个个体
	vector<vector<float>> bestKernel_A;		
	vector<vector<float>> bestKernel_B;		
	vector<float> bestProfit;				

public:
	ProfitSearcher(ProfitArguments Arguments);
	/*
	*功能：构造函数，类初始化时负责传入参数
	*输入：负责给成员变量传参的结构体Arguments
	*输出：无
	*/

	//获取随机生成的第一代种群的Kernel
	vector<float> getKernel_0(int Num, int LOW, int HIGH);

	vector<float> getKernel_1(int Num, int LOW, int HIGH);	
	/*
	*功能：通过随机函数生成得到第一代种群的Kernel
	*输入：卷积核分量数目，分量的随机数生成区间下界Start，分量的随机数生成区间上界End
	*输出：输出长度为Num，分量之和为1的卷积核
	*/

	int propSelect(int Generation);
	/*
	*功能：选择操作：采用轮盘赌策略，使用比例选择法选出一个Agent
	*输入：父代种群的代数Generation，即从第几代中进行选择
	*输出：被选中的Agent的下标
	*/

	vector<float> crossKernel(int Generation, int AorB, int Fa, int Ma);
	/*
	*功能：交叉操作：将Fa的前一半与Ma的后一半进行交叉合并
	*输入：父代种群的代数Generation，是对Kernel_A交叉还是对Kernel_B交叉的标识符AorB，被选中的两个Agent的下标Fa和Ma
	*输出：交叉过后得到Kernel
	*/

	void variateKernel(vector<float> &Kernel);
	/*
	*功能：变异操作：Kernel中的随机两个位置上的分量互换
	*输入：需要进行变异操作的Kernel
	*输出：变异之后的Kernel
	*/

	//经典的三次反转方法：实现数组右移K位
	void Reverse(vector<float> &KP, int p, int q);

	//插入操作：将Kernel中的某个随机的片段移位到另一个随机的位置
	void insertKernel(vector<float> &Kernel);

	vector<float> getKernel_2(int Generation, int AorB);
	/*
	*功能：从父代种群中选出两个个体进行交叉、变异得到新的Kernel
	*输入：父代种群的代数Generation，是对Kernel_A交叉还是对Kernel_B交叉的标识符AorB
	*输出：交叉、变异后得到新Kernel
	*/

	bool createABfiled();
	/*
	*功能：检查并判断是否创建字段RAfield、RBfield，没有就创建
	*输入：无
	*输出：若表中生成了两个字段就输出true，否则输出false
	*/

	void clearLastResult();
	/*
	*功能：清空上一次生成的运行结果字段
	*输入：无
	*输出：将上一次的生成结果更新为NULL
	*/

	int findMaxProfit(int Generation);
	/*
	*功能：把每一代的最优收益找出来
	*输入：需要寻找最优Profit的种群代数Generation
	*输出：这一代个体的最优Profit
	*/

	bool generatePop(int lengthKernel_A, int lengthKernel_B, int Generation, int limitNumber);
	/*
	*功能：生成种群Pop的新一代
	*输入：卷积核A的长度lengthKernel_A，卷积核B的长度lengthKernel_B，繁殖代数Generation，第二代及以后代数的每代个体数limitNumber
	*输出：顺利创建完新一代种群就输出true，否则输出false
	*/

	void createResultTable();
	/*
	*功能：创建存50条最优结果的表与相应字段
	*输入：无
	*输出：创建表Result
	*/

	void createKernelTable();
	/*
	*功能：创建存Kernel的表与相应字段
	*输入：无
	*输出：创建表Kernel_A和Kernel_B
	*/

	string transform(vector<float> Kernel);
	/*
	*功能：将Kernel转换成可插入数据库的string类型
	*输入：需要转换格式的Kernel
	*输出：字符串类型的Kernel
	*/

	void findBestAgent(int limtGeneration);
	/*
	*功能：繁殖结束后，找出所有个体中最优的50个个体
	*输入：总的繁殖代数
	*输出：将最优的50条记录插入结果表Result中
	*/

	void adaptCompute(int Generation);
	/*
	*功能：计算每一代种群的适应度值，并转换成概率值
	*输入：需要计算适应度的种群代数Generation
	*输出：计算这一代每个个体的适应度并存入Adaptability中
	*/

	//训练alpha、beta
	bool trainAB(int limitNum, float Rate);

	bool RandomConv(int lengthKernel_A, int lengthKernel_B, int initialNumber, int limitNumber, int limtGeneration, float Error);
	/*
	*功能：收益搜索器的主函数
	*输入：卷积核A的长度lengthKernel_A，卷积核B的长度lengthKernel_B，初始种群个体数initialNumber，第二代及以后代数的每代个体数limitNumber，总的繁殖代数Generation，两代之间最优收益之差的误差跳出条件
	*输出：输出计算得到的最大收益率
	*/

};