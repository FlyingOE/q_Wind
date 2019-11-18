#ifndef _TDNONMARKETSTRUCT_H
#define _TDNONMARKETSTRUCT_H

/*
所有非市场行情数据，使用tdfapi30项目下的头文件TDNonMarketStruct.h来共同约定,并且启用TDNonMarket命名空间，VSS直接include这个文件，需要增删改查非市场行情结构体，都要在这个文件中体现，下游如果使用非市场行情并且使用结构体定义的方式获取，也要include这个文件，确保上下游同步。
*/

#pragma pack (1) 

namespace TDNonMarket
{
#if 1	//非市场行情定义在这里

//港股非行情数据结构体消息编号，范围0xE0-0xEF
#define VSS_HKEX_TD_CSCSHQ_INDEXVOLUME 224
#define VSS_HKEX_TD_CSCSZQ_INDEXVOLUME 225
#define VSS_HKEX_TD_STOCK_CONNECT_MARKET_TURNOVER  226
#define VSS_HKEX_TD_STOCK_CONNECT_DAILY_QUOTE_BANLANCE 227

//沪股非行情数据结构体消息编号，范围0x90-0x9F
#define VSS_SHFAST_TD_SSE_HK_AMOUNT   144
#define VSS_SHFAST_TD_EXCHRATE_RECORD 145

//深港通额度消息编号
#define VSS_SZ_TD_HK_AMOUNT			  146

//外汇交易中心异常数据处理
#define VSS_CFETS_CANCEL_DEAL		  176  


// 港股通结构(VSS_SHFAST_TD_SSE_HK_AMOUNT, VSS_SZ_TD_HK_AMOUNT)
struct TD_SSE_HK_Amount
{
	int		lDate;
	int		lTime;					// 时间
	int64_t     iThresholdAmount;       // 每日初始额度，单位人民币元
	int64_t     iPosAmt;                // 日中剩余额度，单位人民币元
	char        chAmountStatus;         // 额度状态   '1'：额度用完；'2'：额度可用
};

//
struct TD_SSE_HK_StockStatus
{
	char		chCode[8];				// 证券代码
	char        SecTradingStatus1[8];   // 港股整手订单(C8):         该字段为8位字符串，左起每位表示特定的含义，无定义则填空格。第1位：‘0’表示限制买入，‘1’表示正常无此限制。
	// 第2位：‘0’表示限制卖出，‘1’表示正常无此限制。
	char        SecTradingStatus2[8];   // 港股零股订单(C8):         该字段为8位字符串，左起每位表示特定的含义，无定义则填空格。第1位：‘0’表示限制买入，‘1’表示正常无此限制。
};

//VSS_SHFAST_TD_EXCHRATE_RECORD
struct TD_ExchRate_Record
{
	int64_t    iBuyPrice;       //参考汇率买入价（小数点5位，乘以100000）
	int64_t    iSellPrice;      //参考汇率卖出价（小数点5位，乘以100000）
	int64_t    iMedianPrice;    //参考汇率中间价（小数点5位，乘以100000）
	char       chCurrencyType[8];   //货币种类
};


//沪港通余额(VSS_HKEX_TD_CSCSHQ_INDEXVOLUME)
struct MsgTD_CSCSHQ_IndexVolume
{
	__int64    lIndexVolume;        //成交额
	int     nDate;                 // 成交日期（YYYYMMDD）
	int     nTime;                 // 成交时间（秒）
};

//深港通余额(VSS_HKEX_TD_CSCSZQ_INDEXVOLUME)
struct MsgTD_CSCSZQ_IndexVolume
{
	__int64    lIndexVolume;        //成交额
	int     nDate;                 // 成交日期（YYYYMMDD）
	int     nTime;                 // 成交时间（秒）
};

//(VSS_HKEX_TD_STOCK_CONNECT_DAILY_QUOTE_BANLANCE)
struct MsgTD_Stock_Connect_Daily_Quota_Balance 
{
	int     nDate;                 // 成交日期（YYYYMMDD）
	int     nTime;                 // 成交时间（秒）
	char    chStockConnectMarket[3];
	char    chTradingDirection[3];

	__int64    lDailyQuotaBalance;        
	__int64    lDailyQuotaBalanceTime;         
};

//沪深港成交额(VSS_HKEX_TD_STOCK_CONNECT_MARKET_TURNOVER)
struct MsgTD_Stock_Connect_Market_Turnover 
{
	int     nDate;                 // 成交日期（YYYYMMDD）
	int     nTime;                 // 成交时间（秒）
	char    chStockConnectMarket[3];
	char    chTradingDirection[3];

	__int64    lBuyTurnover;        
	__int64    lSellTurnover;         
	__int64    lSumTurnover;       
};

// 外汇交易中心异常数据撤销结构
struct MsgTD_CFETS_CANCEL_DEAL
{
	int		   nDate;				// 撤销的行情发生日期（YYYYMMDD）
	int		   nCancelTime;			// 撤销的行情发生时间，精确到秒: hhmmss
	__int64    lCancelPrice;        // 撤销的行情价格, 有固定8位小数, 也就是说, 数据除10^8才是真实的值
	char       chWindCode[36];      // Wind代码
};

#endif	//在此之外不要添加任何代码
}

#pragma pack ()

#endif