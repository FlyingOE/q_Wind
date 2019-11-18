#ifndef _HHTRADEDATASET_DEFINE_H_
#define _HHTRADEDATASET_DEFINE_H_

//交易所ID定义///////////////////////////////
#define ID_MARKET_SZ	0	//深圳
#define ID_MARKET_SH	1	//上海
#define ID_MARKET_PT	2	//PT
#define ID_MARKET_HZ	3	//板块指数
#define ID_MARKET_SL	4	//自定义市场
#define ID_MARKET_MI	5	//自定义指数
#define ID_MARKET_HK	6	//港股
#define ID_MARKET_QH	7	//期货
#define ID_MARKET_FE	8	//外汇
#define ID_MARKET_KF	9	//外汇(港股)
#define ID_MARKET_CF	10	//中国金融指数

#define ID_MARKET_SHF	11	//上海金属商品交易所（2012.8.12由一个国内商品交易所分为3个交易所）
#define ID_MARKET_CZC	12	//郑州商品（2012.8.12由一个国内商品交易所分为3个交易所）
#define ID_MARKET_DCE	13	//大连商品（2012.8.12由一个国内商品交易所分为3个交易所）

#define ID_MARKET_CME	14  //芝加哥商业交易所(2013.11.20接入港股及海外行情)
#define ID_MARKET_CBT	15  //芝加哥期货交易所(2013.11.20接入港股及海外行情)
#define ID_MARKET_CMX	16  //纽约商品期货交易所(2013.11.20接入港股及海外行情)
#define ID_MARKET_NYM	17  //纽约商品交易所(2013.11.20接入港股及海外行情)
#define ID_MARKET_NYB	18  //纽约期货交易所(2013.11.20接入港股及海外行情)
#define ID_MARKET_IPE	19	//伦敦国际石油交易所(2013.11.20接入港股及海外行情)
#define ID_MARKET_LME	20	//伦敦金属交易所(2013.11.20接入港股及海外行情)
#define ID_MARKET_N		21  //纽约交易所(2013.11.20接入港股及海外行情)
#define ID_MARKET_O		22  //纳斯达克交易所(2013.11.20接入港股及海外行情)
#define ID_MARKET_HKF	23  //香港交易所期货/期权(2013.11.20接入港股及海外行情)
#define ID_MARKET_BBQ	30  //香港交易所期货/期权(2013.11.20接入港股及海外行情)

//市场设置信息（2013.1.23）
#define MARKETSETFLAGS_ADD(MARKETSET, MARKETID)	 (((char*)MARKETSET)[MARKETID/8] |= 1<<(MARKETID%8))	///<增加一个市场
#define MARKETSETFLAGS_DEL(MARKETSET, MARKETID)	 (((char*)MARKETSET)[MARKETID/8] &= ~(1<<(MARKETID%8)))	///<删除一个市场
#define MARKETSETFLAGS_OK(MARKETSET, MARKETID)	 (((char*)MARKETSET)[MARKETID/8] & (1<<(MARKETID%8)))	///<判断是否有此市场

