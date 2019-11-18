// TDFUnSave.h : TDFUnSave DLL ����ͷ�ļ�
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
//�ɽ���־����
#define HHMATCHMONEY_HI  0xf0000000		//�ɽ�����λ
#define HHMATCHVOLUME_HI 0x0f000000		//�ɽ�����λ
#define HHBSFLAG_B		 0x00800000		//������־B
#define HHBSFLAG_S		 0x00400000		//������־S
#define HHUDFLAG_U		 0x00200000		//������־Up
#define HHUDFLAG_D		 0x00100000		//������־Down

// Tick�������Ͷ���:
#define ID_TKTYPE_VIEWDATA     -3     // �������
#define ID_TKTYPE_INFODATA     -2     // �������ݣ�ETF�嵥�ȣ�
#define ID_TKTYPE_DAY          -1     // Day
#define ID_TKTYPE_TICK         0      // Tick
#define ID_TKTYPE_MINUTE       1      // One Minute
#define ID_TKTYPE_MATCH        2      // Only Match Tick
#define ID_TKTYPE_STEP         3      // Step
#define ID_TKTYPE_ORDERQUEUE   4      // Order Queue of Top 50 for SHSE & SZSE Level2 Data
#define ID_TKTYPE_ORDER        5      // Order
#define ID_TKTYPE_BRKOERQUEUE  6      // Broker Queue of Best 40 for HK
#define ID_TKTYPE_LINKED       7      // Linked ����Ʒ��
#define ID_TKTYPE_OTC_OPTIONS_S 8      // ������Ȩ����

// ������Ϣ����
#define TDCODE_UNICODE_CODE      0x01 // ����ΪUnicode����
#define TDCODE_UNICODE_NAME      0x02 // ����ΪUnicode����
#define TDCODE_UNICODE_NAMEENG   0x04 // Ӣ������ΪUnicode����
//���ݴ����־����(iHandlFlag)
#define TD_HANDLE_MINUTE     0x01
#define TD_HANDLE_MATCH_TICK 0x02
#define TD_HANDLE_NO_KDAY    0x04
#define TD_HANDLE_ONLY_ONE_RECORD  0x08
#define TD_HANDLE_INIT_FLAG        0x10
#define TD_HANDLE_ONLY_TIME_DIF    0x20
//�����¼���־���壨nEventFlags��
#define TD_SP_STATUS_CLEAR     0x01
#define TD_SP_STATUS_NOT_TRADE 0x02
#define TD_SP_STATUS_TRANS     0x04
//�¼����
#define ID_EVENT_OPEN  1  //����
#define ID_EVENT_CLEAR 2  //����
#define ID_EVENT_CLOSE 3  //����
#define ID_EVENT_TRANS 4  //ת��
#define ID_EVENT_SAVE  5  //����
#define ID_EVENT_CLEAR_CODES              6  //���ִ�������
#define ID_EVENT_SINGLE_CODE_CLEAR_NOTTIY 7  //������������֪ͨ
#define ID_EVENT_HEARTBETA                8  //�����г���������
#define ID_EVENT_CODE_HEARTBETA           9  //������������
#define ID_EVENT_TRANS_CODES              10 //���ִ���ת��
#define ID_EVENT_VSS_HEARTBETA            11 //ת����̶�ʱ�䷢��������
#define ID_EVENT_CANCEL                   12 //�����¼�
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
	int nResv[10];//Ԥ��40�ֽ�
};
struct TDFMarketExchangeSet
{ 
	char   chExchangeName[32];    // ������ȫ��
	char   chExchangeNameCJ[4];   // ���������1
	char   chExchangeNameCJ2[12]; // ���������2
	char   chMarket[4];           // ��������־:SZ;SH;CF;DCE;SHF;CZC;HK;HKF;O;N
	int    nMarketID;             // �г�ID
	int    nFlags;                // ��־λ��������: VOLUME_BACKWARD:�Ƿ�����������
	int    nMDFlags;              // MarketData���ݴ����־(0:ί�ж���ͬMDͬʱ���£�1:���ø���)
	int    nAskBidDeeps;          // �����̸���
	int    nAskBidFlags;          // �����̱�־(0x80:�ж�������)
	int    nDataLevel;            // ���ݼ���
	int    nTestFlag;			  // ����/�������ݱ�־
    int    nExchItems;            // ����ʱ�����
    int    nTimeDif;              // ʱ����ӣ�
    short  nBeginTime[8];         // ���׽ڿ�ʼʱ�䣨��������
    short  nEndTime[8];           // ���׽ڽ���ʱ�䣨��������
	char   chExchTimeFlags[8];
};



//����
struct TD_Code_Packet
{
    char chCode[32];			// ���������룺 ���������������Wind����ǰ�沿�ֲ�һ�£����Ϊһ��
	char chWindCode[36];		// Wind����
    char chWindType[8];			// Wind����
    char chName[48];			// ����
    char chNameEng[48];			// Ӣ������
    int  nRecord;				// ���ձ��
    int  nType;					// ֤ȯ����
    int  nLotSize;				// ��������
    int  nQuoteUnit;			// 20170320 Add:���۵�λ���۲��Ԫ������4λС����- �͵�ǰ�ɽ���ƥ��
    int  nID;					// ID
    GUID guid;					// GUID
    char chDec;					// ��ʾС��λ
	char chVol;					// ��ʾ����(10�Ĵη�,֧��1,2,3)
	char chUnicodeFlag;			// Unicode��־[TDCODE_UNICODE_](0x01:chCode, 0x02:chName, 0x04: chNameEng)
    char chCurrencyCode[4];     // ����
	char chQualificationClass;  //���ڷ����ծȯ��Ĭ��ֵ��0.
};

