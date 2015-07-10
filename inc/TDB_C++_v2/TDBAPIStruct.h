/**
* @defgroup  MAINFRAME 系统资源区
* @author 
* @version 1.0
* @date 2010-2013
* @warning 版权所有，翻录必究
* @{
*/

#ifndef _TDBAPISTRUCT_H_
#define _TDBAPISTRUCT_H_

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64) || defined(_WINDOWS)
#define __PLATFORM_WINDOWS__
#else
#define __PLATFORM_LINUX__
#endif


#ifdef __PLATFORM_LINUX__
#define __int64 long long
#endif


#pragma  pack(push)
#pragma pack(1)
///基本声明与定义////////////////////////////////////////////////////
//TDB句柄

#ifndef THANDLE
typedef void* THANDLE;
#endif

enum TDB_ERROR
{
    TDB_SUCCESS = 0,

    TDB_NETWORK_ERROR=-1,       //网络错误
    TDB_NETWORK_TIMEOUT=-2,     //网络超时
    TDB_NO_DATA = -3,           //没有数据
    TDB_OUT_OF_MEMORY=-4,       //内存耗尽
    TDB_LOGIN_FAILED = -5,      //登陆失败
    TDB_INVALID_PARAMS=-11,     //无效的参数
    TDB_INVALID_CODE_TYPE,      //无效的代码类型，比如向TDB_GetFuture传入非期货类型代码，返回之。
    
    
    TDB_WRONG_FORMULA=-50,      //指标公式错误        
};

enum TDB_PROXY_TYPE
{
	TDB_PROXY_SOCK4,
	TDB_PROXY_SOCK4A,
	TDB_PROXY_SOCK5,
	TDB_PROXY_HTTP11,
};

struct TDB_PROXY_SETTING
{
	TDB_PROXY_TYPE nProxyType;
	char szProxyHostIp[64];
	char szProxyPort[8];
	char szProxyUser[32];
	char szProxyPwd[32];
};

struct OPEN_SETTINGS
{
    char szIP[24];              //服务器ip地址
    char szPort[8];             //服务器端口
    char szUser[32];            //用户名
    char szPassword[32];        //密码

    unsigned int nTimeOutVal;   //获取数据时，指定网络超时（秒数，为0则设为2分钟），若超过nTimeOutVal秒后未收到回应数据包，则内部会关闭连接
    unsigned int nRetryCount;   //获取数据时，若掉线，指定重连次数（为0则不重连），若重连nRetryCount次之后仍掉线，则返回网络错误
    unsigned int nRetryGap;     //掉线之后重连的时间间隔（秒数，为0则设为1秒）
};

//复权方式
typedef enum REFILLFLAG
{
    REFILL_NONE = 0,        //不复权
    REFILL_BACKWARD=1,      //全程向前复权（从现在向过去）
    REFILL_FORWARD=2,       //全程向后复权（从过去向现在）
} ;

typedef enum CYCTYPE
{
    CYC_SECOND,
    CYC_MINUTE,
    CYC_DAY,
    CYC_WEEK,
    CYC_MONTH,
    CYC_SEASON,
    CYC_HAFLYEAR,
    CYC_YEAR,
    CYC_TICKBAR,
};

//买卖方向
typedef enum ORDERSIDE
{
    ORDERSIDE_BID = 0,      //买
    ORDERSIDE_ASK,          //卖
} ;

//TODO

///系统消息结构体////////////////////////////////////////////////////

//登录回报
struct TDBDefine_ResLogin
{
    char szInfo[64];             //信息
    int  nMarkets;               //支持市场个数
    char szMarket[256][8];       //市场标志(SZ, SH, CF, SHF, DCE, CZC,...)
    int  nDynDate[256];          //动态数据日期
};

///基本消息结构体////////////////////////////////////////////////////
struct TDBDefine_Code
{
    char chWindCode[32];        //万得代码(AG1312.SHF)
    char chCode[32];            //交易所代码(ag1312)
    char chMarket[8];           //市场代码(SHF)
    char chCNName[32];          //证券中文名称
    char chENName[32];          //证券英文名称
    int  nType;                 //证券类型
};