/////////////////////////////////////////////
//证券基本分类定义(细分)
#define ID_BT_INDEX				0x01	//交易所指数
#define ID_BT_INDEX_ASIA		0x03	//亚洲指数
#define ID_BT_INDEX_FOREIGN		0x04	//国际指数
#define ID_BT_INDEX_HH			0x05	//系统分类指数
#define ID_BT_INDEX_USER		0x06	//用户分类指数
#define ID_BT_INDEX_FUTURES     0x07    //期货指数
#define ID_BT_INDEX_MD			0x08	//指数现货
#define ID_BT_SHARES_A			0x10	//A股
#define ID_BT_SHARES_S			0x11	//中小板股
#define ID_BT_SHARES_G			0x12	//创业板股
#define ID_BT_SHARES_B			0x16	//B股
#define ID_BT_SHARES_H			0x17	//H股(在香港上市的A股，2008.12.4为上海Level2行情H股增加)
#define ID_BT_SHARES_CHADR      0x18   //沪深CDR
#define ID_BT_SHARES_KCB		0x19   //科创板
#define ID_BT_SHARES_US			0x1a	//US
#define ID_BT_SHARES_USADR		0x1b	//US ADR
#define ID_BT_SHARES_E			0x1e	//扩展板块股票(港)
#define ID_BT_FUND				0x20	//基金
#define ID_BT_FUND_OPEN			0x21	//未上市开放基金
#define ID_BT_FUND_LOF			0x22	//上市开放基金
#define ID_BT_FUND_ETF			0x23	//交易型开放式指数基金
#define ID_BT_FUND_ETS			0x25	//扩展板块基金(港)
#define ID_BT_FUND_GRD          0x26    //分级子基金 //add 20160525 深圳V5
#define ID_BT_FUND_OAR          0x27    //仅申赎基金 //add 20160525 深圳V5
#define ID_BT_BOND_NA			0x30	//政府债券
#define ID_BT_BOND_CORP			0x31	//企业债券
#define ID_BT_BOND_FIN			0x32	//金融债券
#define ID_BT_BOND_CON			0x33	//可转债券
#define ID_BT_BOND_WI           0x34    //债券预发行(add by cli)
#define ID_BT_BOND_EXCH         0x35    //可交换债券（Exchangeable Bond，简称EB）
#define ID_BT_BOND_USA          0x38    //美债（1档有无效值）
#define ID_BT_REPO_NA			0x40	//国债回购
#define ID_BT_REPO_CORP			0x41	//企债回购
#define ID_BT_QZ				0x60	//权证
#define ID_BT_QZ_C				0x61	//认购权证
#define ID_BT_QZ_P				0x62	//认沽权证
#define ID_BT_QZ_C_B			0x64	//认购权证(B股)
#define ID_BT_QZ_P_B			0x65	//认沽权证(B股)
#define ID_BT_QZ_M				0x66	//牛证（moo-cow）
#define ID_BT_QZ_B				0x67	//熊证（bear）

#define ID_BT_FUTURES_IDX		0x70	//指数期货
#define ID_BT_FUTURES			0x71	//商品期货
#define ID_BT_FUTURES_SHA		0x72	//股票期货
#define ID_BT_FUTURES_IBOR		0x73	//同业拆借利率期货
#define ID_BT_FUTURES_EFN		0x74	//外汇基金债券(Exchange Fund Note Futures)
#define ID_BT_FUTURES_EFP       0x75    //期货转现货交易(Exchange For Physicals)
#define ID_BT_FUTURES_EFS       0x76    //期货转掉期交易(Exchange of Futures For Swaps)
#define ID_BT_FUTURES_CX_IDX	0x78	//指数期货连线CX
#define ID_BT_FUTURES_CC_IDX	0x79	//指数期货连线CC
#define ID_BT_FUTURES_CX		0x7a	//商品期货连线CX
#define ID_BT_FUTURES_CC		0x7b	//商品期货连线CC
#define ID_BT_FUTURES_CX_SHA	0x7c	//股票期货连线CX
#define ID_BT_FUTURES_CC_SHA	0x7d	//股票期货连线CC
#define ID_BT_FUTURES_QX		0x7e	//期现差价线
#define ID_BT_FUTURES_KQ		0x7f	//跨期差价线

#define ID_BT_FOREX_BASE		0x80	//基本汇率
#define ID_BT_FOREX_CROSE		0x81	//交叉汇率
#define ID_BT_FOREX_REVERSE		0x82	//反向汇率
#define ID_BT_FOREX_FORWARD		0x83	//远期外汇
#define ID_BT_FOREX_SWAP		0x84	//掉期外汇