union TD_Info
{	
    //��Ʊ
    struct TD_Info_Shares
    {
        char chIndustryCode[4];              // ������ҵ����
        int64_t iCapitalShares;              // ��ͨ�ɱ�
        int64_t iTotalShares;                // �ܹɱ�
		char chProfitFlag;                   // ��ʾ��Ŀǰ�Ƿ���δӯ������0:δӯ����1:��ӯ��
		char chWeightedVotingRights;         // ��ʾ���Ƿ����ͶƱȨ���족��0: �����ڣ�1: ����
    }Shares;
    //Ȩ֤
    struct TD_Info_Warrants
    {
        char chUnderlyingCode[8];            // ��Ĺ�Ʊ����
        char chStrikeCode[8];                // ��Ȩ����
        char chCPFlag;                       // ��/����־
        char chType;                         // Ȩ֤����(��ʽ/ŷʽ)
        char chStrSet;                       // ���㷽ʽ
        char chResv;
        int  nExercisePrice;                 // ��Ȩ�۸�
        int  nExerciseRation;                // ��Ȩ����
        int  nExercisePeriod;                // ��Ȩ����
        int64_t iTotalWarrantExecQty;        // Ȩִ֤�е�������(lVolumeInMarket�۹ɽֻ���)
        unsigned int nTotalWarrantCancelQty; // Ȩ֤ע��������
        unsigned int nTotalWarrantOrderQty;  // Ȩ֤����������
        int  nCallPrice;                     // ���ռ۸�(�۹�ţ��֤)
        int  nMaturityDate;                  // ����
    }Warrants;
    //ծȯ
    struct TD_Info_Bond
    {
        int  nNominalValue;          // ��ֵ
        int  nIssuingPrice;          // ���м�
        int  nCouponRate;            // Ʊ������(��תծ:��������) 
        int  nCurrentRate;           // ��������
        int  nPaymentFrequency;      // ��ϢƵ��
        int  nMaturityDate;          // ������
        int  nPaymentDate;           // ��Ϣ��
        int  nValueDate;             // ��Ϣ��
        int  nResidualMaturity;      // ʣ��ʱ��
        //��תծȯ����
        char chConversionCode[8];    // ת�ɴ���			  
        int  nConversionPrice;       // ת�ɼ۸�
        int  nConversionRation;      // ת�ɱ���
        int  nConversionPeriod;      // ת��ʱ��
		int  nIssueDate;             // ������
		uint16_t nBidSettlementSpeed;  //���������ٶ� ���� ȡֵ1��ʾ��T+0����ȡֵ2��ʾ��T+1��
		uint16_t nAskSettlementSpeed;  //���������ٶ� ���� ȡֵ1��ʾ��T+0����ȡֵ2��ʾ��T+1�� 
    }Bond;
    //�ڻ�
    struct TD_Info_Futures
    {
        char chUnderlyingCode[8];   // ��Ĺ�Ʊ����
        int  nListDate;             // ��������
        int  nEndDate;              // ��������
        int  nExchID;               // ������ID
        int  nAskDelta;             // �����۲�
        int  nBidDelta;             // ����۲�
        char chNearCode[8];         // ���ڴ���
        char chFarCode[8];          // Զ�ڴ���
        int  nNearEndDate;          // ���ڽ�������
        int  nFarEndDate;           // Զ�ڽ�������
    }Futures;
	struct TD_Info_Options				//2013.10.23 add by Paul for ShangHai Option
	{
		char chContractID[20];          // ��Ȩ��Լ����C19
		//char chSecuritySymbol[20];	// ��Ȩ��Լ���
		char chUnderlyingSecurityID[8]; // ���֤ȯ����
		char chUnderlyingType;			// ���֤ȯ����C3    0-A�� 1-ETF (EBS �C ETF�� ASH �C A ��)
		char chOptionType;              // ŷʽ��ʽC1        ��Ϊŷʽ��Ȩ�����ֶ�Ϊ��E������Ϊ��ʽ��Ȩ�����ֶ�Ϊ��A��
		char chCallOrPut;               // �Ϲ��Ϲ�C1        �Ϲ������ֶ�Ϊ��C������Ϊ�Ϲ������ֶ�Ϊ��P��
		char chPriceLimitType;          // �ǵ�����������C1 ��N����ʾ���ǵ�����������, ��R����ʾ���ǵ�����������
		int  nContractMultiplierUnit;	// ��Լ��λ,         ������Ȩ��Ϣ������ĺ�Լ��λ, һ��������
		int  nExercisePrice;            // ��Ȩ��Ȩ��,       ������Ȩ��Ϣ���������Ȩ��Ȩ�ۣ��Ҷ��룬��ȷ����
		int  nStartDate;                // ��Ȩ�׸�������,YYYYMMDD
		int  nEndDate;                  // ��Ȩ�������/��Ȩ�գ�YYYYMMDD
		int  nExerciseDate;             // ��Ȩ��Ȩ�գ�YYYYMMDD
		int  nExpireDate;               // ��Ȩ�����գ�YYYYMMDD
	}Options;
};

//2008.11.30������Ʒ��������չΪѹ������
struct TD_Links_Packet
{
    int  nLinks;          // ����Ʒ�ָ���
    int  nLinksLen;       // ����Ʒ��ѹ�����ݳ���
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

//��չ������
union TD_AB_Ext
{
    struct TD_AB_Sec
    {
        int nIEP;
        int nIEV;
        int nAskDelta;      // ί���۸��
        int nBidDelta;      // ί��۸��
    }Sec;
    struct TD_AB_Futures
    {
        int nSettlement;    // �����
        int nPositions;     // �ֲ���
        int nCurDelta;      // ��ʵ��
        int nUnderlingPrice;// ���Ʒ�ּ۸�
    }Futures;
	struct TD_AB_Bonds
	{
		int nSettlement;    // �����
	}Bond;
};

//������
union TD_AskBid
{
    struct TD_AB_Sec
    {
        union TD_AB_Ext Ext;
        
        int64_t nAskAvPrice;    // ί����Ȩ����
        int64_t nBidAvPrice;    // ί���Ȩ����
        int64_t iAskTotalVol;   // ί������
        int64_t iBidTotalVol;   // ί������
        
        int64_t nAskPrice[10];  // ί���۸�
        int64_t nBidPrice[10];  // ί��۸�
        int64_t iAskVol[10];    // ί������
        int64_t iBidVol[10];    // ί������
        int     nAskOrders[10]; // ί����������
        int     nBidOrders[10]; // ί�򶩵�����
    }Sec;
	struct TD_AB_Index
	{
		unsigned int preSZ;
		unsigned int nZgb;		// �ܹɱ�
		unsigned int nKlt;		// ����ͨ
		unsigned int nLtg;		// ��ͨ��
		unsigned int nFxl;		// ������
		int bbzsy;				// ����������
		int bbzzc;				// �������ʲ�
		int bbjzc;				// �����ʲ�
		int bbxjll;				// �����ֽ�����
		int bbgdsl;				// �ɶ�����
		int lastmgsy;			// ���±���ÿ������
		int lastseason;			// ���±��漾��
		int laststocks;			// ���±������

		int	nStocks;			// Ʒ�ָ���
		int	nZDP[3];			// �ǵ�ƽ
		uint32_t	nFxsz;	// ���йɱ���ֵ
		uint32_t	nLtsz;	// ��ͨ����ֵ
		uint32_t	nKltsz;	// ����ͨ����ֵ
		uint32_t	nZsz;	// ����ֵ

		int mmb;

		int64_t nAvPrice;			// ����ƽ����
		int64_t nIndex;				// ����Ȩָ��
		int nAskMoney;			// ί�����
		int nBidMoney;			// ί����

