// TDFUnSave.h : TDFUnSave DLL 的主头文件
//

#pragma once

//#include <stdint.h>
#include <assert.h>
#include "HHTradeDataSetDefine.h"
#ifndef _WIN32
#include "TDCore.h"
#else
typedef long long int64_t;
#endif
typedef signed char int8_t;
typedef short int16_t;
typedef int int32_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

#pragma pack(push, 8)

#define MARKETOFFSET	(64*1024)
//成交标志定义
#define HHMATCHMONEY_HI  0xf0000000		//成交金额高位
#define HHMATCHVOLUME_HI 0x0f000000		//成交量高位
#define HHBSFLAG_B		 0x00800000		//买卖标志B
#define HHBSFLAG_S		 0x00400000		//买卖标志S
#define HHUDFLAG_U		 0x00200000		//升跌标志Up
#define HHUDFLAG_D		 0x00100000		//升跌标志Down

// Tick数据类型定义:
#define ID_TKTYPE_VIEWDATA     -3     // 浏览数据
#define ID_TKTYPE_INFODATA     -2     // 资料数据（ETF清单等）
#define ID_TKTYPE_DAY          -1     // Day
#define ID_TKTYPE_TICK         0      // Tick
#define ID_TKTYPE_MINUTE       1      // One Minute
#define ID_TKTYPE_MATCH        2      // Only Match Tick
#define ID_TKTYPE_STEP         3      // Step
#define ID_TKTYPE_ORDERQUEUE   4      // Order Queue of Top 50 for SHSE & SZSE Level2 Data
#define ID_TKTYPE_ORDER        5      // Order
#define ID_TKTYPE_BRKOERQUEUE  6      // Broker Queue of Best 40 for HK
#define ID_TKTYPE_LINKED       7      // Linked 关联品种
#define ID_TKTYPE_OTC_OPTIONS_S 8      // 场外期权快照

// 代码信息定义
#define TDCODE_UNICODE_CODE      0x01 // 代码为Unicode编码
#define TDCODE_UNICODE_NAME      0x02 // 名称为Unicode编码
#define TDCODE_UNICODE_NAMEENG   0x04 // 英文名称为Unicode编码
//数据处理标志定义(iHandlFlag)
#define TD_HANDLE_MINUTE     0x01
#define TD_HANDLE_MATCH_TICK 0x02
#define TD_HANDLE_NO_KDAY    0x04
#define TD_HANDLE_ONLY_ONE_RECORD  0x08
#define TD_HANDLE_INIT_FLAG        0x10
#define TD_HANDLE_ONLY_TIME_DIF    0x20
//快照事件标志定义（nEventFlags）
#define TD_SP_STATUS_CLEAR     0x01
#define TD_SP_STATUS_NOT_TRADE 0x02
#define TD_SP_STATUS_TRANS     0x04
//事件编号
#define ID_EVENT_OPEN  1  //开盘
#define ID_EVENT_CLEAR 2  //清盘
#define ID_EVENT_CLOSE 3  //闭市
#define ID_EVENT_TRANS 4  //转述
#define ID_EVENT_SAVE  5  //落盘
#define ID_EVENT_CLEAR_CODES              6  //部分代码清盘
#define ID_EVENT_SINGLE_CODE_CLEAR_NOTTIY 7  //单个代码清盘通知
#define ID_EVENT_HEARTBETA                8  //整个市场行情心跳
#define ID_EVENT_CODE_HEARTBETA           9  //单个代码心跳
#define ID_EVENT_TRANS_CODES              10 //部分代码转述
#define ID_EVENT_VSS_HEARTBETA            11 //转码机固定时间发送心跳？
#define ID_EVENT_CANCEL                   12 //撤销事件
/*struct TickHead{
	int bitMap[5];
	int nType;
	int64_t iPreClose;
	int64_t iPreVolume;
	int64_t nHighLimit;
	int64_t nLowLimit;
	int nTimeDif;
	int nExchItems;
	char  chExchTimeFlags[8];
	short nBeginTime[8];
	short nEndTime[8];
	int nPreSettlement;
	int nPrePosition;
	int nPreDelta;
	int chDec;
	int resv[20];
};*/
#define VOLUME_BACKWARD 0x01

struct Tick_Ext_Head{
	int nChangeCount;
	int nChangeIndex[4];
	int64_t iPreclose[4];
	int nResv[10];//预留40字节
};
struct TDFMarketExchangeSet
{ 
	char   chExchangeName[32];    // 交易所全称
	char   chExchangeNameCJ[4];   // 交易所简称1
	char   chExchangeNameCJ2[12]; // 交易所简称2
	char   chMarket[4];           // 交易所标志:SZ;SH;CF;DCE;SHF;CZC;HK;HKF;O;N
	int    nMarketID;             // 市场ID
	int    nFlags;                // 标志位（保留）: VOLUME_BACKWARD:是否容许量回退
	int    nMDFlags;              // MarketData数据处理标志(0:委托队列同MD同时更新，1:分置更新)
	int    nAskBidDeeps;          // 买卖盘个数
	int    nAskBidFlags;          // 买卖盘标志(0x80:有订单数量)
	int    nDataLevel;            // 数据级别
	int    nTestFlag;			  // 测试/仿真数据标志
    int    nExchItems;            // 交易时间节数
    int    nTimeDif;              // 时差（分钟）
    short  nBeginTime[8];         // 交易节开始时间（分钟数）
    short  nEndTime[8];           // 交易节结束时间（分钟数）
	char   chExchTimeFlags[8];
};



//代码
struct TD_Code_Packet
{
    char chCode[32];			// 交易所代码： 交易所代码如果和Wind代码前面部分不一致，需改为一致
	char chWindCode[36];		// Wind代码
    char chWindType[8];			// Wind类型
    char chName[48];			// 名称
    char chNameEng[48];			// 英文名称
    int  nRecord;				// 当日编号
    int  nType;					// 证券类型
    int  nLotSize;				// 交易量纲
    int  nQuoteUnit;			// 20170320 Add:报价单位（价差）（元，定点4位小数）- 和当前成交价匹配
    int  nID;					// ID
    GUID guid;					// GUID
    char chDec;					// 显示小数位
	char chVol;					// 显示量纲(10的次方,支持1,2,3)
	char chUnicodeFlag;			// Unicode标志[TDCODE_UNICODE_](0x01:chCode, 0x02:chName, 0x04: chNameEng)
    char chCurrencyCode[4];     // 货币
	char chQualificationClass;  //对于非深交所债券，默认值填0.
};