#define ID_BT_OPTION_C			0x90	//认购期权
#define ID_BT_OPTION_P			0x91	//认沽期权
#define ID_BT_OPTION_C_IDX      0x92    //指数认购期权
#define ID_BT_OPTION_P_IDX      0x93    //指数认沽期权
#define ID_BT_OPTION_C_CMD      0x94	//商品认沽期权
#define ID_BT_OPTION_P_CMD      0x95    //商品认沽期权
#define ID_BT_OPTION_OTC        0x96    //场外期权
#define ID_BT_OPTION_UDO        0x97    //用户自定义期权

#define ID_BT_BANKRATE			0xd0	//银行利率
#define ID_BT_BANKRATE_HK		0xd1	//银行利率(HK)
#define ID_BT_BANKRATE_WORLD	0xd2	//银行利率(Interal)
#define ID_BT_BANKRATE_BOND 	0xd3	//债券利率
#define ID_BT_NMETAL			0xe0	//贵金属(noble metal)
#define ID_BT_OTHERS			0xf0	//其他
#define ID_BT_OTHER_SHARES_SG_A	0xf1	//A股新股申购
#define ID_BT_OTHER_SHARES_ZF_A	0xf2	//A股增发

#define ID_BT_DIGITALCCY        0xa0	//数字货币DIGITALCCY

//证券基本分类定义
#define ID_BASECLASS_INDEX		0x00	//指数
#define ID_BASECLASS_SHARES		0x10	//股票
#define ID_BASECLASS_FUND		0x20	//基金
#define ID_BASECLASS_BOND		0x30	//债券 & 可转债
#define ID_BASECLASS_REPO		0x40	//回购
#define ID_BASECLASS_QZ			0x60	//权证
#define ID_BASECLASS_FUTURES	0x70	//期货
#define ID_BASECLASS_FOREX		0x80	//外汇
#define ID_BASECLASS_OPTIONS	0x90	//期权(2012.5.22)
#define ID_BASECLASS_DIGITALCCY 0xa0	//数字货币DIGITALCCY
#define ID_BASECLASS_BANKRATE	0xd0	//银行利率
#define ID_BASECLASS_NMETAL		0xe0	//贵金属(noble metal)
#define ID_BASECLASS_OTHER		0xf0	//其他

//证券扩展分类定义
#define ID_BTEX_HASWARRANT		0x80000000	//正股(有权证的股票)
#define ID_BTEX_DELETE			0x40000000	//已经删除
#define ID_BTEX_DELISTING		0x08000000	//退市整理板块（2012.8.15）
#define ID_BTEX_RISKWARNING		0x04000000	//风险警示板块（2012.11.30, 原ST板块）
#define ID_BTEX_NOSSEBOUND		0x02000000  //
#define ID_BTEX_BBQBOUND		0x10000000  //BBQ BOUND
#define ID_BTEX_ETFIOPV			0x20000000  //ETF IOPV




//报价栏目类型
#define ID_PRICETYPE_SHARES		0x00	//股票
#define ID_PRICETYPE_FUND		0x01	//基金
#define ID_PRICETYPE_BOND		0x02	//债券
#define ID_PRICETYPE_CBOND		0x03	//转债
#define ID_PRICETYPE_REPO		0x04	//回购
#define ID_PRICETYPE_QZ			0x05	//权证
#define ID_PRICETYPE_INDEX		0x06	//指数
#define ID_PRICETYPE_FUTURES	0x07	//商品期货
#define ID_PRICETYPE_FOREX		0x08	//外汇
#define ID_PRICETYPE_MB			0x09	//牛熊证
#define ID_PRICETYPE_NMATEL		0x0a	//贵金属
#define ID_PRICETYPE_IDXFUTURES 0x0b	//指数期货
#define ID_PRICETYPE_DEF1		0x10	//用户定义1
#define ID_PRICETYPE_DEF2		0x11	//用户定义2
#define ID_PRICETYPE_DEF3		0x12	//用户定义3
#define ID_PRICETYPE_DEF4		0x13	//用户定义4