///行情消息结构体////////////////////////////////////////////////////
//K线数据 KLine
struct TDBDefine_ReqKLine
{
    char chCode[32];            //证券万得代码(AG1312.SHF)
    REFILLFLAG nCQFlag;         //除权标志：不复权，向前复权，向后复权
    int nCQDate;                //复权日期(<=0:全程复权) 格式：YYMMDD，例如20130101表示2013年1月1日
    int nQJFlag;                //全价标志(债券)(0:净价 1:全价)
    CYCTYPE nCycType;           //数据周期：秒线、分钟、日线、周线、月线、季线、半年线、年线
    int nCycDef;                //周期数量：仅当nCycType取值：秒、分钟、日线、周线、月线时，这个字段有效。
    int nAutoComplete;          //自动补齐：仅1秒钟线、1分钟线支持这个标志，（不为0：补齐；0：不补齐）
    int nBeginDate;             //开始日期(交易日，<0:从上市日期开始； 0:从今天开始)
    int nEndDate;               //结束日期(交易日，<=0:跟nBeginDate一样) 
    int nBeginTime;             //开始时间，<=0表示从开始，格式：（HHMMSSmmm）例如94500000 表示 9点45分00秒000毫秒
    int nEndTime;               //结束时间，<=0表示到结束，格式：（HHMMSSmmm）例如94500000 表示 9点45分00秒000毫秒
};

struct TDBDefine_KLine
{
    char chWindCode[32];            //万得代码(AG1312.SHF)
    char chCode[32];                //交易所代码(ag1312)
    int nDate;                      //日期（自然日）格式：YYMMDD，例如20130101表示2013年1月1日，0表示今天
    int nTime;                      //时间（HHMMSSmmm）例如94500000 表示 9点45分00秒000毫秒
    int    nOpen;                   //开盘((a double number + 0.00005) *10000)
    int    nHigh;                   //最高((a double number + 0.00005) *10000)
    int    nLow;                    //最低((a double number + 0.00005) *10000)
    int    nClose;                  //收盘((a double number + 0.00005) *10000)
    __int64 iVolume;                //成交量
    __int64    iTurover;            //成交额(元)
    int    nMatchItems;             //成交笔数
    int nInterest;                  //持仓量(期货)、IOPV(基金)、利息(债券)
};

struct TDBDefine_Tick
{
    char chWindCode[32];                //万得代码(AG1312.SHF)
    char chCode[32];                    //交易所代码(ag1312)
    int nDate;                          //日期（自然日）
    int nTime;                          //时间（HHMMSSmmm）例如94500000 表示 9点45分00秒000毫秒
    int nPrice;                         //成交价((a double number + 0.00005) *10000)
    __int64 iVolume;                    //成交量
    __int64    iTurover;                //成交额(元)
    int nMatchItems;                    //成交笔数
    int nInterest;                      //IOPV(基金)、利息(债券)
    char chTradeFlag;                   //成交标志
    char chBSFlag;                      //BS标志
    __int64 iAccVolume;                 //当日累计成交量
    __int64    iAccTurover;             //当日成交额(元)
    int nHigh;                          //最高((a double number + 0.00005) *10000)
    int nLow;                           //最低((a double number + 0.00005) *10000)
    int    nOpen;                       //开盘((a double number + 0.00005) *10000)
    int    nPreClose;                   //前收盘((a double number + 0.00005) *10000)

    //下面的字段指数使用
    int        nIndex;                  //不加权指数
    int        nStocks;                 //品种总数
    int        nUps;                    //上涨品种数
    int        nDowns;                  //下跌品种数
    int        nHoldLines;              //持平品种数
};


struct TDBDefine_ReqTick
{
    char chCode[32];    //证券万得代码(AG1312.SHF)
    int  nBeginDate;    //开始日期（交易日）,为0则从今天，格式：YYMMDD，例如20130101表示2013年1月1日
    int  nEndDate;      //结束日期（交易日），为0则和nBeginDate一样
    int  nBeginTime;    //开始时间：若<=0则从头，格式：（HHMMSSmmm）例如94500000 表示 9点45分00秒000毫秒
    int  nEndTime;      //结束时间：若<=0则至最后
};