union TD_Info
{	
    //股票
    struct TD_Info_Shares
    {
        char chIndustryCode[4];              // 所属行业代码
        int64_t iCapitalShares;              // 流通股本
        int64_t iTotalShares;                // 总股本
		char chProfitFlag;                   // 表示“目前是否尚未盈利”，0:未盈利，1:已盈利
		char chWeightedVotingRights;         // 表示“是否存在投票权差异”，0: 不存在，1: 存在
    }Shares;
    //权证
    struct TD_Info_Warrants
    {
        char chUnderlyingCode[8];            // 标的股票代码
        char chStrikeCode[8];                // 行权代码
        char chCPFlag;                       // 购/沽标志
        char chType;                         // 权证类型(美式/欧式)
        char chStrSet;                       // 结算方式
        char chResv;
        int  nExercisePrice;                 // 行权价格
        int  nExerciseRation;                // 行权比例
        int  nExercisePeriod;                // 行权日期
        int64_t iTotalWarrantExecQty;        // 权证执行的总数量(lVolumeInMarket港股街货量)
        unsigned int nTotalWarrantCancelQty; // 权证注销总数量
        unsigned int nTotalWarrantOrderQty;  // 权证创设总数量
        int  nCallPrice;                     // 回收价格(港股牛熊证)
        int  nMaturityDate;                  // 到期
    }Warrants;
    //债券
    struct TD_Info_Bond
    {
        int  nNominalValue;          // 面值
        int  nIssuingPrice;          // 发行价
        int  nCouponRate;            // 票面利率(可转债:首年利率) 
        int  nCurrentRate;           // 当年利率
        int  nPaymentFrequency;      // 付息频率
        int  nMaturityDate;          // 到期日
        int  nPaymentDate;           // 付息日
        int  nValueDate;             // 起息日
        int  nResidualMaturity;      // 剩余时间
        //可转债券数据
        char chConversionCode[8];    // 转股代码			  
        int  nConversionPrice;       // 转股价格
        int  nConversionRation;      // 转股比例
        int  nConversionPeriod;      // 转股时间
		int  nIssueDate;             // 发行日
		uint16_t nBidSettlementSpeed;  //买入清算速度 —— 取值1表示“T+0”，取值2表示“T+1”
		uint16_t nAskSettlementSpeed;  //卖出清算速度 —— 取值1表示“T+0”，取值2表示“T+1” 
    }Bond;
    //期货
    struct TD_Info_Futures
    {
        char chUnderlyingCode[8];   // 标的股票代码
        int  nListDate;             // 上市日期
        int  nEndDate;              // 最后结算日
        int  nExchID;               // 交易所ID
        int  nAskDelta;             // 叫卖价差
        int  nBidDelta;             // 叫买价差
        char chNearCode[8];         // 近期代码
        char chFarCode[8];          // 远期代码
        int  nNearEndDate;          // 近期结束日期
        int  nFarEndDate;           // 远期结束日期
    }Futures;
	struct TD_Info_Options				//2013.10.23 add by Paul for ShangHai Option
	{
		char chContractID[20];          // 期权合约代码C19
		//char chSecuritySymbol[20];	// 期权合约简称
		char chUnderlyingSecurityID[8]; // 标的证券代码
		char chUnderlyingType;			// 标的证券类型C3    0-A股 1-ETF (EBS – ETF， ASH – A 股)
		char chOptionType;              // 欧式美式C1        若为欧式期权，则本字段为“E”；若为美式期权，则本字段为“A”
		char chCallOrPut;               // 认购认沽C1        认购，则本字段为“C”；若为认沽，则本字段为“P”
		char chPriceLimitType;          // 涨跌幅限制类型C1 ‘N’表示有涨跌幅限制类型, ‘R’表示无涨跌幅限制类型
		int  nContractMultiplierUnit;	// 合约单位,         经过除权除息调整后的合约单位, 一定是整数
		int  nExercisePrice;            // 期权行权价,       经过除权除息调整后的期权行权价，右对齐，精确到厘
		int  nStartDate;                // 期权首个交易日,YYYYMMDD
		int  nEndDate;                  // 期权最后交易日/行权日，YYYYMMDD
		int  nExerciseDate;             // 期权行权日，YYYYMMDD
		int  nExpireDate;               // 期权到期日，YYYYMMDD
	}Options;
};

//2008.11.30将关联品种数据扩展为压缩数据
struct TD_Links_Packet
{
    int  nLinks;          // 关联品种个数
    int  nLinksLen;       // 关联品种压缩数据长度
    char chCode[4000];
};

struct TD_BBQ_ASKBID
{
	int   nPrice;                
	int   nVolume;           
	char  chPriceStatus;
	char  chIsBidOrAsk;
	char  chSpecialFlag;
	char  chStatus;
};

//扩展买卖盘
union TD_AB_Ext
{
    struct TD_AB_Sec
    {
        int nIEP;
        int nIEV;
        int nAskDelta;      // 委卖价格差
        int nBidDelta;      // 委买价格差
    }Sec;
    struct TD_AB_Futures
    {
        int nSettlement;    // 结算价
        int nPositions;     // 持仓量
        int nCurDelta;      // 虚实度
        int nUnderlingPrice;// 标的品种价格
    }Futures;
	struct TD_AB_Bonds
	{
		int nSettlement;    // 结算价
	}Bond;
};

//买卖盘
union TD_AskBid
{
    struct TD_AB_Sec
    {
        union TD_AB_Ext Ext;
        
        int64_t nAskAvPrice;    // 委卖加权均价
        int64_t nBidAvPrice;    // 委买加权均价
        int64_t iAskTotalVol;   // 委卖总量
        int64_t iBidTotalVol;   // 委买总量
        
        int64_t nAskPrice[10];  // 委卖价格
        int64_t nBidPrice[10];  // 委买价格
        int64_t iAskVol[10];    // 委卖数量
        int64_t iBidVol[10];    // 委买数量
        int     nAskOrders[10]; // 委卖订单数量
        int     nBidOrders[10]; // 委买订单数量
    }Sec;
	struct TD_AB_Index
	{
		unsigned int preSZ;
		unsigned int nZgb;		// 总股本
		unsigned int nKlt;		// 可流通
		unsigned int nLtg;		// 流通股
		unsigned int nFxl;		// 发行量
		int bbzsy;				// 报表总收益
		int bbzzc;				// 报表总资产
		int bbjzc;				// 报表净资产
		int bbxjll;				// 报表现金流量
		int bbgdsl;				// 股东数量
		int lastmgsy;			// 最新报告每股收益
		int lastseason;			// 最新报告季度
		int laststocks;			// 最新报告个数

		int	nStocks;			// 品种个数
		int	nZDP[3];			// 涨跌平
		uint32_t	nFxsz;	// 发行股本市值
		uint32_t	nLtsz;	// 流通股市值
		uint32_t	nKltsz;	// 可流通股市值
		uint32_t	nZsz;	// 总市值

		int mmb;

		int64_t nAvPrice;			// 算术平均价
		int64_t nIndex;				// 不加权指数
		int nAskMoney;			// 委卖金额
		int nBidMoney;			// 委买金额

		char chFirstCode[5][8];	// 领涨股
		char chLastCode[5][8];	// 领跌股
		int  nFirstMarkup[5];
		int  nLastMarkup[5];
		//新加字段
		int nTransactionsNum;//成交家数
		int nMaxIncrease; //最大涨幅
		int nMaxDrawDown; //最大跌幅
		//20170727:添加
		int64_t iPreSettlement;
		int64_t iSettlement;
		int64_t iPrePosition;
		int64_t iPosition;
	}Indexs;
	struct TD_NoSSE_Bond{
		char    chBidQuoteOrg[32];   //买报价方
		char    chAskQuoteOrg[32];   //卖报价方
		int     nBidCleanPrice;      //最优买入净价
		int     nAskCleanPrice;      //最优卖出净价
		int     nBidYield;           //最优买入到期收益率
		int     nAskYield;           //最优卖出到期收益率
		int64_t iBidTradeVolume;     //最优买入报价量
		int64_t iAskTradeVolume;     //最优卖出报价量
		int     nBidTotalPrice;      //最优买入全价
		int     nAskTotalPrice;      //最优卖出全价
	}NSSEBond;
	struct TD_BBQ_Bond
	{
		TD_BBQ_ASKBID stAsk;
		TD_BBQ_ASKBID stBid;
		char aucInfo[420];
	}BBQBond;
	struct TD_ETF_IOPV
	{
		uint32_t	IOPV_ask0;
		uint32_t	IOPV_bid0;
		uint32_t	IOPV_ask1;
		uint32_t	IOPV_bid1;
		uint32_t	IOPV_aska;
		uint32_t	IOPV_bida;
	}ETF_IOPV;
};