		char chFirstCode[5][8];	// ���ǹ�
		char chLastCode[5][8];	// �����
		int  nFirstMarkup[5];
		int  nLastMarkup[5];
		//�¼��ֶ�
		int nTransactionsNum;//�ɽ�����
		int nMaxIncrease; //����Ƿ�
		int nMaxDrawDown; //������
		//20170727:���
		int64_t iPreSettlement;
		int64_t iSettlement;
		int64_t iPrePosition;
		int64_t iPosition;
	}Indexs;
	struct TD_NoSSE_Bond{
		char    chBidQuoteOrg[32];   //�򱨼۷�
		char    chAskQuoteOrg[32];   //�����۷�
		int     nBidCleanPrice;      //�������뾻��
		int     nAskCleanPrice;      //������������
		int     nBidYield;           //�������뵽��������
		int     nAskYield;           //������������������
		int64_t iBidTradeVolume;     //�������뱨����
		int64_t iAskTradeVolume;     //��������������
		int     nBidTotalPrice;      //��������ȫ��
		int     nAskTotalPrice;      //��������ȫ��
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

//��չ����//////////////////////////////////
union TD_Exdata
{
	struct TD_Exdata_Shares
	{
		int nPERation1;         // ��ӯ��1
		int nPERation2;         // ��ӯ��2
		int nStockDividend;     // ��Ϣ��
		int nTime;              //��ǰ/�̺�ʱ��		
		int nBeforeVolume;      //��ǰ��
		int nAfterVolume;		//�̺���
		int64_t iBeforPrice;    //��ǰ�۸�
		int64_t iAfterPrice;    //�̺�۸�
		int64_t iAfterTurnover;	//�̺�ɽ���� (�ƴ�����ʹ�õ�)
		int nAfterMatchItems;   //�̺�ɽ�����
	}Shares;
	struct TD_Exdata_Indexs
	{
		int nPERation1;           // ��ӯ��1
		int nPERation2;           // ��ӯ��2
		int64_t iExtTurnover;     // ָ����չ�Ŀǰ����SZ��ָ��д��ָ���ݣ����ޣ��豣�ֺ���������һֱ
		int64_t iExtVolume;       // ָ����չ����Ŀǰ����SZ��ָ��д��ָ���ݣ����ޣ��豣�ֺ���������һֱ
	}Indexs;
	struct TD_Exdata_ETF
	{
		int nNetValue;          // ��ֵ
		int nIOPV;              // IOPV ��ֵ��ֵ(EFT)
		int nETFBuyNumber;       // ETF�깺����
		int nETFSellNumber;      // ETF��ر���
		int64_t iETFBuyAmount;   // ETF�깺����
		int64_t iETFBuyMoney;    // ETF�깺���
		int64_t iETFSellAmount;  // ETF�������
		int64_t iETFSellMoney;   // ETF��ؽ��
	}ETF;
	struct TD_Exdata_Bond
	{
		int nAccruedInterestAmount;   // Ӧ����Ϣ��
		int nYieldToMaturity;         // ���������� - ����������  259
		int nTotalYieldToMaturity;    // ������������
		int nBondGage;                // ծȯ��ֵ
		//20160712:��㽻������ȱ���ֶ�
		int nPreWeightedCleanPrice;   //ǰ��Ȩƽ������ 313
		int nWeightedCleanPrice;      //��Ȩƽ������ 310	
		int nPreClosingYield;         //ǰ���������� 511
		int nOpeningYield;            //���������� 316
		int nHighestYield;            //��������� 318
		int nLowestYield;             //��������� 317	
		int nPreWeightedYield;        //ǰ��Ȩƽ��������
		int nWeightedYield;           //��Ȩƽ�������� 512
		int nBidYield;                //��������
		int nAskYield;                //����������
	}Bond;
	struct TD_Exdata_Futures
	{
		int nPreSettlement;// ǰ�ս����
		int nPrePosition;  // ǰ�ճֲ���
		int nPreDelta;     // ǰ����ʵ��
		int	nAvgPrice;	   //���� TODO:����ʱ��
		int nDayVolume;        //�ռ���
		int nNightVolume;	   //ҹ����
		int64_t iDayPrice;     //�ռ�۸�
		int64_t iNightPrice;   //ҹ��۸�	
		int64_t iEFPVolume;
		int     nEFPPositions; 
	}Futures;
	struct TD_Exdata_FX
	{
		int64_t nVAskPricePoint;       // �������ڵ�
		int64_t nVBidPricePoint;       // ������ڵ�
		int nVBidTime;                 // �����ʱ�� HHMMSS
		int nPriceFlag;                // ����
		int nForwardLatestFullPrice;   // Զ������ȫ�� 257
		int nSwapDate;                 // ���ڵ����� 324
		int nVAskTime;                 // �����ʱ�� HHMMSS
		int nResv;                     // �����ֶ�
		int64_t iDayPrice;             // ��㽻���������̼�
	}FX;
	struct TD_Exdata_IR  //interest rate
	{
		int  nWeightedInterestRate;  // ��Ȩƽ������
		int  nPreWeightedInterestRate;  //ǰ��Ȩƽ������
	}IR;
	struct TD_Exdata_UserDefOptions
	{
		int64_t dUnderlyingPrice;		// �ּ�
		int nChange;		       // �ǵ�
		int nOptionBalance;		// Ȩ����
		int nBailBalance;		// ��֤��
		int nMaxIncome;		 // �������
		int nMaxLoss;		    // ������
		int nDelta;		        // delta
		int nGamma;		        // gamma
		int nTheta;		        // theta
		int nVega;		        // vega
		int nRho;		        // rho
	}UserDefOptions;
};

//ί�ж���////////////////////////////////////////
struct TD_OrderQueue_Packet
{
    int nAskTime;       // ����ʱ�䣨�룩
    int nAskOrders;     // ����������
	int64_t nAskPrice;  // �����۸�
    int nAskDetail[50]; // ��һ����ǰ50��
    int nBidTime;       // ����ʱ�䣨�룩
    int nBidOrders;     // ���򶩵���
	int64_t nBidPrice;  // ����۸�
    int nBidDetail[50]; // ��һ����ǰ50��
};

//���Ͷ���(HK)/////////////////////////////////////
struct TD_BrokerQueue_Packet
{
    int   nAskTime;       // ����ʱ�䣨�룩
    int   nBidTime;       // ����ʱ�䣨�룩
    int   nAskBrokers;    // �������͸���
    int   nBidBrokers;    // �������͸���
    short sAskBroker[40]; // ����ǰ40����
    short sBidBroker[40]; // ����ǰ40����
};

//��ʳɽ�//////////////////////////////////////////
struct TD_Step_Packet
{
    int  nTime;          // �ɽ�ʱ�䣨�룩
    int  nMilliSecond;	 // �ɽ�ʱ�䣨���룩
    int64_t nPrice;         // �ɽ���
    int  nVolume;        // �ɽ���
    int  nTurnover;      // �ɽ����
    int  nAskOrder;      // ����ί�����
    int  nBidOrder;      // ����ί�����
	int  nOrder;         // ��ʳɽ����(20160104 add)
    char chTradeFlag;    // ���ױ�־(2012.2.14��Ϊ�������ڱ���BSFlag:1=B,2=S,0=��������)
    char chOrderKind;    // �ɽ����
    char chFunctionCode; // �ɽ�����
};

//���ί��///////////////////////////////////////////
struct TD_Order_Packet
{
    int  nTime;          // ί��ʱ�䣨�룩
    int  nMilliSecond;   // ί��ʱ��(����)
    int64_t nPrice;      // ί�м۸�
    int  nVolume;        // ί����
    int  nOrder;         // ί�����
	int  nBroker;        //�����̱���
    char chOrderKind;    // ί�����	
    char chFunctionCode; // ί�д���
	char chStatus;       //ί��״̬
	char chFlag;         //��־
	//20160616(nzhang): ���nBroker��chStatus��chFlag�������Ͱ��������32���ֽڣ�Ŀǰ�Ѿ������ֵ�����ܼ�������ֶΣ�
};

//��������////////////////////////////////////////////
struct TD_Minute_Packet
{
    int nDate;             // ����(2013.8.8 add by Paul ��������ҹ��֧��)
    int nMinute;           // ʱ��(����)
    int64_t nOpen;             // ����
    int64_t nHigh;             // ���
    int64_t nLow;              // ���
    int64_t nClose;            // ����
    int nMatchItems;       // �ɽ�����
    int64_t	iVolume;       // �ɽ���(������ԭʼ����)	
    int64_t	iTurnover;     // �ɽ����(ע:ָ��Ϊ�ɽ����Ϊ100Ԫ)
    int nABRate;           // ί��
    int nBSRate;           // ������
    union TDFDynData_Minute
    {
        struct TDFDynData_Minute_Index
        {
            int nIndex;    // ����Ȩָ��
            int nUps;      // ��
            int nDowns;    // ��
            int nLevels;   // ƽ
        }Index;
        struct TDFDynData_Minute_Fund
        {
            int nNetValue;	// ��̬��ֵ
        }Fund;
        struct TDFDynData_Minute_Futures
        {
            int nPositions;	// �ֲ���
            int nSettlement;// �����
            int nCurDelta;	// ��ʵ��
        }Futures;
    }ExtData;
};

//ί�ж���////////////////////////////////////////////
union TD_OrderQueue
{
	TD_OrderQueue_Packet  Order;      // ί�ж���
	TD_BrokerQueue_Packet Broker;	  // �����̶���(HK)
};

//
struct TD_Match_Packet
{
	int		nDate;        // �ɽ�����
	int     nTime;        // �ɽ�ʱ��
	int		nMilliSecond; // �ɽ�ʱ�䣨���룩
	int     nMatchItems;  // �ɽ�����
	int64_t nPrice;		  // �ɽ���
	int     nPositions;   // �ֲ���
	int     nSettlement;  // �����
	int64_t iVolume;	  // �ɽ���
	int64_t iTurnover; 	  // �ɽ����
	char	chTradeFlag;  // ���ױ�־
	char	chBSFlag;	  // BS��־
	char    chResv[2];
};

// �ļ�����ETF�嵥
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

//����ͷ//////////////////////////////////////////////
struct TDMarketData_Head
{
	char  chTDRemark[32];
	int   nHeadSize;
    int   nMagicCode;         // ��ʾ��
	int   nFlags;             // ���ݱ�־ 
	int   nStatus;            // ����״̬��0:�������� 1:������ 2:��ʷ����ת���� 3:��ʷ����ת����ϣ�
	int   nDataVersion;       // ���ݰ汾
	int   nLoadTimes;         // 
	///////////////////////////////////////////////////
    int   nMarketID;          // �г�ID
    char  chMarket[4];        // �г���־
    char  chMarketName[32];   // ����������
    char  chMarketNameEng[48];// Ӣ������
    int   nDataLevel;         // ���ݻ���(Level1��Level2)
	int   nTestFlag;          // ��������
    int   nAskBidDeeps;       // ���۸���
    int   nAskBidFlags;       // ���۱�־ 0x80:�ж�������
    int   nExchItems;         // ����ʱ�����
    int   nTimeDif;           // ʱ����г��Ľ���ʱ�����õ�ʱ������Ͷ�����֮��ʱ���ķ�����������������ʱ��Ϊ-5����������������ʱ�������������Ľ�����timediff�-720��((-5+1-8)*60=-720)�� 
							  //2016/10/19�޸�Ϊ�� ��2���ֽں�����һ������ʾ�������ʱ��(��������ʱ)�Ͷ�������ʱ���ֵ���ӵ�λ��������ֽڱ�ʾ������ʱ�䵱ǰ����ʱʱ��������ʱ��ƫ�Ʒ�������û������ʱ�Ļ��߷�����ʱʱ����0������ֽڱ���0. 
							  //2017/3/15ʱ��ƫ���������޸ģ���2���ֽڱ�ʾ��������ʱ���Ͷ�����ʱ���ֵ����������������һ��
    short nBeginTime[8];      // ���׽ڿ�ʼʱ�䣨��������
    short nEndTime[8];        // ���׽ڽ���ʱ�䣨��������
	char  chExchTimeFlags[8]; // 20160511�����׽ڱ�־
	int   nStaticsParam[8];   // ��̬ͳ�Ʋ���
    //����ʱ����Ϣ//////////////////////////////////////
    int   nDate;              // ���ݽ����գ�YYYYMMDD��
    int   nLastDate;          // ���������ڣ�YYYYMMDD��
    int   nLastTime;          // ������ʱ�䣨�룩
    int   nLastMilliSecond;   // ������ʱ�䣨���룩
    //����������Ϣ//////////////////////////////////////
    int   nDataType;          // ��������
    int   nItemSize;          // �������С
    int   nItems;             // ���ݼ�¼��
    int   nTickSize;          // Tick���ݴ�С		
    int   nTickPacketSize;    // Tick���ݿ��С	
    int   nTickBlocks;        // �Ѿ������Tick���ݿ����
    int   nIndexs;            // ָ������
	int   nCRC;
	//֤ȯ���Ʊ仯������������֪ͨ�ͻ������¼�������
    int   nStkNameChangeCountForClient;
    //Save������Ϣ���ϵ㴫�ͱ�����Ϣ��//////////////////
	//����
	//   int   nCodeOffset;        // ���뿪ʼλ��
	//   int   nCodeSize;          // ������Ϣ
	//   int   nCodeItems;         // �������
	//   int   nCodeCRC;           // CRC
	////Ԥ�����ļ�
	//   int   nFileOffset;        // Ԥ�����ļ���ʼλ��
	//   int   nFileSize;          // Ԥ�����ļ��ܴ�С
	//   int   nFileItems;         // Ԥ�����ļ�����
	//��ѹλ��
    int64_t   nProcessedSize;     // �Ѵ������ݴ�С