struct TDBDefine_TickAB
{
    char chWindCode[32];                //万得代码(AG1312.SHF)
    char chCode[32];                    //交易所代码(ag1312)
    int nDate;                          //日期（自然日）格式YYMMDD
    int nTime;                          //时间（HHMMSSmmm）例如94500000 表示 9点45分00秒000毫秒
    int nPrice;                         //成交价((a double number + 0.00005) *10000)
    __int64 iVolume;                    //成交量
    __int64    iTurover;                //成交额(元)
    int nMatchItems;                    //成交笔数
    int nInterest;                      //IOPV(基金)、利息(债券)
    char chTradeFlag;                   //成交标志
    char chBSFlag;                      //BS标志
    __int64 iAccVolume;                 //当日累计成交量
    __int64    iAccTurover;             //当日成交额(元)
    int nHigh;                          //最高((a double number + 0.00005) *10000)
    int nLow;                           //最低((a double number + 0.00005) *10000)
    int    nOpen;                       //开盘((a double number + 0.00005) *10000)
    int    nPreClose;                   //前收盘((a double number + 0.00005) *10000)
    int    nAskPrice[10];               //叫卖价((a double number + 0.00005) *10000)
    unsigned nAskVolume[10];            //叫卖量
    int    nBidPrice[10];               //叫买价((a double number + 0.00005) *10000)
    unsigned nBidVolume[10];            //叫买量
    int    nAskAvPrice;                 //加权平均叫卖价(上海L2)((a double number + 0.00005) *10000)
    int    nBidAvPrice;                 //加权平均叫买价(上海L2)((a double number + 0.00005) *10000)
    __int64  iTotalAskVolume;           //叫卖总量(上海L2)
    __int64  iTotalBidVolume;           //叫买总量(上海L2)

    //下面的字段指数使用
    int        nIndex;                  //不加权指数
    int        nStocks;                 //品种总数
    int        nUps;                    //上涨品种数
    int        nDowns;                  //下跌品种数
    int        nHoldLines;              //持平品种数
};


struct TDBDefine_ReqFuture
{
    char chCode[32];    //证券万得代码(AG1312.SHF)
    int  nBeginDate;    //开始日期（交易日）,为0则从当天，例如20130101
    int  nEndDate;      //结束日期（交易日），小于等于0则和nBeginDate相同
    int  nBeginTime;    //开始时间：若<=0则从头，格式：（HHMMSSmmm）例如94500000 表示 9点45分00秒000毫秒
    int  nEndTime;      //结束时间：若<=0则至最后

    int nAutoComplete;  //自动补齐标志:( 0：不自动补齐，1:自动补齐）
};

struct TDBDefine_FutureAB
{
    char chWindCode[32];                //万得代码(AG1312.SHF)
    char chCode[32];                    //交易所代码(ag1312)
    int nDate;                          //日期（自然日）格式：YYMMDD
    int nTime;                          //时间（精确到毫秒，HHMMSSmmm）
    
    __int64 iVolume;                    //成交量
    __int64    iTurover;                //成交额(元)

    int nSettle;                        //结算价((a double number + 0.00005) *10000)
    int nPosition;                       //持仓量
    int nCurDelta;                      //虚实度

    char chTradeFlag;                   //成交标志
    
    __int64 iAccVolume;                 //当日累计成交量
    __int64    iAccTurover;             //当日成交额(元)
    
    int nOpen;                          //开盘((a double number + 0.00005) *10000)
    int nHigh;                          //最高((a double number + 0.00005) *10000)
    int nLow;                           //最低((a double number + 0.00005) *10000)
    int nPrice;                         //成交价((a double number + 0.00005) *10000)

    int    nAskPrice[5];               //叫卖价((a double number + 0.00005) *10000)
    unsigned int nAskVolume[5];        //叫卖量
    int    nBidPrice[5];               //叫买价((a double number + 0.00005) *10000)
    unsigned int nBidVolume[5];        //叫买量

    int nPreClose;                     //前收盘((a double number + 0.00005) *10000)
    int nPreSettle;                    //昨结算((a double number + 0.00005) *10000)
    int nPrePosition;                  //昨持仓
};

struct TDBDefine_Future
{
    char chWindCode[32];               //万得代码(AG1312.SHF)
    char chCode[32];                   //交易所代码(ag1312)
    int nDate;                         //日期（自然日）格式：YYMMDD
    int nTime;                         //时间，格式：HHMMSSmmm
    
    __int64 iVolume;                   //成交量
    __int64    iTurover;               //成交额(元)

    int nSettle;                       //结算价((a double number + 0.00005) *10000)
    int nPosition;                     //持仓量
    int nCurDelta;                     //虚实度

    char chTradeFlag;                  //成交标志（港股有值）

    __int64 iAccVolume;                //当日累计成交量
    __int64    iAccTurover;            //当日成交额(元)

    int nHigh;                         //最高((a double number + 0.00005) *10000)
    int nLow;                          //最低((a double number + 0.00005) *10000)
    int nOpen;                         //开盘((a double number + 0.00005) *10000)
    int nPrice;                        //成交价((a double number + 0.00005) *10000)