//扩展数据//////////////////////////////////
union TD_Exdata
{
	struct TD_Exdata_Shares
	{
		int nPERation1;         // 市盈率1
		int nPERation2;         // 市盈率2
		int nStockDividend;     // 股息率
		int nTime;              //盘前/盘后时间		
		int nBeforeVolume;      //盘前量
		int nAfterVolume;		//盘后量
		int64_t iBeforPrice;    //盘前价格
		int64_t iAfterPrice;    //盘后价格
		int64_t iAfterTurnover;	//盘后成交金额 (科创板有使用到)
		int nAfterMatchItems;   //盘后成交笔数
	}Shares;
	struct TD_Exdata_Indexs
	{
		int nPERation1;           // 市盈率1
		int nPERation2;           // 市盈率2
		int64_t iExtTurnover;     // 指数扩展额，目前用于SZ成指填写综指数据，如无，需保持和正常额量一直
		int64_t iExtVolume;       // 指数扩展量，目前用于SZ成指填写综指数据，如无，需保持和正常额量一直
	}Indexs;
	struct TD_Exdata_ETF
	{
		int nNetValue;          // 净值
		int nIOPV;              // IOPV 净值估值(EFT)
		int nETFBuyNumber;       // ETF申购笔数
		int nETFSellNumber;      // ETF赎回笔数
		int64_t iETFBuyAmount;   // ETF申购数量
		int64_t iETFBuyMoney;    // ETF申购金额
		int64_t iETFSellAmount;  // ETF赎回数量
		int64_t iETFSellMoney;   // ETF赎回金额
	}ETF;
	struct TD_Exdata_Bond
	{
		int nAccruedInterestAmount;   // 应付利息额
		int nYieldToMaturity;         // 到期收益率 - 最新收益率  259
		int nTotalYieldToMaturity;    // 到期总收益率
		int nBondGage;                // 债券估值
		//20160712:外汇交易中心缺少字段
		int nPreWeightedCleanPrice;   //前加权平均净价 313
		int nWeightedCleanPrice;      //加权平均净价 310	
		int nPreClosingYield;         //前收盘收益率 511
		int nOpeningYield;            //开盘收益率 316
		int nHighestYield;            //最高收益率 318
		int nLowestYield;             //最低收益率 317	
		int nPreWeightedYield;        //前加权平均收益率
		int nWeightedYield;           //加权平均收益率 512
		int nBidYield;                //买方收益率
		int nAskYield;                //卖方收益率
	}Bond;
	struct TD_Exdata_Futures
	{
		int nPreSettlement;// 前日结算价
		int nPrePosition;  // 前日持仓量
		int nPreDelta;     // 前日虚实度
		int	nAvgPrice;	   //均价 TODO:换成时间
		int nDayVolume;        //日间量
		int nNightVolume;	   //夜间量
		int64_t iDayPrice;     //日间价格
		int64_t iNightPrice;   //夜间价格	
		int64_t iEFPVolume;
		int     nEFPPositions; 
	}Futures;
	struct TD_Exdata_FX
	{
		int64_t nVAskPricePoint;       // 卖出掉期点
		int64_t nVBidPricePoint;       // 买入掉期点
		int nVBidTime;                 // 买入价时间 HHMMSS
		int nPriceFlag;                // 保留
		int nForwardLatestFullPrice;   // 远期最新全价 257
		int nSwapDate;                 // 掉期到期日 324
		int nVAskTime;                 // 卖入价时间 HHMMSS
		int nResv;                     // 保留字段
		int64_t iDayPrice;             // 外汇交易中心日盘价
	}FX;
	struct TD_Exdata_IR  //interest rate
	{
		int  nWeightedInterestRate;  // 加权平均利率
		int  nPreWeightedInterestRate;  //前加权平均利率
	}IR;
	struct TD_Exdata_UserDefOptions
	{
		int64_t dUnderlyingPrice;		// 现价
		int nChange;		       // 涨跌
		int nOptionBalance;		// 权利金
		int nBailBalance;		// 保证金
		int nMaxIncome;		 // 最大收益
		int nMaxLoss;		    // 最大亏损
		int nDelta;		        // delta
		int nGamma;		        // gamma
		int nTheta;		        // theta
		int nVega;		        // vega
		int nRho;		        // rho
	}UserDefOptions;
};

//委托队列////////////////////////////////////////
struct TD_OrderQueue_Packet
{
    int nAskTime;       // 叫卖时间（秒）
    int nAskOrders;     // 叫卖订单数
	int64_t nAskPrice;  // 叫卖价格
    int nAskDetail[50]; // 第一叫卖前50笔
    int nBidTime;       // 叫买时间（秒）
    int nBidOrders;     // 叫买订单数
	int64_t nBidPrice;  // 叫买价格
    int nBidDetail[50]; // 第一叫买前50笔
};

//经纪队列(HK)/////////////////////////////////////
struct TD_BrokerQueue_Packet
{
    int   nAskTime;       // 叫卖时间（秒）
    int   nBidTime;       // 叫买时间（秒）
    int   nAskBrokers;    // 叫卖经纪个数
    int   nBidBrokers;    // 叫卖经纪个数
    short sAskBroker[40]; // 叫卖前40经纪
    short sBidBroker[40]; // 叫买前40经纪
};

//逐笔成交//////////////////////////////////////////
struct TD_Step_Packet
{
    int  nTime;          // 成交时间（秒）
    int  nMilliSecond;	 // 成交时间（毫秒）
    int64_t nPrice;         // 成交价
    int  nVolume;        // 成交量
    int  nTurnover;      // 成交金额
    int  nAskOrder;      // 叫卖委托序号
    int  nBidOrder;      // 叫买委托序号
	int  nOrder;         // 逐笔成交序号(20160104 add)
    char chTradeFlag;    // 交易标志(2012.2.14改为单纯用于保存BSFlag:1=B,2=S,0=不明方向)
    char chOrderKind;    // 成交类别
    char chFunctionCode; // 成交代码
};

//逐笔委托///////////////////////////////////////////
struct TD_Order_Packet
{
    int  nTime;          // 委托时间（秒）
    int  nMilliSecond;   // 委托时间(毫秒)
    int64_t nPrice;      // 委托价格
    int  nVolume;        // 委托量
    int  nOrder;         // 委托序号
	int  nBroker;        //经纪商编码
    char chOrderKind;    // 委托类别	
    char chFunctionCode; // 委托代码
	char chStatus;       //委托状态
	char chFlag;         //标志
	//20160616(nzhang): 添加nBroker和chStatus和chFlag；此类型包长度最多32个字节，目前已经到最大值，不能继续添加字段！
};

//分钟数据////////////////////////////////////////////
struct TD_Minute_Packet
{
    int nDate;             // 日期(2013.8.8 add by Paul 增加上期夜盘支持)
    int nMinute;           // 时间(分钟)
    int64_t nOpen;             // 开盘
    int64_t nHigh;             // 最高
    int64_t nLow;              // 最低
    int64_t nClose;            // 收盘
    int nMatchItems;       // 成交笔数
    int64_t	iVolume;       // 成交量(交易所原始数据)	
    int64_t	iTurnover;     // 成交金额(注:指数为成交金额为100元)
    int nABRate;           // 委比
    int nBSRate;           // 买卖比
    union TDFDynData_Minute
    {
        struct TDFDynData_Minute_Index
        {
            int nIndex;    // 不加权指数
            int nUps;      // 涨
            int nDowns;    // 跌
            int nLevels;   // 平
        }Index;
        struct TDFDynData_Minute_Fund
        {
            int nNetValue;	// 动态净值
        }Fund;
        struct TDFDynData_Minute_Futures
        {
            int nPositions;	// 持仓量
            int nSettlement;// 结算价
            int nCurDelta;	// 虚实度
        }Futures;
    }ExtData;
};

//委托队列////////////////////////////////////////////
union TD_OrderQueue
{
	TD_OrderQueue_Packet  Order;      // 委托队列
	TD_BrokerQueue_Packet Broker;	  // 经纪商队列(HK)
};

//
struct TD_Match_Packet
{
	int		nDate;        // 成交日期
	int     nTime;        // 成交时间
	int		nMilliSecond; // 成交时间（毫秒）
	int     nMatchItems;  // 成交笔数
	int64_t nPrice;		  // 成交价
	int     nPositions;   // 持仓量
	int     nSettlement;  // 结算价
	int64_t iVolume;	  // 成交量
	int64_t iTurnover; 	  // 成交金额
	char	chTradeFlag;  // 交易标志
	char	chBSFlag;	  // BS标志
	char    chResv[2];
};

// 文件，如ETF清单
struct TD_File_Packet
{
	int   nDataLen;
	int   nDate;
	int   nTime;
	char  chType;
	char  chFileType[16];
	char  chFileName[_MAX_PATH];
	char  pData[1];
};

