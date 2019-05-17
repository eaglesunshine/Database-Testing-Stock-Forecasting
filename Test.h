#include "Accessor.h"
#include "CSVLoader.h"
#include "DBConvolution.h"
#include "MySQLAccessor.h"
#include "SQLServerAccessor.h"

using namespace std;
using namespace dbaccessor;

//测试类
class Test
{
public:
	Data getNRecordData(string fileName);				
	/*
	*功能：从数据文件中获取随机的N条记录
	*输入：数据文件名
	*输出：存储了N条记录的Data类
	*/

	bool testInsertNRecord(Accessor &databaseName);		
	/*
	*功能：测试增加N条记录是否成功
	*输入：数据库连接名
	*输出：测试通过输出true，失败输出false
	*/

	bool testDeleteNRecord(Accessor &databaseName);		
	/*
	*功能：测试删除N条记录是否成功
	*输入：数据库连接名
	*输出：测试通过输出true，失败输出false
	*/

	bool testUpdateNRecord(Accessor &databaseName);		
	/*
	*功能：测试更新N条记录是否成功
	*输入：数据库连接名
	*输出：测试通过输出true，失败输出false
	*/

};