	// BreakPoint ID
	int nBreakPointID;
	char  chAuthor[16];       // �����Ȩ��

	//����Ϊ�Զ�����;�ֶ�
	int64_t iExternFlags;       //20160528 �ⲿʹ�����ݱ�־����iResvDataȡ1��
	int64_t iResvData[15];
};

//�������////////////////////////////////////////////
//NOTE�� ���м۸�chDec����.����4λС�������м۸�Ϊԭʼ�۸����10000
struct TDMarketData_Packet
{
	//������Ϣ
    char    chCode[32];        // ����������
	char    chWindCode[36];    // Wind����
    char    chWindType[8];     // Wind����
    char    chName[48];        // (ԭʼ)����
    char    chNameEng[48];     // Ӣ������
	int     nRecord;           // ���ձ��

	GUID    guid;              // ����ָ��guid
    int     nID;               // ����ָ�����
    int     nType;             // ����
	int     nQuoteUnit;        // ���۵�λ��Ԫ������4λС����
    int	    nLotSize;          // ��������
    char	chDec;             // �۸�С��λ; С�ڵ���4ͳһ��4����!
	char    chVol;             // �ɽ�����ʾ����(10�Ĵη�:֧��0,1,2,3)
	char    chUnicodeFlag;     // Unicode��־[TDCODE_UNICODE_](0x01:chCode, 0x02:chName, 0x04: chNameEng)
    char    chCurrencyCode[4]; // ����


	//����ʱ��
    char	nExchItems;        // ��������ʱ�����Ŀ
	int     nTimeDif;          // ʱ����г��Ľ���ʱ�����õ�ʱ������Ͷ�����֮��ʱ���ķ�����������������ʱ��Ϊ-5����������������ʱ�������������Ľ�����timediff�-720��((-5+1-8)*60=-720)��
	char	chExchTimeFlags[8];// 20160511�����ӽ���ʱ���״̬��ע�⣺1)Ϊ���ݣ�ʱ��������2�Σ�ǰһ�κ���ǰ������ͬ����������ʱ���),��������nExchItems�� �ڶ�����������Σ�����chExchTimeFlags��Ӧλ��ʾ��������nExchItems
    short   nBeginTime[8];     // ����ʱ��
    short   nEndTime[8];       // ����ʱ��

	// Tick���ݱ����¼
	int     nTickItems[12];     // Tick���ݸ���
	int     nTickSize[12];      // Tick���ݴ�С
	int     nTickHead[12];      // Tick����ͷλ��
	int     nTickTail[12];      // Tick����βλ��
                               // �������Ͷ���:
                               // ID_TKTYPE_TICK         0 Tick
                               // ID_TKTYPE_MINUTE       1 One Minute
                               // ID_TKTYPE_MATCH        2 Only Match Tick
                               // ID_TKTYPE_STEP         3 Step
                               // ID_TKTYPE_ORDER        4 Order
                               // ID_TKTYPE_ORDERQUEUE   5 Order Queue
                               // ID_TKTYPE_BROKERQUEUE  6 Broker Queue
                               // ID_TKTYPE_LINKED       7 Linked ����Ʒ��

	//���ͱ�־(TDϵͳ����ϵ����ݴ����￪ʼ)
	int     nTranslateFlag;
    
	int     nDate;                 // �ɽ����ڣ�YYYYMMDD��
    int     nTime;                 // �ɽ�ʱ�䣨�룩
    int     nMilliSecond;          // �ɽ�ʱ�䣨���룩
	int     nFlags;                // ��־(����)
    int	    nTickFlag;             // Tick�ǵ���־

	//���ױ�־
    char	chStatusFlag;          //����״̬��־
									//  ��������
									//0 ��������
									//1 �����¹�
									//2 �������۷���
									//3 �������۷���
									//A ���׽�����
									//B ����ͣ��
									//C ȫ������
									//D ��ͣ����
									//d ���Ͼ��۽׶ν������������۽׶ο�ʼ֮ǰ��ʱ�Σ����У�
									//E START - ����������
									//F PRETR - ��ǰ����
									//G DEL   - ���ɻָ����׵��۶Ͻ׶Σ��Ͻ�����N��  
									//H HOLIDAY - �ż�
									//I OCALL - ���м��Ͼ���
									//J ICALL - ���м��Ͼ���
									//K OPOBB - ���ж�����ƽ��ǰ��
									//L IPOBB - ���ж�����ƽ��ǰ��
									//M OOBB  - ���ж�����ƽ��
									//N IOBB  - ���ж�����ƽ��
									//O TRADE - �������;������������
									//P BREAK - ����
									//Q VOLA  - �������ж�
									//q �ɻָ����׵��۶�ʱ��(�Ͻ�����M)
									//R BETW  - ���׼�
									//S NOTRD - �ǽ��׷���֧��
									//T FCALL - �̶��۸񼯺Ͼ���
									//U POSTR - �̺���
									//V ENDTR - ��������
									//W HALT  - ��ͣ
									//X SUSP  - ͣ��
									//Y ADD   - ������Ʒ
									//Z DEL   - ��ɾ���Ĳ�Ʒ
									//v VCM   -  �г��������ڻ���(�۽�����20160503
									//        - ���侲�ڡ����������£�1)�侲��Ϊʱ5���ӣ�����Ҫ�ڼ۸�Χ�ڽ��ף�����׻ָ����� 2)�侲���ڣ���Ӱ��ĸ����Ʊ���ڻ���Լ�����ڹ̶��۸������ڣ���Ʊ���ڻ���Լ����10������5%�����н���
									//< ������ǰ
									//> �����̺�
									//c (Сдc)���о��۽���(Closing Auction Session (CAS))
	char    chTradeFlag;           // ���ױ�־
    char    chOrderIndicator;      // ��ʾ��õ��򷽻������������������ڼ��Ͼ��۽׶λ����������׽׶�(A-���Ͼ��� C-�������� N-��)
    char    chVolaIndicator;       // �������ж�(B-�Ѿ����� P-Ǳ�� V-�������ж� N-�޲������ж� F-���� X-�ӳ��Ĳ������ж�)
    char    chDividenIndicator[4]; // ��Ȩ��Ϣ��־(XD-�ֽ�ֺ� XR-��� DR-�ֽ������ ' '-�޷ֺ�����)
    char    chPrefix[4];           // ����ǰ׺
    char    chFreeText[45];        // �����ı�
	char	chFreeTextCount;       // �����ı��䶯������
    char	chMatchFlag;           // �ɽ���־�����㣩
	char    chQualificationClass;
	//char    chFreeText1[48];       // �����ı�1,Ŀǰ����������ͨ��Ʊ״̬

	//��������
	int64_t iPreVolume;       // ǰ�ɽ���
	int64_t iHandleFlag;      // ���ݴ����־Ŀǰ��3����TD_HANDLE_MINUTE(0x01) :��ʾ�˱������������߼��㣻TD_HANDLE_MATCH_TICK(0x02)  :��ʾ�˱��������MATCH_TICK����; TD_HANDLE_NO_KDAY(0x04):��ʾ�˱ʿ��ղ�������������k��
	int64_t iAvgPrice;
    int64_t nPreClose;	      // ǰ����
	int64_t nHighLimit;       // ��ͣ��
    int64_t nLowLimit;        // ��ͣ��
	int     nTradingDate;
	int     nEventFlags;           //���ŵ�8��bit��Ϊ���λ: �����¼���־����:TD_SP_STATUS_CLEAR 0x01; TD_SP_STATUS_NOT_TRADE 0x02; TD_SP_STATUS_TRANS 0x04

	int64_t nOpen;            // ���̼�            <----------+
    int64_t nHigh;            // ��߼�                       |
    int64_t nLow;             // ��ͼ�                       |
    int64_t nMatch;           // �ɽ���                       |
	int     nMatchItems;      // �ɽ�����                     |
	int64_t nAuctionPrice;    // �������жϲο���             |
	int     nAuctionQty;      // �������жϼ��Ͼ�������ƥ���� |
	int64_t iVolume;          // �ɽ���(ָ���ĵ�������100��)  | �˶����ݲ����������
    int64_t iTurnover;        // �ɽ���(ָ���ĵ�������100Ԫ)  | ѹ���㷨�����������ݱȽ�
	int64_t iBuyVolume;       // ����(��������)             |
    int64_t iSaleVolume;      // ����(���������)             |
    TD_AskBid     ABData;     // ί��ί������      <----------+
    TD_OrderQueue OrderQueue; // ί�ж���
    TD_Exdata     Exdata;     // ��չ����
    TD_Info       Info;       // ������Ϣ