//数据头//////////////////////////////////////////////
struct TDMarketData_Head
{
	char  chTDRemark[32];
	int   nHeadSize;
    int   nMagicCode;         // 标示码
	int   nFlags;             // 数据标志 
	int   nStatus;            // 数据状态（0:正常处理 1:清盘中 2:历史数据转换中 3:历史数据转换完毕）
	int   nDataVersion;       // 数据版本
	int   nLoadTimes;         // 
	///////////////////////////////////////////////////
    int   nMarketID;          // 市场ID
    char  chMarket[4];        // 市场标志
    char  chMarketName[32];   // 交易所名称
    char  chMarketNameEng[48];// 英文名称
    int   nDataLevel;         // 数据基本(Level1、Level2)
	int   nTestFlag;          // 测试数据
    int   nAskBidDeeps;       // 报价个数
    int   nAskBidFlags;       // 报价标志 0x80:有订单数量
    int   nExchItems;         // 交易时间节数
    int   nTimeDif;           // 时差（该市场的交易时段所用的时区当天和东八区之间时间差的分钟数。如美国东部时区为-5，交易所采用夏令时，在夏天美国的交易所timediff填”-720”((-5+1-8)*60=-720)） 
							  //2016/10/19修改为： 低2个字节和现在一样，表示该行情的时间(包含夏令时)和东八区的时间差值。从低位起第三个字节表示该行情时间当前夏令时时间与正常时间偏移分钟数，没有夏令时的或者非夏令时时间填0。最高字节保持0. 
							  //2017/3/15时区偏移量含义修改：低2个字节表示行情所在时区和东八区时间差值分钟数，其他含义一致
    short nBeginTime[8];      // 交易节开始时间（分钟数）
    short nEndTime[8];        // 交易节结束时间（分钟数）
	char  chExchTimeFlags[8]; // 20160511：交易节标志
	int   nStaticsParam[8];   // 静态统计参数
    //数据时间信息//////////////////////////////////////
    int   nDate;              // 数据交易日（YYYYMMDD）
    int   nLastDate;          // 最后更新日期（YYYYMMDD）
    int   nLastTime;          // 最后更新时间（秒）
    int   nLastMilliSecond;   // 最后更新时间（毫秒）
    //数据类型信息//////////////////////////////////////
    int   nDataType;          // 数据类型
    int   nItemSize;          // 数据项大小
    int   nItems;             // 数据记录数
    int   nTickSize;          // Tick数据大小		
    int   nTickPacketSize;    // Tick数据块大小	
    int   nTickBlocks;        // 已经保存的Tick数据块个数
    int   nIndexs;            // 指数个数
	int   nCRC;
	//证券名称变化计数器，用于通知客户端重新加载名称
    int   nStkNameChangeCountForClient;
    //Save数据信息（断点传送保存信息）//////////////////
	//代码
	//   int   nCodeOffset;        // 代码开始位置
	//   int   nCodeSize;          // 代码信息
	//   int   nCodeItems;         // 代码个数
	//   int   nCodeCRC;           // CRC
	////预处理文件
	//   int   nFileOffset;        // 预处理文件开始位置
	//   int   nFileSize;          // 预处理文件总大小
	//   int   nFileItems;         // 预处理文件个数
	//解压位置
    int64_t   nProcessedSize;     // 已处理数据大小

	// BreakPoint ID
	int nBreakPointID;
	char  chAuthor[16];       // 宏汇授权号

	//保留为自定义用途字段
	int64_t iExternFlags;       //20160528 外部使用数据标志，从iResvData取1个
	int64_t iResvData[15];
};

//行情快照////////////////////////////////////////////
//NOTE： 所有价格按chDec计算.比如4位小数则所有价格为原始价格乘以10000
struct TDMarketData_Packet
{
	//代码信息
    char    chCode[32];        // 交易所代码
	char    chWindCode[36];    // Wind代码
    char    chWindType[8];     // Wind类型
    char    chName[48];        // (原始)名称
    char    chNameEng[48];     // 英文名称
	int     nRecord;           // 当日编号

	GUID    guid;              // 分类指数guid
    int     nID;               // 分类指数编号
    int     nType;             // 类型
	int     nQuoteUnit;        // 报价单位（元，定点4位小数）
    int	    nLotSize;          // 交易量纲
    char	chDec;             // 价格小数位; 小于等于4统一按4处理!
	char    chVol;             // 成交量显示量纲(10的次方:支持0,1,2,3)
	char    chUnicodeFlag;     // Unicode标志[TDCODE_UNICODE_](0x01:chCode, 0x02:chName, 0x04: chNameEng)
    char    chCurrencyCode[4]; // 币种


	//交易时间
    char	nExchItems;        // 正常交易时间段数目
	int     nTimeDif;          // 时差（该市场的交易时段所用的时区当天和东八区之间时间差的分钟数。如美国东部时区为-5，交易所采用夏令时，在夏天美国的交易所timediff填”-720”((-5+1-8)*60=-720)）
	char	chExchTimeFlags[8];// 20160511：增加交易时间节状态！注意：1)为兼容，时间节数组分2段，前一段和以前含义相同（正常交易时间段),个数等于nExchItems； 第二部分是特殊段，类型chExchTimeFlags对应位表示，不计入nExchItems
    short   nBeginTime[8];     // 开市时间
    short   nEndTime[8];       // 休市时间

	// Tick数据保存记录
	int     nTickItems[12];     // Tick数据个数
	int     nTickSize[12];      // Tick数据大小
	int     nTickHead[12];      // Tick数据头位置
	int     nTickTail[12];      // Tick数据尾位置
                               // 数据类型定义:
                               // ID_TKTYPE_TICK         0 Tick
                               // ID_TKTYPE_MINUTE       1 One Minute
                               // ID_TKTYPE_MATCH        2 Only Match Tick
                               // ID_TKTYPE_STEP         3 Step
                               // ID_TKTYPE_ORDER        4 Order
                               // ID_TKTYPE_ORDERQUEUE   5 Order Queue
                               // ID_TKTYPE_BROKERQUEUE  6 Broker Queue
                               // ID_TKTYPE_LINKED       7 Linked 关联品种

	//传送标志(TD系统行情断点数据从这里开始)
	int     nTranslateFlag;
    
	int     nDate;                 // 成交日期（YYYYMMDD）
    int     nTime;                 // 成交时间（秒）
    int     nMilliSecond;          // 成交时间（毫秒）
	int     nFlags;                // 标志(保留)
    int	    nTickFlag;             // Tick涨跌标志

	//交易标志
    char	chStatusFlag;          //交易状态标志
									//  正常交易
									//0 首日上市
									//1 增发新股
									//2 上网定价发行
									//3 上网竞价发行
									//A 交易节休市
									//B 整天停牌
									//C 全天收市
									//D 暂停交易
									//d 集合竞价阶段结束到连续竞价阶段开始之前的时段（如有）
									//E START - 启动交易盘
									//F PRETR - 盘前处理
									//G DEL   - 不可恢复交易的熔断阶段（上交所的N）  
									//H HOLIDAY - 放假
									//I OCALL - 开市集合竞价
									//J ICALL - 盘中集合竞价
									//K OPOBB - 开市订单簿平衡前期
									//L IPOBB - 盘中订单簿平衡前期
									//M OOBB  - 开市订单簿平衡
									//N IOBB  - 盘中订单簿平衡
									//O TRADE - 连续撮合;美股正常交易
									//P BREAK - 休市
									//Q VOLA  - 波动性中断
									//q 可恢复交易的熔断时段(上交所的M)
									//R BETW  - 交易间
									//S NOTRD - 非交易服务支持
									//T FCALL - 固定价格集合竞价
									//U POSTR - 盘后处理
									//V ENDTR - 结束交易
									//W HALT  - 暂停
									//X SUSP  - 停牌
									//Y ADD   - 新增产品
									//Z DEL   - 可删除的产品
									//v VCM   -  市场波动调节机制(港交所）20160503
									//        - “冷静期”的描述如下：1)冷静期为时5分钟，并需要在价格范围内交易，其后交易恢复正常 2)冷静期内，受影响的个别股票或期货合约仅可在固定价格限制内，股票（期货合约）±10％（±5%）进行交易
									//< 代表盘前
									//> 代表盘后
									//c (小写c)收市竞价交易(Closing Auction Session (CAS))
	char    chTradeFlag;           // 交易标志
    char    chOrderIndicator;      // 表示最好的买方或者卖方订单是来自于集合竞价阶段还是连续交易阶段(A-集合竞价 C-连续交易 N-无)
    char    chVolaIndicator;       // 波动性中断(B-已经进入 P-潜在 V-波动性中断 N-无波动性中断 F-冻结 X-延长的波动性中断)
    char    chDividenIndicator[4]; // 除权除息标志(XD-现金分红 XR-配股 DR-现金与配股 ' '-无分红或配股)
    char    chPrefix[4];           // 冠名前缀
    char    chFreeText[45];        // 自由文本
	char	chFreeTextCount;       // 自由文本变动计数器
    char	chMatchFlag;           // 成交标志（计算）
	char    chQualificationClass;
	//char    chFreeText1[48];       // 自由文本1,目前用来做沪港通股票状态