//板块分类定义
#define ID_ROOTCLASS_SYSTEM			0x00000000		//系统分类
#define ID_ROOTCLASS_INDUSTRY		0x00000001		//行业分类
#define ID_ROOTCLASS_AREA			0x00000002		//地区分类
#define ID_ROOTCLASS_BLOCK			0x00000003		//板块
#define ID_ROOTCLASS_INVEST			0x00010004		//投资品种
#define ID_ROOTCLASS_SELFSEL		0x00020005		//自选品种
#define ID_ROOTCLASS_COMBO			0x00030006		//组合品种
#define ID_ROOTCLASS_INDEXBLOCK		0x00030007		//分类指数
#define ID_ROOTCLASS_INDEX			0x00040007		//样本股指数
#define ID_ROOTCLASS_INDEXCOMBO		0x00050008		//投资组合
#define ID_ROOTCLASS_HZ				0x00080009		//板块分析"


//系统分类表
/*struct HHTradeData_SystemClassTable
{
	int64_t  nMarket;
	int  nPriceType;
	char sName[24];
	char sNote[32];
	int  nGrade;
	char sSet[8];
	int  nID;
	int  nTypeEx;
	GUID guid;
};*/

struct HHNsdSystemSet_BaseTypeDef
{
	int    nType;			//类型
	float  fSXFee;			//手续费率
	float  fYHFee;			//印花税率
	float  fGHFee;	 		//过户费
	float  fQTFee;	 		//其它费用
	float  fMinFee;	 		//最小费用
	float  fExchRate; 		//汇率exchange rate
	float  fPriceRate;		//换算系数
};


struct HHSystemSet_IndexDefine
{
	int    nRecord;		//指数记录号
	char   sCode[16];	//代码
	char   sName[16];	//名称
	char   sCodeOrg[8];	//交易所代码
	char   cTjFlag;		//统计标志('K'读库)
	char   sTjType[16];	//统计范围
	char   sTjCode[11];	//成交量统计代码
};

struct HHNsdSystemSet_BaseTypeSet
{
	char   sTypeName[12];	//代码分类名称
	int    nType;			//类型
	int    nVol;			//成交量单位
	int    nMinVol;			//成交量单位
	int    nDec;			//报价小数点位数
	char   sTypeDef[95];	//代码分类定义
	float  fSXFee;			//手续费率
	float  fYHFee;			//印花税率
	float  fGHFee;	 		//过户费
	float  fQTFee;	 		//其它费用
	float  fMinFee;	 		//最小费用
	float  fExchRate; 		//汇率exchange rate
	float  fPriceRate;		//换算系数
};

struct exchange_time		//交易时间(此结构不可修改，因为在数据保存、传送中有使用)
{	
	short times;			//交易时间节数
	short timedif;			//时差（该市场的交易时段所用的时区当天和东八区之间时间差的分钟数。如美国东部时区为-5，交易所采用夏令时，在夏天美国的交易所timediff填”-720”((-5+1-8)*60=-720)） 
	short opentime[4];		//开盘时间
	short closetime[4];		//休盘时间
};
struct exchange_time_n		//交易时间(此结构不可修改，因为在数据保存、传送中有使用)
{	
	short times;			//交易时间节数
	short timedif;			//时差
	short opentime[8];		//开盘时间
	short closetime[8];		//休盘时间
	char  chExchTimeFlags[8];
};
struct HHNsdSystemSet_DBF
{
	char   ExchFileFlag[4];
	//信息库定义	
	char   zq_field_code[12];	//代码
	char   zq_field_name[12];	//名称
	char   zq_field_csclass[12];//行业类别
	char   zq_field_cfclass[12];//成分标志
	char   zq_field_fxgb[12];	//发行股本
	char   zq_field_ltgb[12];	//流通股本
	char   zq_field_mgsy[12];	//每股收益
	char   zq_field_highLim[12];//当日涨停价
	char   zq_field_lowLim[12];	//当日跌停价
	//行情库定义
	long   hq_first;			//开始记录号
	char   hq_field_code[12];	//代码	
	char   hq_field_name[12];	//名称
	char   hq_field_close[12];	//前收盘
	char   hq_field_open[12];	//开盘
	char   hq_field_high[12];	//最高
	char   hq_field_low[12];	//最低
	char   hq_field_new[12];	//最新
	char   hq_field_volume[12];	//成交量
	char   hq_field_money[12];	//成交金额
	char   hq_field_syl[12];	//市盈率
	char   hq_field_bs[12];		//成交笔数
	char   hq_field_syl2[12];	//市盈率2
	char   hq_field_hold[12];	//和约持仓量