	// Tickͳ������
	int64_t nPrePrice;        // ��һ�ʳɽ���
	int     nCurrentVol;      // ����
	// ��������ͳ��
	TD_Minute_Packet Minute;
	TD_Minute_Packet PreMinute;
	// ����Tick����ѹ��/��ԭ������
    TD_Step_Packet   Step;  // ��ʳɽ�
    TD_Order_Packet  Order; // ���ί��
	TD_Match_Packet  Match; // �ɽ�Tick
};


//TDF Datafeed �������Ͷ���
#define ID_TDFDATATYPE_SNAPSHOT      1  // �������
#define ID_TDFDATATYPE_ORDERQUEUE    2  // ί�ж���
#define ID_TDFDATATYPE_STEP          3  // �𲽳ɽ�
#define ID_TDFDATATYPE_MARKETVIEW    4  // �г�����
#define ID_TDFDATATYPE_ORDER         6  // ��ί��
#define ID_TDFDATATYPE_ADDCODE       9  // ����֤ȯ����
#define ID_TDFDATATYPE_NAMECHANGE    10 // ֤ȯ���Ʊ��
#define ID_TDFDATATYPE_VDECHANGE     11 // ����Դ�����L2-��L1��L1-��L2��
#define ID_TDFDATATYPE_TRADINGTIME   12 // ����ʱ��
#define ID_TDFDATATYPE_FILE          13 // �ļ�
#define ID_TDFDATATYPE_NON_MD_MSG    14 // ��������������
#define ID_TDFDATATYPE_MARKET_EVENT  15 // �г��¼�֪ͨ
#define ID_TDFDATATYPE_CODE_EVENT    16 // �����¼�֪ͨ
#define ID_TDFDATATYPE_OTC_OPTION    17 // ������Ȩ
#define ID_TDFDATATYPE_QUOTEUNITCHANGE    19 // �۲���
//#define ID_TDFDATATYPE_CODE_CLEAR_LIST 18 //���̴����б�
//#define ID_TDFDATATYPE_SSE_HKAMOUNT		14 // �Ͻ����۹�ͨ���
//#define ID_TDFDATATYPE_SSE_HKSTKSTATUS	15 // �Ͻ����۹�ͨ��Ʊ״̬
//#define ID_TDFDATATYPE_EXCHRATE			16 // �ο�����






struct TDFDynData_DataFeed_Packet
{
	int  nDFType;
    int  nRecord;                // ���ձ��
    int  nType;                  // ��������: 
                                 // ID_TDFDATATYPE_SNAPSHOT    1 �������
                                 // ID_TDFDATATYPE_ORDERQUEUE  2 ί�ж���
                                 // ID_TDFDATATYPE_STEP        3 �𲽳ɽ�
                                 // ID_TDFDATATYPE_MARKETVIEW  4 �г�����
                                 // ID_TDFDATATYPE_ORDER       6 ��ί��
                                 // ID_TDFDATATYPE_ADDCODE     9 ����֤ȯ����
                                 // ID_TDFDATATYPE_NAMECHANGE  10 ��ȷ���Ʊ��
                                 // ID_TDFDATATYPE_VDECHANGE   11 ����Դ�����L2-��L1��L1-��L2��
                                 // ID_TDFDATATYPE_TRADINGTIME 12 ������ʱ��
    int  nSide;                  // ���ݷ���(��nType==ID_TDFDATATYPE_SNAPSHOT��nSide&0x01������б仯��nSide&0x02������б仯)
    int  nBegin;                 // ��ʼλ��(���ί�С���ʳɽ��Ŀ�ʼ��¼��)
    int  nItems;                 // ���ݸ���(���ί�С���ʳɽ��ļ�¼����)
    int  nItemSize;              // ���������ݳ���
    const TDMarketData_Packet *pMDData; // �������
    void *pData;                 // ������������
};


//SaveData Head
struct TDFSaveData_Head
{
	int  nCodeOffset;	// ���뿪ʼλ��
	int  nFlags;		// ���ݱ�־
	char chMarket[4];	// �г���־
	int  nDate;			// ��������
	int  nMarketID;		// �г�ID
	int  nDataSize;		// �ܴ�С

	int  nCodeSize;		// ������Ϣ
	int  nCodeItems;	// �������
	int  nCodeCRC;		// ����CRC

	int  nFileOffset;	// �ļ���ʼλ��
	int  nFileSize;		// ����ļ��ܴ�С
	int  nFileItems;	// �ļ�����

	GUID guidData;		// ����GUID

	//�ֶ��ڴ���С
	int  nSaveBlockSize[4];
	int  nDataVer;		// 20160929 SaveЭ��汾����ǰ�汾Ϊ2.000(2000)
	int  nResv[3];      //20160630,2,3�ֽ���չΪ�°汾Save��nDataSize