	//基本数据
	int64_t iPreVolume;       // 前成交量
	int64_t iHandleFlag;      // 数据处理标志目前有3个：TD_HANDLE_MINUTE(0x01) :表示此笔行情参与分钟线计算；TD_HANDLE_MATCH_TICK(0x02)  :表示此笔行情参与MATCH_TICK计算; TD_HANDLE_NO_KDAY(0x04):表示此笔快照不能用于生产日k线
	int64_t iAvgPrice;
    int64_t nPreClose;	      // 前收盘
	int64_t nHighLimit;       // 涨停价
    int64_t nLowLimit;        // 跌停价
	int     nTradingDate;
	int     nEventFlags;           //开放低8个bit最为标记位: 快照事件标志定义:TD_SP_STATUS_CLEAR 0x01; TD_SP_STATUS_NOT_TRADE 0x02; TD_SP_STATUS_TRANS 0x04

	int64_t nOpen;            // 开盘价            <----------+
    int64_t nHigh;            // 最高价                       |
    int64_t nLow;             // 最低价                       |
    int64_t nMatch;           // 成交价                       |
	int     nMatchItems;      // 成交笔数                     |
	int64_t nAuctionPrice;    // 波动性中断参考价             |
	int     nAuctionQty;      // 波动性中断集合竞价虚拟匹配量 |
	int64_t iVolume;          // 成交量(指数的的量纲是100股)  | 此段数据不可随意调整
    int64_t iTurnover;        // 成交额(指数的的量纲是100元)  | 压缩算法用了整块数据比较
	int64_t iBuyVolume;       // 买盘(外汇买入价)             |
    int64_t iSaleVolume;      // 卖盘(外汇卖出价)             |
    TD_AskBid     ABData;     // 委卖委买数据      <----------+
    TD_OrderQueue OrderQueue; // 委托队列
    TD_Exdata     Exdata;     // 扩展数据
    TD_Info       Info;       // 基本信息

	// Tick统计数据
	int64_t nPrePrice;        // 上一笔成交价
	int     nCurrentVol;      // 现量
	// 分钟数据统计
	TD_Minute_Packet Minute;
	TD_Minute_Packet PreMinute;
	// 用于Tick数据压缩/还原的数据
    TD_Step_Packet   Step;  // 逐笔成交
    TD_Order_Packet  Order; // 逐笔委托
	TD_Match_Packet  Match; // 成交Tick
};


//TDF Datafeed 数据类型定义
#define ID_TDFDATATYPE_SNAPSHOT      1  // 行情快照
#define ID_TDFDATATYPE_ORDERQUEUE    2  // 委托队列
#define ID_TDFDATATYPE_STEP          3  // 逐步成交
#define ID_TDFDATATYPE_MARKETVIEW    4  // 市场总揽
#define ID_TDFDATATYPE_ORDER         6  // 逐步委托
#define ID_TDFDATATYPE_ADDCODE       9  // 新增证券代码
#define ID_TDFDATATYPE_NAMECHANGE    10 // 证券名称变更
#define ID_TDFDATATYPE_VDECHANGE     11 // 行情源变更（L2-〉L1，L1-〉L2）
#define ID_TDFDATATYPE_TRADINGTIME   12 // 交易时间
#define ID_TDFDATATYPE_FILE          13 // 文件
#define ID_TDFDATATYPE_NON_MD_MSG    14 // 非行情数据类型
#define ID_TDFDATATYPE_MARKET_EVENT  15 // 市场事件通知
#define ID_TDFDATATYPE_CODE_EVENT    16 // 代码事件通知
#define ID_TDFDATATYPE_OTC_OPTION    17 // 场外期权
#define ID_TDFDATATYPE_QUOTEUNITCHANGE    19 // 价差变更
//#define ID_TDFDATATYPE_CODE_CLEAR_LIST 18 //清盘代码列表
//#define ID_TDFDATATYPE_SSE_HKAMOUNT		14 // 上交所港股通额度
//#define ID_TDFDATATYPE_SSE_HKSTKSTATUS	15 // 上交所港股通股票状态
//#define ID_TDFDATATYPE_EXCHRATE			16 // 参考汇率






struct TDFDynData_DataFeed_Packet
{
	int  nDFType;
    int  nRecord;                // 当日编号
    int  nType;                  // 数据类型: 
                                 // ID_TDFDATATYPE_SNAPSHOT    1 行情快照
                                 // ID_TDFDATATYPE_ORDERQUEUE  2 委托队列
                                 // ID_TDFDATATYPE_STEP        3 逐步成交
                                 // ID_TDFDATATYPE_MARKETVIEW  4 市场总揽
                                 // ID_TDFDATATYPE_ORDER       6 逐步委托
                                 // ID_TDFDATATYPE_ADDCODE     9 新增证券代码
                                 // ID_TDFDATATYPE_NAMECHANGE  10 正确名称变更
                                 // ID_TDFDATATYPE_VDECHANGE   11 行情源变更（L2-〉L1，L1-〉L2）
                                 // ID_TDFDATATYPE_TRADINGTIME 12 交易所时间
    int  nSide;                  // 数据方向(当nType==ID_TDFDATATYPE_SNAPSHOT，nSide&0x01买出队列变化，nSide&0x02买入队列变化)
    int  nBegin;                 // 开始位置(逐笔委托、逐笔成交的开始记录数)
    int  nItems;                 // 数据个数(逐笔委托、逐笔成交的记录个数)
    int  nItemSize;              // 数据项数据长度
    const TDMarketData_Packet *pMDData; // 行情快照
    void *pData;                 // 其他数据类型
};


//SaveData Head
struct TDFSaveData_Head
{
	int  nCodeOffset;	// 代码开始位置
	int  nFlags;		// 数据标志
	char chMarket[4];	// 市场标志
	int  nDate;			// 数据日期
	int  nMarketID;		// 市场ID
	int  nDataSize;		// 总大小

	int  nCodeSize;		// 代码信息
	int  nCodeItems;	// 代码个数
	int  nCodeCRC;		// 代码CRC

	int  nFileOffset;	// 文件开始位置
	int  nFileSize;		// 相关文件总大小
	int  nFileItems;	// 文件个数

	GUID guidData;		// 数据GUID

	//分段内存块大小
	int  nSaveBlockSize[4];
	int  nDataVer;		// 20160929 Save协议版本。当前版本为2.000(2000)
	int  nResv[3];      //20160630,2,3字节扩展为新版本Save的nDataSize


	//交易时间
	int  nTimeDif;				// 时差（该市场的交易时段所用的时区当天和东八区之间时间差的分钟数。如美国东部时区为-5，交易所采用夏令时，在夏天美国的交易所timediff填”-720”((-5+1-8)*60=-720)） 
	int  nExchTimes;			// 交易时间节数
	int  chTimeFlags[2];        // 交易节标志
	int  nBeginTime[8];			// 开始时间
	int	 nEndTime[8];			// 结束时间
	//分段保存记录
	int  nSaveDataTimeDelta;	// 保存时间间隔(每5分钟保存一次)
	int  nSaveDataTimeNext;		// 下次保存时间
	int  nSaveDataTimes;		// 已保存次数
	int	 nSaveDataTime[24*12];	// 时间
	int  nSaveDataOffset[24*12];// 数据偏量
	//Add 20160202 Danny
	int  nNonMDMsgDefOffset; 	// 非行情数据描述信息开始位置
	int  nNonMDMsgDefItems; 	// 非行情数据描述信息项数
	int  nNonMDMsgDefSize;		// 非行情数据描述信息总大小
	//char chMarketType[64];      // 20161117:添加市场类型,是否单一类型市场，如果为空，则为复合类型，否则为单一类型  --》市场主类型，子类型，。。。，子类型
};