	//买卖盘库
	char   hq_field_sale[10][12];	//叫卖价
	char   hq_field_buy[10][12];	//叫买价
	char   hq_field_salevol[10][12];//叫卖量
	char   hq_field_buyvol[10][12];	//叫买量

	//指数库定义
	char   idx_field_code[12];	//代码
	char   idx_field_close[12];	//前收盘
	char   idx_field_open[12];	//开盘
	char   idx_field_high[12];	//最高
	char   idx_field_low[12];	//最低
	char   idx_field_new[12];	//最新
	char   idx_field_volume[12];//成交量
	char   idx_field_money[12];	//成交金额
};

struct HHNsdSystemSet_SSE
{ 
	BOOL   bEnable;  
	BOOL   bInterior;
	char   ExchangeName[20];    				//交易所全称
	char   ExchangeNameCJ[4];  					//交易所简称1
	char   ExchangeNameCJ2[9];  				//交易所简称2
	char   ExchFileFlag[4];     				//系统文件标志 SZ:深圳 SH:上海
	char   bLocalBackup;						//本地备份
	char   mmpitems;							//买卖盘个数
	char   mmpflags;							//买卖盘标志

	long   nBaseTypes;							//分类数量
	HHNsdSystemSet_BaseTypeDef BaseType[32];	//品种费率
	
	exchange_time ExchTime;						//交易时间

	char   Backup_1Minute[9][64];				//1分钟数据备份目录
	char   Backup_Tick[9][64];					//Tick数据备份目录

	char   BaseDataTxtCopy[12];					//外部文本资料拷贝标志
	char   BaseDataTxtName[10][20]; 			//外部文本资料名称
	char   BaseDataTxtPath[10][60]; 			//外部文本资料路径

	char   bFJIndexFlag;						//附加指数标志
	char   strFJIndexFile[71];  				//附加指数库名称

	char   ZQ_File[2][64];						//信息库
	char   HQ_File[2][64];						//行情库
	char   IDX_File[2][64];						//指数库

	BOOL   bCheckHQMode;						//启用备份行情库方式 0, 1, 2
	int    nCheckTime;							//行情变化检测时间(秒) BOOL   checkvolume; //成交量容错标志
	BOOL	checkvolume;						//成交量容错标志

	char   bCheckFirstRecord[2];				//检测行情库第一条记录变化

	char   sRecv[62];							//保留 

	int    nIndexDefines;
	int	   nIndexDefinesPointer;				//HHSystemSet_IndexDefine *pIndexDefine;

	int    nBaseTypeSets;
	int	   nBaseTypeSetsPointer;				//HHNsdSystemSet_BaseTypeSet *pBaseTypeSet;

	int	   nExcludeCodePointer;					//char   *pExcludeCode;
	int	   nDBFSetPointer;						//HHNsdSystemSet_DBF	*pDBFSet;				//数据库定义

	//为兼容32位和64位, 将指针定义到最后
	HHSystemSet_IndexDefine *pIndexDefine;
	HHNsdSystemSet_BaseTypeSet *pBaseTypeSet;
	char   *pExcludeCode;
	HHNsdSystemSet_DBF	*pDBFSet;				//数据库定义
};


#endif