    int nPreClose;                     //前收盘((a double number + 0.00005) *10000)
    int nPreSettle;                    //昨结算((a double number + 0.00005) *10000)
    int nPrePosition;                  //昨持仓
};


//逐笔成交数据 Transaction
struct TDBDefine_ReqTransaction
{
    char chCode[32];            //证券万得代码(AG1312.SHF)
    int  nBeginDate;            //开始日期（交易日），格式YYMMDD
    int  nEndDate;              //数据日期（交易日）小于等于0和nBeginDate相同
    int  nBeginTime;            //开始时间:<=0表示从0开始，格式：HHMMSSmmm
    int  nEndTime;              //结束时间：<=0表示到最后
};

struct TDBDefine_Transaction
{
    char    chWindCode[32];     //万得代码(AG1312.SHF)
    char    chCode[32];         //交易所代码(ag1312)
    int     nDate;              //日期（自然日）格式:YYMMDD
    int     nTime;              //成交时间(精确到毫秒HHMMSSmmm)
    int     nIndex;             //成交编号(从1开始，递增1)
    char    chFunctionCode;     //成交代码: 'C', 0
    char    chOrderKind;        //委托类别
    char    chBSFlag;           //BS标志
    int     nTradePrice;        //成交价格((a double number + 0.00005) *10000)
    int     nTradeVolume;       //成交数量
    int     nAskOrder;          //叫卖序号
    int     nBidOrder;          //叫买序号
};

//逐笔委托数据 Order (SZ Level2 Only) 
typedef struct TDBDefine_ReqTransaction TDBDefine_ReqOrder ;

struct TDBDefine_Order
{
    char chWindCode[32];        //万得代码(AG1312.SHF)
    char chCode[32];            //交易所代码(ag1312)
    int  nDate;                 //日期（自然日）格式YYMMDD
    int  nTime;                 //委托时间（精确到毫秒HHMMSSmmm）
    int  nIndex;                //委托编号，从1开始，递增1
    int  nOrder;                //交易所委托号
    char chOrderKind;           //委托类别
    char chFunctionCode;        //委托代码, B, S, C
    int  nOrderPrice;           //委托价格((a double number + 0.00005) *10000)
    int  nOrderVolume;          //委托数量
};

//委托队列数据 OrderQueue
typedef struct TDBDefine_ReqTransaction TDBDefine_ReqOrderQueue;

struct TDBDefine_OrderQueue
{
    char    chWindCode[32];         //万得代码(AG1312.SHF)
    char    chCode[32];             //交易所代码(ag1312)
    int     nDate;                  //日期（自然日）格式YYMMDD
    int     nTime;                  //订单时间(精确到毫秒HHMMSSmmm)
    int     nSide;                  //买卖方向('B':Bid 'A':Ask)
    int     nPrice;                 //成交价格((a double number + 0.00005) *10000)
    int     nOrderItems;            //订单数量
    int     nABItems;               //明细个数
    int     nABVolume[50];          //订单明细
};

struct TDBDefine_AddFormulaRes
{
    int nErrLine;           //错误行
    char chText[132];       //错误行指标公式正文
    char chInfo[68];        //错误信息
};

struct TDBDefine_FormulaItem
{
    char chFormulaName[28];    //指标公式名称
    char chParam[132];         //参数
};

struct TDBDefine_DelFormulaRes
{
    char chFormularName[28];    //删除的指标名称
    char chInfo[68];            //删除信息
};

struct TDBDefine_CalcFormulaRes
{
    char chWindCode[32];        //万得代码(AG1312.SHF)
    char chCode[32];            //交易所代码(ag1312)

    int nRecordCount;           //记录数量
    int nFieldCount;            //字段数量，最多50个

    char chFieldName[50][28];   //字段名称，
    int* dataFileds[50];        //最多50个字段，如果有日期和时间字段，总是排在前面
};


//请求计算指标公式
struct TDBDefine_ReqCalcFormula
{
    char chFormulaName[28]; //公式名称
    char chParam[32];       //参数, "N1=5,N2=10, N3='000001'
    char chCode[32];        //万得代码，形如：AG1312.SHF

    CYCTYPE  nCycType;      //数据周期
    int  nCycDef;           //数据周期定义

    REFILLFLAG  nCQFlag;    //除权标志
    int nBondDataType;      //债券全价标志

    int  nCQDate;           //除权日期
    int  nCalcMaxItems;     //计算的最大数据量
    int  nResultMaxItems;   //传送的结果最大数据量
};

#pragma pack(pop)

#endif//_TDBAPISTRUCT_H_