struct TDSaveDataCode_Head
{
	int   nHeadSize;
	int   nDate;
	int   nItems;
	int   nSize;
	int   nCRC;
	int   nLanguage;	//语种代码
	char  nAskBidDeeps;
	char  nAskBidFlags;
	char  minidataFlag;	//小行情标志
	char  nFlags;		//0x01:AB200
	exchange_time ExchTime;
	short nStaticsParam[8];
	char  hhauthor[16];
	int   nMarketID;
	char  chMarket[4];
	GUID  guidData;
	int  nTimeDif;				// 时差（该市场的交易时段所用的时区当天和东八区之间时间差的分钟数。如美国东部时区为-5，交易所采用夏令时，在夏天美国的交易所timediff填”-720”((-5+1-8)*60=-720)） 
	int  nExchTimes;			// 交易时间节数
	int  chTimeFlags[2];        // 交易节标志
	int  nBeginTime[8];			// 开始时间
	int	 nEndTime[8];			// 结束时间
};

//SaveFile Head
struct TDFSaveData_File
{
	char chFileName[64];
	int  nFileDate;
	int  nFileTime;
	int  nFileSize;
	int  nCompressLen;
};

//TDB 数据定义

// K线周期定义
#define ID_KCYC_TICK         0  // Tick线
#define ID_KCYC_TICKBAR      1  // TickBar线
#define ID_KCYC_XSECONDS     2  // X秒线
#define ID_KCYC_XMINUTES     3  // X分钟线
#define ID_KCYC_DAY          6  // 日线
#define ID_KCYC_WEEK         7  // 周线
#define ID_KCYC_MONTH        8  // 月线
#define ID_KCYC_SEASON       9  // 季线
#define ID_KCYC_HALFYEAR     10 // 半年线
#define ID_KCYC_YEAR         11 // 年线
#define ID_KCYC_XDAYS        12 // X周线
#define ID_KCYC_XWEEKS       13 // X周线
#define ID_KCYC_XMONTHS      14 // X月线

// 除权/复权
#define ID_KDATAFLAG_ORGDATA  0 // 原始数据
#define ID_KDATAFLAG_CQ       1 // 除权
#define ID_KDATAFLAG_FQ       2 // 复权

// Tick数据
struct TD_Tick_Packet
{
	int	      nDate;        // 日期
	int	      nTime;		// 精确到秒(time&0x1ffff)  0x00800000=B 0x00400000=S
	int	      nMillisecond; // 毫秒
	int	      nMatchItems;	// 成交笔数
	int64_t	  nOpen;        // 开盘
	int64_t	  nHigh;		// 最高
	int64_t	  nLow;			// 最低
	int64_t	  nPrice;		// 价格
	int64_t	  nHighLimit;   // 涨停价
	int64_t	  nLowLimit;    // 跌停价
	char      chTradeFlag;	// 交易标志	
	char	  chStatusFlag;  //状态标志
	int64_t   iVolume;      // 成交量
	int64_t   iTurnover;	// 成交额 1元|100元	
	int64_t   iInside;      // 内盘
	int64_t   iOutside;     // 外盘
	TD_AskBid ABData;
	TD_Exdata ExtData;
};

struct TDBar_K_Head 
{
	int  nCyc;	                  // 数据周期
	int  nCyc1;		              // 数据标志	
	int  nCQFlag;	              // 除权标志（除权ID_KDATAFLAG_定义）
	int  nFlagEx;                 // 扩展数据标志（债券:0:竞价 1:全价）
	int  nGetMode;                // 0:按最近数据个数读取  1:按日期段读取 2:从nBeginDate开始倒退nLastItems条
	int  nLastItems;              // 最近数据个数（nGetMode==0）
	int  nBeginDate;              // 开始日期（nGetMode==1）
	int  nEndDate;	              // 结束日期（nGetMode==1）
	int  nDynDate;                // 动态数据日期
	int  nCQStartDate;            // 从此日期开始除权
	// 以下数据是在K线中加入最新数据时设置
	BOOL bNewDataFlag;            // 加入最新数据（TRUE：加入）
	const TDMarketData_Packet *pMDData; // 最新行情快照（日线或分钟线）
	TD_Minute_Packet    *pMinute; // 动态数据个数（分钟线）
	int  nMinutes;
	TD_Tick_Packet      *pTick;   // 动态数据缓存（秒线或分钟线）
	int  nTicks;
};
struct TDEventInfo{
	int64_t market_time;
	int64_t vss_time;
	int nEventCode;
};
struct TDUserDefineEventInfo{
	int nEventCode;
	uint8_t* buf; //需释放
	uint32_t len;
};
struct TDClearCodes{
	int64_t time;
	unsigned int nRecordLen;
	int* records;  //需释放
};

// K线数据
struct TDBar_K_Packet
{
	int nDate;         // 日期
	int nTime;         // 时间
	int nStatus;       // 状态
	int nMatchItems;   // 成交笔数
	int64_t nPreClose;     // 前收盘
	int64_t nOpen;         // 开盘价
	int64_t nHigh;         // 最高价
	int64_t nLow;          // 最低价
	int64_t nClose;        // 收盘价
	int64_t nAVPrice;      // 均价
	int64_t iVolume;   // 成交量
	int64_t iTurnover; // 成交额
	int     nFlags;     //标记位，0x01:是否参与变频计算
	union TD_K_Ext     // 扩展数据
	{
		struct TD_K_Index_Packet
		{
			int nStocks;       // 样本个数
			int nUps;          // 上涨个数
			int nDowns;        // 下跌个数
			int nLevels;       // 持品个数
		}Index;
		struct TD_K_Futures_Packet
		{
			int nPreSettlement;// 前结算
			int nSettlement;   // 结算价
			int nPositions;    // 持仓量
			int nDelta;
		}Futures;
		struct TD_K_Fund_Packet
		{
			int nNetValue;     // 净值
		}Fund;
		struct TD_K_Bond_Packet
		{
			int nInterest;     // 利息
		}Bond;
	}Ext;
};

struct TDBar_DirSet_Packet
{
	int nType;              // 数据类型：ID_TKTYPE_
	int nBeginYear;         // 开始年（日线无效，日线只保存在一个目录中，其它数据按年分目录）
	int nEndYear;           // 结束年（日线无效）
	char chDir[_MAX_PATH]; // 主目录
	                        // 例如d:\td，表示:2010年深圳L2类型tick数据目录为d:\td\data2\tick\sz\sz2010
	                        //            表示:2010年深圳L1类型tick数据目录为d:\td\data\tick\sz\sz2010
};

//Tick数据文件头信息
struct TDBar_Save_Head
{
	char	chRemark[15];   // 识别串
	char    nSave;			// 类型
	char	chCode[8];      // 代码
	char	chMarket[4];    // 市场
	char	endflag;
	char    compressflag;	// 压缩标志
	char	filetype;
	char    type;
	int     tsflag[4];
	int     nItems;			// 总记录
	int     nIndexMax;		// Index最大记录数
	int     nIndexSize;		// Index记录大小
	int     nIndexOffset;	// Index数据存放位置
	int     nDataOffset;	// 数据存放位置
};

struct TDBar_Save_Index
{
	int nDate;		     // 数据日期
	int	nOffset;	     // 数据存放位置
	int nItems;		     // 数据数量
	int nSize;		     // 数据保存大小
};

/**
* 数据目录结构
* [data2] 
*     + - [day]   
*     |       + [sz]
*     |       + [sh]
*     |       + [cf]
*     + - [infodata]
*     |       + [sz]
*     |       |   + [sz2010]
*     |       |   + [sz2011]
*     |       + [sh] ...
*     |       + [cf] ...
*     + - [minute]
*     |       + [sz]
*     |       |   + [sz2010]
*     |       |   + [sz2011]
*     |       + [sh] ...
*     |       + [cf] ...
*     + - [tick]
*     + - [step]
*     + - [order]
*     + - [ab50]
*     + - [broker]
*/

struct TDBar_Day_Head
{
	char chRemark[15];
	char chVer;		//版本号, 2008.01.02升级为1, 将换手率*10000
	int32_t tsFlag[4];
	char chMarket[4];
	int  nType;
		 
