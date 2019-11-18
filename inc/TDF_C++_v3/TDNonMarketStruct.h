#ifndef _TDNONMARKETSTRUCT_H
#define _TDNONMARKETSTRUCT_H

/*
���з��г��������ݣ�ʹ��tdfapi30��Ŀ�µ�ͷ�ļ�TDNonMarketStruct.h����ͬԼ��,��������TDNonMarket�����ռ䣬VSSֱ��include����ļ�����Ҫ��ɾ�Ĳ���г�����ṹ�壬��Ҫ������ļ������֣��������ʹ�÷��г����鲢��ʹ�ýṹ�嶨��ķ�ʽ��ȡ��ҲҪinclude����ļ���ȷ��������ͬ����
*/

#pragma pack (1) 

namespace TDNonMarket
{
#if 1	//���г����鶨��������

//�۹ɷ��������ݽṹ����Ϣ��ţ���Χ0xE0-0xEF
#define VSS_HKEX_TD_CSCSHQ_INDEXVOLUME 224
#define VSS_HKEX_TD_CSCSZQ_INDEXVOLUME 225
#define VSS_HKEX_TD_STOCK_CONNECT_MARKET_TURNOVER  226
#define VSS_HKEX_TD_STOCK_CONNECT_DAILY_QUOTE_BANLANCE 227

//���ɷ��������ݽṹ����Ϣ��ţ���Χ0x90-0x9F
#define VSS_SHFAST_TD_SSE_HK_AMOUNT   144
#define VSS_SHFAST_TD_EXCHRATE_RECORD 145

//���ͨ�����Ϣ���
#define VSS_SZ_TD_HK_AMOUNT			  146

//��㽻�������쳣���ݴ���
#define VSS_CFETS_CANCEL_DEAL		  176  


// �۹�ͨ�ṹ(VSS_SHFAST_TD_SSE_HK_AMOUNT, VSS_SZ_TD_HK_AMOUNT)
struct TD_SSE_HK_Amount
{
	int		lDate;
	int		lTime;					// ʱ��
	int64_t     iThresholdAmount;       // ÿ�ճ�ʼ��ȣ���λ�����Ԫ
	int64_t     iPosAmt;                // ����ʣ���ȣ���λ�����Ԫ
	char        chAmountStatus;         // ���״̬   '1'��������ꣻ'2'����ȿ���
};

//
struct TD_SSE_HK_StockStatus
{
	char		chCode[8];				// ֤ȯ����
	char        SecTradingStatus1[8];   // �۹����ֶ���(C8):         ���ֶ�Ϊ8λ�ַ���������ÿλ��ʾ�ض��ĺ��壬�޶�������ո񡣵�1λ����0����ʾ�������룬��1����ʾ�����޴����ơ�
	// ��2λ����0����ʾ������������1����ʾ�����޴����ơ�
	char        SecTradingStatus2[8];   // �۹���ɶ���(C8):         ���ֶ�Ϊ8λ�ַ���������ÿλ��ʾ�ض��ĺ��壬�޶�������ո񡣵�1λ����0����ʾ�������룬��1����ʾ�����޴����ơ�
};

//VSS_SHFAST_TD_EXCHRATE_RECORD
struct TD_ExchRate_Record
{
	int64_t    iBuyPrice;       //�ο���������ۣ�С����5λ������100000��
	int64_t    iSellPrice;      //�ο����������ۣ�С����5λ������100000��
	int64_t    iMedianPrice;    //�ο������м�ۣ�С����5λ������100000��
	char       chCurrencyType[8];   //��������
};


//����ͨ���(VSS_HKEX_TD_CSCSHQ_INDEXVOLUME)
struct MsgTD_CSCSHQ_IndexVolume
{
	__int64    lIndexVolume;        //�ɽ���
	int     nDate;                 // �ɽ����ڣ�YYYYMMDD��
	int     nTime;                 // �ɽ�ʱ�䣨�룩
};

//���ͨ���(VSS_HKEX_TD_CSCSZQ_INDEXVOLUME)
struct MsgTD_CSCSZQ_IndexVolume
{
	__int64    lIndexVolume;        //�ɽ���
	int     nDate;                 // �ɽ����ڣ�YYYYMMDD��
	int     nTime;                 // �ɽ�ʱ�䣨�룩
};

//(VSS_HKEX_TD_STOCK_CONNECT_DAILY_QUOTE_BANLANCE)
struct MsgTD_Stock_Connect_Daily_Quota_Balance 
{
	int     nDate;                 // �ɽ����ڣ�YYYYMMDD��
	int     nTime;                 // �ɽ�ʱ�䣨�룩
	char    chStockConnectMarket[3];
	char    chTradingDirection[3];

	__int64    lDailyQuotaBalance;        
	__int64    lDailyQuotaBalanceTime;         
};

//����۳ɽ���(VSS_HKEX_TD_STOCK_CONNECT_MARKET_TURNOVER)
struct MsgTD_Stock_Connect_Market_Turnover 
{
	int     nDate;                 // �ɽ����ڣ�YYYYMMDD��
	int     nTime;                 // �ɽ�ʱ�䣨�룩
	char    chStockConnectMarket[3];
	char    chTradingDirection[3];

	__int64    lBuyTurnover;        
	__int64    lSellTurnover;         
	__int64    lSumTurnover;       
};

// ��㽻�������쳣���ݳ����ṹ
struct MsgTD_CFETS_CANCEL_DEAL
{
	int		   nDate;				// ���������鷢�����ڣ�YYYYMMDD��
	int		   nCancelTime;			// ���������鷢��ʱ�䣬��ȷ����: hhmmss
	__int64    lCancelPrice;        // ����������۸�, �й̶�8λС��, Ҳ����˵, ���ݳ�10^8������ʵ��ֵ
	char       chWindCode[36];      // Wind����
};

#endif	//�ڴ�֮�ⲻҪ����κδ���
}

#pragma pack ()

#endif