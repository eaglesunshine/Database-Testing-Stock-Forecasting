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

	//ʹ��vectorKernel��vectorProfit�ֱ�洢ÿһ������Agent��Kernel��Profit
	vector<vector<vector<float>>> vectorKernel_A;		//��СԪ���ǵ�i����j��Agent��Kernel_A�ĵ�k������ֵ
	vector<vector<vector<float>>> vectorKernel_B;		//��СԪ���ǵ�i����j��Agent��Kernel_B�ĵ�k������ֵ
	vector<vector<float>> vectorProfit;					//��СԪ���ǵ�i����j��Agent��Profitֵ

	vector<float> maxProfit;							//ÿһ������õ�������
	vector<vector<float>> maxKernel_A;
	vector<vector<float>> maxKernel_B;

	vector<vector<float>> Adaptability;		//��СԪ�ر�ʾ��i����j��Agent����Ӧ��ֵ
	
	vector<float> sumProfit;			//��ȡ���и��������
	vector<vector<float>> sumKernel_A;	//��ȡ���и����Kernel_A
	vector<vector<float>> sumKernel_B;	//��ȡ���и����Kernel_B

	//��ֳ���������и��������ŵ�50������
	vector<vector<float>> bestKernel_A;		
	vector<vector<float>> bestKernel_B;		
	vector<float> bestProfit;				

public:
	ProfitSearcher(ProfitArguments Arguments);
	/*
	*���ܣ����캯�������ʼ��ʱ���������
	*���룺�������Ա�������εĽṹ��Arguments
	*�������
	*/

	//��ȡ������ɵĵ�һ����Ⱥ��Kernel
	vector<float> getKernel_0(int Num, int LOW, int HIGH);

	vector<float> getKernel_1(int Num, int LOW, int HIGH);	
	/*
	*���ܣ�ͨ������������ɵõ���һ����Ⱥ��Kernel
	*���룺����˷�����Ŀ����������������������½�Start����������������������Ͻ�End
	*������������ΪNum������֮��Ϊ1�ľ����
	*/

	int propSelect(int Generation);
	/*
	*���ܣ�ѡ��������������̶Ĳ��ԣ�ʹ�ñ���ѡ��ѡ��һ��Agent
	*���룺������Ⱥ�Ĵ���Generation�����ӵڼ����н���ѡ��
	*�������ѡ�е�Agent���±�
	*/

	vector<float> crossKernel(int Generation, int AorB, int Fa, int Ma);
	/*
	*���ܣ������������Fa��ǰһ����Ma�ĺ�һ����н���ϲ�
	*���룺������Ⱥ�Ĵ���Generation���Ƕ�Kernel_A���滹�Ƕ�Kernel_B����ı�ʶ��AorB����ѡ�е�����Agent���±�Fa��Ma
	*������������õ�Kernel
	*/

	void variateKernel(vector<float> &Kernel);
	/*
	*���ܣ����������Kernel�е��������λ���ϵķ�������
	*���룺��Ҫ���б��������Kernel
	*���������֮���Kernel
	*/

	//��������η�ת������ʵ����������Kλ
	void Reverse(vector<float> &KP, int p, int q);

	//�����������Kernel�е�ĳ�������Ƭ����λ����һ�������λ��
	void insertKernel(vector<float> &Kernel);

	vector<float> getKernel_2(int Generation, int AorB);
	/*
	*���ܣ��Ӹ�����Ⱥ��ѡ������������н��桢����õ��µ�Kernel
	*���룺������Ⱥ�Ĵ���Generation���Ƕ�Kernel_A���滹�Ƕ�Kernel_B����ı�ʶ��AorB
	*��������桢�����õ���Kernel
	*/

	bool createABfiled();
	/*
	*���ܣ���鲢�ж��Ƿ񴴽��ֶ�RAfield��RBfield��û�оʹ���
	*���룺��
	*����������������������ֶξ����true���������false
	*/

	void clearLastResult();
	/*
	*���ܣ������һ�����ɵ����н���ֶ�
	*���룺��
	*���������һ�ε����ɽ������ΪNULL
	*/

	int findMaxProfit(int Generation);
	/*
	*���ܣ���ÿһ�������������ҳ���
	*���룺��ҪѰ������Profit����Ⱥ����Generation
	*�������һ�����������Profit
	*/

	bool generatePop(int lengthKernel_A, int lengthKernel_B, int Generation, int limitNumber);
	/*
	*���ܣ�������ȺPop����һ��
	*���룺�����A�ĳ���lengthKernel_A�������B�ĳ���lengthKernel_B����ֳ����Generation���ڶ������Ժ������ÿ��������limitNumber
	*�����˳����������һ����Ⱥ�����true���������false
	*/

	void createResultTable();
	/*
	*���ܣ�������50�����Ž���ı�����Ӧ�ֶ�
	*���룺��
	*�����������Result
	*/

	void createKernelTable();
	/*
	*���ܣ�������Kernel�ı�����Ӧ�ֶ�
	*���룺��
	*�����������Kernel_A��Kernel_B
	*/

	string transform(vector<float> Kernel);
	/*
	*���ܣ���Kernelת���ɿɲ������ݿ��string����
	*���룺��Ҫת����ʽ��Kernel
	*������ַ������͵�Kernel
	*/

	void findBestAgent(int limtGeneration);
	/*
	*���ܣ���ֳ�������ҳ����и��������ŵ�50������
	*���룺�ܵķ�ֳ����
	*����������ŵ�50����¼��������Result��
	*/

	void adaptCompute(int Generation);
	/*
	*���ܣ�����ÿһ����Ⱥ����Ӧ��ֵ����ת���ɸ���ֵ
	*���룺��Ҫ������Ӧ�ȵ���Ⱥ����Generation
	*�����������һ��ÿ���������Ӧ�Ȳ�����Adaptability��
	*/

	//ѵ��alpha��beta
	bool trainAB(int limitNum, float Rate);

	bool RandomConv(int lengthKernel_A, int lengthKernel_B, int initialNumber, int limitNumber, int limtGeneration, float Error);
	/*
	*���ܣ�������������������
	*���룺�����A�ĳ���lengthKernel_A�������B�ĳ���lengthKernel_B����ʼ��Ⱥ������initialNumber���ڶ������Ժ������ÿ��������limitNumber���ܵķ�ֳ����Generation������֮����������֮��������������
	*������������õ������������
	*/

};