	char chFlag;	//标志
	char bSysData;	//系统数据
	char bCycFlag;	//周期标志 0:日 'W':周 'M':月 'S':季 'Y':年
	char bOneDays;	//一天行情
	int  nLastDays;	//最近天数
	     
	int  nBegin;
	int  nEnd;
	int  nDays;		//交易日
	     
	int  nPreBegin;
	int  nPreEnd;
	int  nPreDays;
	     
	int  nItems;
	int  nItemSize;
	int  nCompressSize;
	int  nOffset;
};


struct TDBar_Day_Packet
{
    char chCode[32];         // 代码
    char chName[48];         // 名称
    int  nType;              // 证券类型
    int  nLotSize;           // 交易量纲
    char chDec;              // 显示小数位
	char chVol;              // 显示量纲
	char chUnicodeFlag;      // Unicode标志[TDCODE_UNICODE_](0x01:chCode, 0x02:chName, 0x04: chNameEng)
    char chCurrencyCode[4];  // 货币

	int  nExchangeDays;		 // 时间段交易天数
	int  nStatus;			 // 状态
	//交易数据
	int64_t iPreVolume;	//上一个交易日成交量
	int64_t nPreClose;	//前收
	int64_t nOpen;		//开盘
	int64_t nHigh;		//最高
	int64_t nLow;		//最低
	int64_t nClose;		//最新
	int nMatchItems;	//成交笔数

	int64_t iVolume;	//成交量	(交易所原始数据, 指数为100股)
	int64_t iTurnover;	//成交金额	(个股为1元，指数为100元)

	//区间统计
	int64_t  nHighClose;			//最高收盘价
	int64_t  nLowClose;			//最低收盘价

	//扩展数据
	union TDBar_DayEx_Packet
	{
		struct indexdata
		{
			int nStocks;		//品种个数
			int nZDP[3];		//涨跌平
		}Index;
		struct funddata
		{
			int nNetValue;		//基金净值
		}Fund;
		struct bonddata
		{
			int nInterest;		//国债利息额
			int nYTM;			//国债到期收益率
		}Bond;
		struct futuresdata
		{
			int nSettlement;	//结算价
			int nPosition;		//合约持仓量
			int nPreSettlement;	//前结算价
			int nPrePosition;	//前持仓量
		}Futures;
	}ExtData;
};


struct NonMDHead
{
	char chTDRemark[32];
	int  nMagicCode;         // 标示码
	int  nDataSize;
	int  nMsgNum;
	int  nDataOffset;
};
// 港股通结构
/*struct TD_SSE_HK_Amount
{
	int		lDate;
	int		lTime;					// 时间
	int64_t     iThresholdAmount;       // 每日初始额度，单位人民币元
	int64_t     iPosAmt;                // 日中剩余额度，单位人民币元
	char        chAmountStatus;         // 额度状态   '1'：额度用完；'2'：额度可用
};*/

/*struct TD_SSE_HK_StockStatus
{
	char		chCode[8];				// 证券代码
	char        SecTradingStatus1[8];   // 港股整手订单(C8):         该字段为8位字符串，左起每位表示特定的含义，无定义则填空格。第1位：‘0’表示限制买入，‘1’表示正常无此限制。
	// 第2位：‘0’表示限制卖出，‘1’表示正常无此限制。
	char        SecTradingStatus2[8];   // 港股零股订单(C8):         该字段为8位字符串，左起每位表示特定的含义，无定义则填空格。第1位：‘0’表示限制买入，‘1’表示正常无此限制。
};*/

/*struct TD_ExchRate_Record
{
	int64_t    iBuyPrice;       //参考汇率买入价（小数点5位，乘以100000）
	int64_t    iSellPrice;      //参考汇率卖出价（小数点5位，乘以100000）
	int64_t    iMedianPrice;    //参考汇率中间价（小数点5位，乘以100000）
	char       chCurrencyType[8];   //货币种类
};*/

//场外期权
struct TD_OTC_Option{
	int     nTime;           // 成交时间（秒）
	int     iVolume;         // 成交量
	int64_t nMatch;          // 成交价
	int64_t  nPreSettlement; // 前日结算价
	int64_t  nSettlement;    // 前日结算价
	int nPrePosition;        // 前日持仓量
	int nPosition;           // 持仓量
	int64_t nAskPrice;       // 委卖价格
	int64_t nBidPrice;       // 委买价格
	int  iAskVol;            // 委卖数量
	int  iBidVol;            // 委买数量
};

struct TD_OTC_Option_Key{
	char chOptionType;             // 欧式美式C1        若为欧式期权，则本字段为“E”；若为美式期权，则本字段为“A”
	char chCallOrPut;              // 认购认沽C1        认购，则本字段为“C”；若为认沽，则本字段为“P”
	int64_t  nExercisePrice;       // 期权行权价,   
};


#define OTC_SNAPSHOT_HEAD       1024
// 基本类型定义
#define ID_TDTYPE_DATE            0x0001   // 日期
#define ID_TDTYPE_TIME            0x0002   // 时间（零时开始计时的秒）
#define ID_TDTYPE_TIME_MS         0x0003   // 毫秒
#define ID_TDTYPE_MINUTE          0x0004   // 分钟（零时开始计时的分钟数）
#define ID_TDTYPE_TIME_HHMMSS     0x0005   // 时间（HH:MM:SS）
#define ID_TDTYPE_TIME_HHMMSSMMM  0x0006   // 时间（HH:MM:SS.MMM精确到毫秒）
#define ID_TDTYPE_MINUTE_HHMM     0x0007   // 分钟（HH:MM）

#define ID_TDTYPE_PRICE32      0x0011   // 32位价格类型（定点4位小数）
#define ID_TDTYPE_PRICE64      0x0012   // 64位价格类型（定点10位小数）

#define ID_TDTYPE_INT8         0x0021   // 8位整型
#define ID_TDTYPE_UINT8        0x0022   // 8位无符整型
#define ID_TDTYPE_INT16        0x0023   // 16位整型
#define ID_TDTYPE_UINT16       0x0024   // 16位无符整型
#define ID_TDTYPE_INT32        0x0025   // 32位整型
#define ID_TDTYPE_UINT32       0x0026   // 32位无符整型
#define ID_TDTYPE_INT64        0x0027   // 64位整型
#define ID_TDTYPE_UINT64       0x0028   // 64位无符整型

#define ID_TDTYPE_INT32X10N    0x002a   // 32位整型(有10**n的特征，压缩更小)
#define ID_TDTYPE_UINT32X10N   0x002b   // 32位无符整型(有10**n的特征，压缩更小)
#define ID_TDTYPE_INT64X10N    0x002c   // 64位整型(有10**n的特征，压缩更小)
#define ID_TDTYPE_UINT64X10N   0x002d   // 64位无符整型(有10**n的特征，压缩更小)

// FIXME: 拼写错误 DF -> FD
/*#define ID_SDBDF_1DEC32       0x0031   // 一位小数
#define ID_SDBDF_2DEC32       0x0032   // 二位小数
#define ID_SDBDF_3DEC32       0x0033   // 三位小数
#define ID_SDBDF_4DEC32       0x0034   // 四位小数
#define ID_SDBDF_1DEC64       0x0038   // 一位小数(64位)
#define ID_SDBDF_2DEC64       0x0039   // 二位小数(64位)
#define ID_SDBDF_3DEC64       0x003a   // 三位小数(64位)
#define ID_SDBDF_4DEC64       0x003b   // 四位小数(64位)
#define ID_SDBDF_5DEC64       0x003c   // 五位小数(64位)
#define ID_SDBDF_6DEC64       0x003d   // 六位小数(64位)
#define ID_SDBDF_7DEC64       0x003e   // 七位小数(64位)
#define ID_SDBDF_8DEC64       0x003f   // 八位小数(64位)
*/
#define ID_TDTYPE_FLOAT        0x0041   // 浮点数
#define ID_TDTYPE_DOUBLE       0x0042   // 双精度数

#define ID_TDTYPE_CHAR         0x0051   // 1个字符
#define ID_TDTYPE_TEXT         0x0052   // 文本