	//����ʱ��
	int  nTimeDif;				// ʱ����г��Ľ���ʱ�����õ�ʱ������Ͷ�����֮��ʱ���ķ�����������������ʱ��Ϊ-5����������������ʱ�������������Ľ�����timediff�-720��((-5+1-8)*60=-720)�� 
	int  nExchTimes;			// ����ʱ�����
	int  chTimeFlags[2];        // ���׽ڱ�־
	int  nBeginTime[8];			// ��ʼʱ��
	int	 nEndTime[8];			// ����ʱ��
	//�ֶα����¼
	int  nSaveDataTimeDelta;	// ����ʱ����(ÿ5���ӱ���һ��)
	int  nSaveDataTimeNext;		// �´α���ʱ��
	int  nSaveDataTimes;		// �ѱ������
	int	 nSaveDataTime[24*12];	// ʱ��
	int  nSaveDataOffset[24*12];// ����ƫ��
	//Add 20160202 Danny
	int  nNonMDMsgDefOffset; 	// ����������������Ϣ��ʼλ��
	int  nNonMDMsgDefItems; 	// ����������������Ϣ����
	int  nNonMDMsgDefSize;		// ����������������Ϣ�ܴ�С
	//char chMarketType[64];      // 20161117:����г�����,�Ƿ�һ�����г������Ϊ�գ���Ϊ�������ͣ�����Ϊ��һ����  --���г������ͣ������ͣ���������������
};

struct TDSaveDataCode_Head
{
	int   nHeadSize;
	int   nDate;
	int   nItems;
	int   nSize;
	int   nCRC;
	int   nLanguage;	//���ִ���
	char  nAskBidDeeps;
	char  nAskBidFlags;
	char  minidataFlag;	//С�����־
	char  nFlags;		//0x01:AB200
	exchange_time ExchTime;
	short nStaticsParam[8];
	char  hhauthor[16];
	int   nMarketID;
	char  chMarket[4];
	GUID  guidData;
	int  nTimeDif;				// ʱ����г��Ľ���ʱ�����õ�ʱ������Ͷ�����֮��ʱ���ķ�����������������ʱ��Ϊ-5����������������ʱ�������������Ľ�����timediff�-720��((-5+1-8)*60=-720)�� 
	int  nExchTimes;			// ����ʱ�����
	int  chTimeFlags[2];        // ���׽ڱ�־
	int  nBeginTime[8];			// ��ʼʱ��
	int	 nEndTime[8];			// ����ʱ��
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

//TDB ���ݶ���

// K�����ڶ���
#define ID_KCYC_TICK         0  // Tick��
#define ID_KCYC_TICKBAR      1  // TickBar��
#define ID_KCYC_XSECONDS     2  // X����
#define ID_KCYC_XMINUTES     3  // X������
#define ID_KCYC_DAY          6  // ����
#define ID_KCYC_WEEK         7  // ����
#define ID_KCYC_MONTH        8  // ����
#define ID_KCYC_SEASON       9  // ����
#define ID_KCYC_HALFYEAR     10 // ������
#define ID_KCYC_YEAR         11 // ����
#define ID_KCYC_XDAYS        12 // X����
#define ID_KCYC_XWEEKS       13 // X����
#define ID_KCYC_XMONTHS      14 // X����

// ��Ȩ/��Ȩ
#define ID_KDATAFLAG_ORGDATA  0 // ԭʼ����
#define ID_KDATAFLAG_CQ       1 // ��Ȩ
#define ID_KDATAFLAG_FQ       2 // ��Ȩ

// Tick����
struct TD_Tick_Packet
{
	int	      nDate;        // ����
	int	      nTime;		// ��ȷ����(time&0x1ffff)  0x00800000=B 0x00400000=S
	int	      nMillisecond; // ����
	int	      nMatchItems;	// �ɽ�����
	int64_t	  nOpen;        // ����
	int64_t	  nHigh;		// ���
	int64_t	  nLow;			// ���
	int64_t	  nPrice;		// �۸�
	int64_t	  nHighLimit;   // ��ͣ��
	int64_t	  nLowLimit;    // ��ͣ��
	char      chTradeFlag;	// ���ױ�־	
	char	  chStatusFlag;  //״̬��־
	int64_t   iVolume;      // �ɽ���
	int64_t   iTurnover;	// �ɽ��� 1Ԫ|100Ԫ	
	int64_t   iInside;      // ����
	int64_t   iOutside;     // ����
	TD_AskBid ABData;
	TD_Exdata ExtData;
};

struct TDBar_K_Head 
{
	int  nCyc;	                  // ��������
	int  nCyc1;		              // ���ݱ�־	
	int  nCQFlag;	              // ��Ȩ��־����ȨID_KDATAFLAG_���壩
	int  nFlagEx;                 // ��չ���ݱ�־��ծȯ:0:���� 1:ȫ�ۣ�
	int  nGetMode;                // 0:��������ݸ�����ȡ  1:�����ڶζ�ȡ 2:��nBeginDate��ʼ����nLastItems��
	int  nLastItems;              // ������ݸ�����nGetMode==0��
	int  nBeginDate;              // ��ʼ���ڣ�nGetMode==1��
	int  nEndDate;	              // �������ڣ�nGetMode==1��
	int  nDynDate;                // ��̬��������
	int  nCQStartDate;            // �Ӵ����ڿ�ʼ��Ȩ
	// ������������K���м�����������ʱ����
	BOOL bNewDataFlag;            // �����������ݣ�TRUE�����룩
	const TDMarketData_Packet *pMDData; // ����������գ����߻�����ߣ�
	TD_Minute_Packet    *pMinute; // ��̬���ݸ����������ߣ�
	int  nMinutes;
	TD_Tick_Packet      *pTick;   // ��̬���ݻ��棨���߻�����ߣ�
	int  nTicks;
};
struct TDEventInfo{
	int64_t market_time;
	int64_t vss_time;
	int nEventCode;
};
struct TDUserDefineEventInfo{
	int nEventCode;
	uint8_t* buf; //���ͷ�
	uint32_t len;
};
struct TDClearCodes{
	int64_t time;
	unsigned int nRecordLen;
	int* records;  //���ͷ�
};

// K������
struct TDBar_K_Packet
{
	int nDate;         // ����
	int nTime;         // ʱ��
	int nStatus;       // ״̬
	int nMatchItems;   // �ɽ�����
	int64_t nPreClose;     // ǰ����
	int64_t nOpen;         // ���̼�
	int64_t nHigh;         // ��߼�
	int64_t nLow;          // ��ͼ�
	int64_t nClose;        // ���̼�
	int64_t nAVPrice;      // ����
	int64_t iVolume;   // �ɽ���
	int64_t iTurnover; // �ɽ���
	int     nFlags;     //���λ��0x01:�Ƿ�����Ƶ����
	union TD_K_Ext     // ��չ����
	{
		struct TD_K_Index_Packet
		{
			int nStocks;       // ��������
			int nUps;          // ���Ǹ���
			int nDowns;        // �µ�����
			int nLevels;       // ��Ʒ����
		}Index;
		struct TD_K_Futures_Packet
		{
			int nPreSettlement;// ǰ����
			int nSettlement;   // �����
			int nPositions;    // �ֲ���
			int nDelta;
		}Futures;
		struct TD_K_Fund_Packet
		{
			int nNetValue;     // ��ֵ
		}Fund;
		struct TD_K_Bond_Packet
		{
			int nInterest;     // ��Ϣ
		}Bond;
	}Ext;
};

struct TDBar_DirSet_Packet
{
	int nType;              // �������ͣ�ID_TKTYPE_
	int nBeginYear;         // ��ʼ�꣨������Ч������ֻ������һ��Ŀ¼�У��������ݰ����Ŀ¼��
	int nEndYear;           // �����꣨������Ч��
	char chDir[_MAX_PATH]; // ��Ŀ¼
	                        // ����d:\td����ʾ:2010������L2����tick����Ŀ¼Ϊd:\td\data2\tick\sz\sz2010
	                        //            ��ʾ:2010������L1����tick����Ŀ¼Ϊd:\td\data\tick\sz\sz2010
};

//Tick�����ļ�ͷ��Ϣ
struct TDBar_Save_Head
{
	char	chRemark[15];   // ʶ��
	char    nSave;			// ����
	char	chCode[8];      // ����
	char	chMarket[4];    // �г�
	char	endflag;
	char    compressflag;	// ѹ����־
	char	filetype;
	char    type;
	int     tsflag[4];
	int     nItems;			// �ܼ�¼
	int     nIndexMax;		// Index����¼��
	int     nIndexSize;		// Index��¼��С
	int     nIndexOffset;	// Index���ݴ��λ��
	int     nDataOffset;	// ���ݴ��λ��
};

struct TDBar_Save_Index
{
	int nDate;		     // ��������
	int	nOffset;	     // ���ݴ��λ��
	int nItems;		     // ��������
	int nSize;		     // ���ݱ����С
};

/**
* ����Ŀ¼�ṹ
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
	char chVer;		//�汾��, 2008.01.02����Ϊ1, ��������*10000
	int32_t tsFlag[4];
	char chMarket[4];
	int  nType;
		 
	char chFlag;	//��־
	char bSysData;	//ϵͳ����
	char bCycFlag;	//���ڱ�־ 0:�� 'W':�� 'M':�� 'S':�� 'Y':��
	char bOneDays;	//һ������
	int  nLastDays;	//�������
	     
	int  nBegin;
	int  nEnd;
	int  nDays;		//������
	     
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
    char chCode[32];         // ����
    char chName[48];         // ����
    int  nType;              // ֤ȯ����
    int  nLotSize;           // ��������
    char chDec;              // ��ʾС��λ
	char chVol;              // ��ʾ����
	char chUnicodeFlag;      // Unicode��־[TDCODE_UNICODE_](0x01:chCode, 0x02:chName, 0x04: chNameEng)
    char chCurrencyCode[4];  // ����

	int  nExchangeDays;		 // ʱ��ν�������
	int  nStatus;			 // ״̬
	//��������
	int64_t iPreVolume;	//��һ�������ճɽ���
	int64_t nPreClose;	//ǰ��
	int64_t nOpen;		//����
	int64_t nHigh;		//���
	int64_t nLow;		//���
	int64_t nClose;		//����
	int nMatchItems;	//�ɽ�����

	int64_t iVolume;	//�ɽ���	(������ԭʼ����, ָ��Ϊ100��)
	int64_t iTurnover;	//�ɽ����	(����Ϊ1Ԫ��ָ��Ϊ100Ԫ)

	//����ͳ��
	int64_t  nHighClose;			//������̼�
	int64_t  nLowClose;			//������̼�

	//��չ����
	union TDBar_DayEx_Packet
	{
		struct indexdata
		{
			int nStocks;		//Ʒ�ָ���
			int nZDP[3];		//�ǵ�ƽ
		}Index;
		struct funddata
		{
			int nNetValue;		//����ֵ
		}Fund;
		struct bonddata
		{
			int nInterest;		//��ծ��Ϣ��
			int nYTM;			//��ծ����������
		}Bond;
		struct futuresdata
		{
			int nSettlement;	//�����
			int nPosition;		//��Լ�ֲ���
			int nPreSettlement;	//ǰ�����
			int nPrePosition;	//ǰ�ֲ���
		}Futures;
	}ExtData;
};


struct NonMDHead
{
	char chTDRemark[32];
	int  nMagicCode;         // ��ʾ��
	int  nDataSize;
	int  nMsgNum;
	int  nDataOffset;
};
// �۹�ͨ�ṹ
/*struct TD_SSE_HK_Amount
{
	int		lDate;
	int		lTime;					// ʱ��
	int64_t     iThresholdAmount;       // ÿ�ճ�ʼ��ȣ���λ�����Ԫ
	int64_t     iPosAmt;                // ����ʣ���ȣ���λ�����Ԫ
	char        chAmountStatus;         // ���״̬   '1'��������ꣻ'2'����ȿ���
};*/

/*struct TD_SSE_HK_StockStatus
{
	char		chCode[8];				// ֤ȯ����
	char        SecTradingStatus1[8];   // �۹����ֶ���(C8):         ���ֶ�Ϊ8λ�ַ���������ÿλ��ʾ�ض��ĺ��壬�޶�������ո񡣵�1λ����0����ʾ�������룬��1����ʾ�����޴����ơ�
	// ��2λ����0����ʾ������������1����ʾ�����޴����ơ�
	char        SecTradingStatus2[8];   // �۹���ɶ���(C8):         ���ֶ�Ϊ8λ�ַ���������ÿλ��ʾ�ض��ĺ��壬�޶�������ո񡣵�1λ����0����ʾ�������룬��1����ʾ�����޴����ơ�
};*/

/*struct TD_ExchRate_Record
{
	int64_t    iBuyPrice;       //�ο���������ۣ�С����5λ������100000��
	int64_t    iSellPrice;      //�ο����������ۣ�С����5λ������100000��
	int64_t    iMedianPrice;    //�ο������м�ۣ�С����5λ������100000��
	char       chCurrencyType[8];   //��������
};*/

//������Ȩ
struct TD_OTC_Option{
	int     nTime;           // �ɽ�ʱ�䣨�룩
	int     iVolume;         // �ɽ���
	int64_t nMatch;          // �ɽ���
	int64_t  nPreSettlement; // ǰ�ս����
	int64_t  nSettlement;    // ǰ�ս����
	int nPrePosition;        // ǰ�ճֲ���
	int nPosition;           // �ֲ���
	int64_t nAskPrice;       // ί���۸�
	int64_t nBidPrice;       // ί��۸�
	int  iAskVol;            // ί������
	int  iBidVol;            // ί������
};

struct TD_OTC_Option_Key{
	char chOptionType;             // ŷʽ��ʽC1        ��Ϊŷʽ��Ȩ�����ֶ�Ϊ��E������Ϊ��ʽ��Ȩ�����ֶ�Ϊ��A��
	char chCallOrPut;              // �Ϲ��Ϲ�C1        �Ϲ������ֶ�Ϊ��C������Ϊ�Ϲ������ֶ�Ϊ��P��
	int64_t  nExercisePrice;       // ��Ȩ��Ȩ��,   
};


#define OTC_SNAPSHOT_HEAD       1024
// �������Ͷ���
#define ID_TDTYPE_DATE            0x0001   // ����
#define ID_TDTYPE_TIME            0x0002   // ʱ�䣨��ʱ��ʼ��ʱ���룩
#define ID_TDTYPE_TIME_MS         0x0003   // ����
#define ID_TDTYPE_MINUTE          0x0004   // ���ӣ���ʱ��ʼ��ʱ�ķ�������
#define ID_TDTYPE_TIME_HHMMSS     0x0005   // ʱ�䣨HH:MM:SS��
#define ID_TDTYPE_TIME_HHMMSSMMM  0x0006   // ʱ�䣨HH:MM:SS.MMM��ȷ�����룩
#define ID_TDTYPE_MINUTE_HHMM     0x0007   // ���ӣ�HH:MM��

#define ID_TDTYPE_PRICE32      0x0011   // 32λ�۸����ͣ�����4λС����
#define ID_TDTYPE_PRICE64      0x0012   // 64λ�۸����ͣ�����10λС����

#define ID_TDTYPE_INT8         0x0021   // 8λ����
#define ID_TDTYPE_UINT8        0x0022   // 8λ�޷�����
#define ID_TDTYPE_INT16        0x0023   // 16λ����
#define ID_TDTYPE_UINT16       0x0024   // 16λ�޷�����
#define ID_TDTYPE_INT32        0x0025   // 32λ����
#define ID_TDTYPE_UINT32       0x0026   // 32λ�޷�����
#define ID_TDTYPE_INT64        0x0027   // 64λ����
#define ID_TDTYPE_UINT64       0x0028   // 64λ�޷�����

#define ID_TDTYPE_INT32X10N    0x002a   // 32λ����(��10**n��������ѹ����С)
#define ID_TDTYPE_UINT32X10N   0x002b   // 32λ�޷�����(��10**n��������ѹ����С)
#define ID_TDTYPE_INT64X10N    0x002c   // 64λ����(��10**n��������ѹ����С)
#define ID_TDTYPE_UINT64X10N   0x002d   // 64λ�޷�����(��10**n��������ѹ����С)

// FIXME: ƴд���� DF -> FD
/*#define ID_SDBDF_1DEC32       0x0031   // һλС��
#define ID_SDBDF_2DEC32       0x0032   // ��λС��
#define ID_SDBDF_3DEC32       0x0033   // ��λС��
#define ID_SDBDF_4DEC32       0x0034   // ��λС��
#define ID_SDBDF_1DEC64       0x0038   // һλС��(64λ)
#define ID_SDBDF_2DEC64       0x0039   // ��λС��(64λ)
#define ID_SDBDF_3DEC64       0x003a   // ��λС��(64λ)
#define ID_SDBDF_4DEC64       0x003b   // ��λС��(64λ)
#define ID_SDBDF_5DEC64       0x003c   // ��λС��(64λ)
#define ID_SDBDF_6DEC64       0x003d   // ��λС��(64λ)
#define ID_SDBDF_7DEC64       0x003e   // ��λС��(64λ)
#define ID_SDBDF_8DEC64       0x003f   // ��λС��(64λ)
*/
#define ID_TDTYPE_FLOAT        0x0041   // ������
#define ID_TDTYPE_DOUBLE       0x0042   // ˫������

#define ID_TDTYPE_CHAR         0x0051   // 1���ַ�
#define ID_TDTYPE_TEXT         0x0052   // �ı�

#define ID_TDTYPE_CURRENCY     0x0061   // ����
#define ID_TDTYPE_GUID         0x0062   // GUID
#define ID_TDTYPE_IPV4         0x0063   // IPV4 Address

// �ǽṹ������
#define ID_TDTYPE_VLTEXT       0x0071   // �ɱ䳤�ı���offset, len��
#define ID_TDTYPE_BINARY       0x0072   // �����ƣ�offset, len, items��

#define ID_TDTYPE_ICON         0x0081   // ͼ�꣨offset, len��
#define ID_TDTYPE_PICTURE      0x0082   // ͼƬ��offset, len��
#define ID_TDTYPE_VIDEO        0x0083   // ��Ƶ��offset, len, time��
#define ID_TDTYPE_AUDIO        0x0084   // ��Ƶ��offset, len, time��

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
	char chName[48]; // �ֶ�����
	int nType;       // Field Type
	int nTypeEx;     // Field Extend Type
	int nOffset;     // Data  offset
	int nWidth;      // Data  Width
	int nFlags;      // Field Flags 0x0f:С����
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
	//ģ�壿
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