#define ID_TDTYPE_CURRENCY     0x0061   // 币种
#define ID_TDTYPE_GUID         0x0062   // GUID
#define ID_TDTYPE_IPV4         0x0063   // IPV4 Address

// 非结构化数据
#define ID_TDTYPE_VLTEXT       0x0071   // 可变长文本（offset, len）
#define ID_TDTYPE_BINARY       0x0072   // 二进制（offset, len, items）

#define ID_TDTYPE_ICON         0x0081   // 图标（offset, len）
#define ID_TDTYPE_PICTURE      0x0082   // 图片（offset, len）
#define ID_TDTYPE_VIDEO        0x0083   // 视频（offset, len, time）
#define ID_TDTYPE_AUDIO        0x0084   // 声频（offset, len, time）

//
struct TD_Field_Info
{
	void Init(char* _chName, int _nType, int _nOffset, int _nWidth, int _nTypeEx, int _nFlags, int _resv)
	{
		strncpy(chName,_chName,48);
		chName[47] ='\0';
		nType   = _nType;
		nOffset = _nOffset;
		nWidth  = _nWidth;
		nTypeEx = _nTypeEx;
		nFlags  = _nFlags;
		resv    = _resv;
	}
	char chName[48]; // 字段名称
	int nType;       // Field Type
	int nTypeEx;     // Field Extend Type
	int nOffset;     // Data  offset
	int nWidth;      // Data  Width
	int nFlags;      // Field Flags 0x0f:小数点
	int resv;
};
struct _NonMD_Msg_Def
{
	int msgType;
	int nFieldNumber;
	int msgSize;
	int msgDefSize;
	TD_Field_Info fieldInfo[1];	
};
struct NonMD_Msg_Def
{
	int msgType;
	int nFieldNumber;
	int msgSize;
	int msgDefSize;
	TD_Field_Info* fieldInfo;
	void relese()
	{
		if(fieldInfo)
			delete[] fieldInfo;
	}
};

struct NonMD_MsgInfo
{		
	int nSnapshotOffset;
	_NonMD_Msg_Def* pMsgDef;
	NonMD_MsgInfo():nSnapshotOffset(0), pMsgDef(NULL){};
	NonMD_MsgInfo(int _nSnapshotOffset, _NonMD_Msg_Def* _pMsgDef):nSnapshotOffset(_nSnapshotOffset), pMsgDef(_pMsgDef){};
};
struct NonMD_Packet
{
	_NonMD_Msg_Def* msgDef;
	char* msgContent;
};

class NonMDSnapshotVector
{
public:
	NonMDSnapshotVector(){ snapshotPointer = NULL;pPackets= NULL;length=0;}
	void Init(char* _snapshotPointer){ 		
		snapshotPointer       = _snapshotPointer;				
		NonMDHead* pNonMDHead =  (NonMDHead*)_snapshotPointer;
		length = pNonMDHead->nMsgNum;
		pPackets = new NonMD_Packet[length];
		_NonMD_Msg_Def* pTmp = (_NonMD_Msg_Def*)(_snapshotPointer + pNonMDHead->nDataOffset);
		for(int i = 0; i<length; i++)
		{
			pPackets[i].msgDef = pTmp;
			pPackets[i].msgContent = (char*)pTmp + pTmp->msgDefSize;
			pTmp =  (_NonMD_Msg_Def*)((char*)pTmp + pTmp->msgDefSize + pTmp->msgSize);
		}
	}
	NonMD_Packet& operator [] (int i)
	{
		return pPackets[i];
	}
	~NonMDSnapshotVector(){
		if(pPackets)
			delete[] pPackets;
		if(snapshotPointer)
			delete [] snapshotPointer;
	}
	int Length(){return length;}
	const NonMDHead* GetNonMDHead(){return (NonMDHead*)snapshotPointer;}
	const char* GetData() {return snapshotPointer;}
private:
	char* snapshotPointer;
	int length;
	NonMD_Packet* pPackets;
};
class NonMDMsgDecoder{
public:
	NonMDMsgDecoder(){ 
		pMsg = NULL;
	}
	bool Init(char* _pMsg, int size){			
		pDef=(_NonMD_Msg_Def*) _pMsg;
		assert(size == (pDef->msgSize + pDef->msgDefSize));
		if(size != (pDef->msgSize + pDef->msgDefSize))
			return false;
		pMsg = _pMsg + pDef->msgDefSize;
	}
	//模板？
	template <class Type, int nFieldType>
	Type GetNumField(int index)
	{
		if(!pDef || !pMsg)
			return 0;
		if(index < pDef->nFieldNumber && pDef->fieldInfo[index].nType == nFieldType)
			return *(Type*)(pMsg+ pDef->fieldInfo[index].nOffset);
		return 0;
	}
	int GetInt32Field(int index)
	{
		if(!pDef || !pMsg)
			return 0;
		if(index < pDef->nFieldNumber&&(ID_TDTYPE_INT16 == pDef->fieldInfo[index].nType || ID_TDTYPE_INT32 == pDef->fieldInfo[index].nType))
			return *(int*)(pMsg+ pDef->fieldInfo[index].nOffset);
		return 0;
	}
	int64_t GetInt64Field(int index)
	{
		if(!pDef || !pMsg)
			return 0;
		if(index < pDef->nFieldNumber && pDef->fieldInfo[index].nType == ID_TDTYPE_INT64)
			return *(int64_t*)(pMsg+ pDef->fieldInfo[index].nOffset);
		return 0;
	}
	int64_t GetUInt64Field(int index)
	{
		if(!pDef || !pMsg)
			return 0;
		if(index < pDef->nFieldNumber && ID_TDTYPE_UINT64 == pDef->fieldInfo[index].nType)
			return *(int64_t*)(pMsg+ pDef->fieldInfo[index].nOffset);
		return 0;
	}
	int GetUInt32Field(int index)
	{
		if(!pDef || !pMsg)
			return 0;
		if(index < pDef->nFieldNumber && (ID_TDTYPE_UINT32 == pDef->fieldInfo[index].nType || ID_TDTYPE_UINT16 == pDef->fieldInfo[index].nType || ID_TDTYPE_TIME_HHMMSSMMM ==  pDef->fieldInfo[index].nType ||
			ID_TDTYPE_TIME == pDef->fieldInfo[index].nType || ID_TDTYPE_TIME_HHMMSS == pDef->fieldInfo[index].nType || ID_TDTYPE_TIME_HHMMSSMMM ==  pDef->fieldInfo[index].nType ||
			ID_TDTYPE_PRICE32 == pDef->fieldInfo[index].nType || ID_TDTYPE_MINUTE == pDef->fieldInfo[index].nType || ID_TDTYPE_MINUTE_HHMM ==  pDef->fieldInfo[index].nType ||
			ID_TDTYPE_TIME_MS == pDef->fieldInfo[index].nType))
			return *(uint32_t*)(pMsg+ pDef->fieldInfo[index].nOffset);
		return 0;
	}
	float GetFloatField(int index)
	{
		if(!pDef || !pMsg)
			return 0;
		if(index < pDef->nFieldNumber && ID_TDTYPE_FLOAT == pDef->fieldInfo[index].nType)
			return *(float*)(pMsg+ pDef->fieldInfo[index].nOffset);
		return 0;
	}
	double GetDoubleField(int index)
	{
		if(!pDef || !pMsg)
			return 0;
		if(index < pDef->nFieldNumber && ID_TDTYPE_DOUBLE == pDef->fieldInfo[index].nType)
			return *(double*)(pMsg+ pDef->fieldInfo[index].nOffset);
		return 0;
	}
	char GetCharField(int index)
	{
		if(!pDef || !pMsg)
			return 0;
		if(index < pDef->nFieldNumber && ID_TDTYPE_CHAR == pDef->fieldInfo[index].nType)
			return *(pMsg+ pDef->fieldInfo[index].nOffset);
		return 0;
	}
	char* GetStringField(int index)
	{
		if(!pDef || !pMsg)
			return "";
		if(index < pDef->nFieldNumber && ID_TDTYPE_TEXT == pDef->fieldInfo[index].nType)
			return pMsg+ pDef->fieldInfo[index].nOffset;
		return "";
	}
private:
	_NonMD_Msg_Def* pDef;
	char* pMsg;
};
#pragma pack(pop) 