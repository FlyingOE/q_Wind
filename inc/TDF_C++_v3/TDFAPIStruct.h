#ifndef __TDF_API_STRUCT_H__ 
#define __TDF_API_STRUCT_H__
#pragma  pack(push)
#pragma pack(1)

#ifndef THANDLE
typedef void* THANDLE;
#endif

#ifndef __int64
#define __int64 long long
#endif
#define MAXSERVER 4

/******************************************************************* ��Ϣͷ *********************************************************************/
struct TDF_APP_HEAD
{
	int	nHeadSize;         //����¼�ṹ��С
	int	nItemCount;	       //��¼����
	int	nItemSize;         //��¼��С
};
struct TDF_MSG
{
	unsigned short  	    sFlags;		        //16λ ��ʶ��.
	int  	                nDataType;	        //16λ �������� ��enum TDF_MSG_ID
	int			            nDataLen;	        //32λ ���ݳ��ȣ�������TDF_APP_HEAD�ĳ��ȣ�
	int			            nServerTime;		//32λ������ʱ�������ȷ������HHMMSSmmm��������ԭ��ѹ�������Ǳ���ʱ��
	int     		        nOrder;		        //32λ ��ˮ��
	int                     nConnectId;         //����ID����TDF_Open�ǣ����������趨��ֵ
	TDF_APP_HEAD*           pAppHead;	        //Ӧ��ͷ
	void*                   pData;              //����ָ��
};
struct TDF_REFDATA_MSG
{
	int                     nReqID;
	int                     errorCode;          //==0��ʾ�ɹ���-1��ȡETF�ļ�ʧ�ܣ�-2��ETF�ļ�ʧ��
	int  	                nDataType;	        //32λ �������� ��enum TDF_MSG_ID
	int			            nDataLen;	        //32λ ���ݳ��ȣ�������TDF_APP_HEAD�ĳ��ȣ�
	void*                   pData;              //����ָ�룬TDF_ETF_LIST_HEAD
};
struct TDF_ETF_LIST_HEAD
{
	int   nFileLength;                         //�ļ�����
	char  szFileName[64];                      //�ļ���(20161227-SZ159949.ETF)
	void* pData;                               //�ļ�����
};
/*******************************************************************  �ص����� *********************************************************************/
//���ݻص�������֪ͨ�û��յ������顢��ʳɽ������ί�У�ί�ж��е�,pMsgHead->pAppHead->ItemCount�ֶο��Ի�֪�õ��˶�������¼��pMsgHead->pAppHead->pDataָ���һ�����ݼ�¼
typedef void (*TDF_DataMsgHandler)  (THANDLE hTdf, TDF_MSG* pMsgHead);    
//ϵͳ��Ϣ�ص�������֪ͨ�û��յ�������Ͽ��¼������ӣ��������������������ȡ�����ȡϵͳ��Ϣʱ��pMsgHead->pAppHeadָ��Ϊ��, pMsgHead->pDataָ����Ӧ�Ľṹ��
typedef void (*TDF_SystemMsgHandler)(THANDLE hTdf, TDF_MSG* pMsgHead);  
//ϵͳ��Ϣ�ص�������֪ͨ�û��յ��˲ο�����
typedef void (*TDF_RefDataMsgHandler)(THANDLE hTdf, TDF_REFDATA_MSG* pMsgHead);  
/******************************************************************* ��Ϣ���Ͷ��� *********************************************************************/
//��Ϣ���Ͷ�Ӧ�Ľṹ�������
enum TDF_MSG_ID
{
    MSG_INVALID = -100,
    //ϵͳ��Ϣ
    MSG_SYS_DISCONNECT_NETWORK,     //����Ͽ��¼�, ��Ӧ�Ľṹ��ΪNULL
    MSG_SYS_CONNECT_RESULT,         //�����������ӵĽ��
    MSG_SYS_LOGIN_RESULT,           //��½Ӧ��
    MSG_SYS_CODETABLE_RESULT,       //����������յ�����Ϣ��ɻ�ȡ��Ӧ�����,����Ϣ��ʾ������Ȩ�г�������ȡ
    MSG_SYS_QUOTATIONDATE_CHANGE,   //�������ڱ��֪ͨ����ȡ����
    MSG_SYS_MARKET_CLOSE,           //���У���ȡ����
    MSG_SYS_HEART_BEAT,             //������������Ϣ, ��Ӧ�Ľṹ��ΪNULL
    //������Ϣ
    MSG_DATA_INDEX,                 //ָ������
    MSG_DATA_MARKET,                //��������
    MSG_DATA_FUTURE,                //�ڻ�����
    MSG_DATA_TRANSACTION,           //��ʳɽ�
    MSG_DATA_ORDERQUEUE,            //ί�ж���
    MSG_DATA_ORDER,                 //���ί��
	MSG_DATA_BBQTRANSACTION,        //BBQ��ȯ�ɽ�����
	MSG_DATA_BBQBID,                //BBQ��ȯ��������
	MSG_DATA_NON_MD,				//��������Ϣ
	MSG_DATA_OTC_OPTION,			//������Ȩ
	MSG_DATA_BROKERQUEUE,           //�����̶���(�۹�)

	MSG_SYS_PACK_OVER   = -10,      //��ǰ������������
	//�ο�������Ϣ
	MSG_REF_ETFL_LIST  = 1,			//�յ��ο�����(��ETF�ļ�)
	//�г��¼���Ϣ
	MSG_SYS_MARKET_EVENT = -9,		//�г��¼�֪ͨ��Ϣ(���г����̣�ת����),�յ����¼������̻��߿��պ�ɻ�ȡ���պ���Ȩ��Ϣ

	//�����г������֪ͨ.����ĳЩ��С�г����ݻص�ʱ��ͬʱ����������г��Ŵ����������������ݽ�MSG_SYS_CODETABLE_RESULT�ȵ���
	//���Ӹ���Ϣ��֪ͨÿһ�г�������������ͻ��ɾݴ�ѡ���ʱ��������
	MSG_SYS_SINGLE_CODETABLE_RESULT,	//�����г�����������յ�����Ϣ��ɻ�ȡ��Ӧ�ĵ����г������

	MSG_SYS_QUOTEUNIT_CHANGE,	//�۲�仯
	MSG_SYS_FAIL_REPLAY,			//�г��ط�ʧ��֪ͨ
};

/******************************************************************* ϵͳ��Ϣ *********************************************************************/
//MSG_SYS_PACK_OVER
struct TDF_PACK_OVER
{
	int nDataNumber;
	int nConID;
};
//��������MSG_SYS_CONNECT_RESULT
struct TDF_CONNECT_RESULT
{
    char szIp[32];
    char szPort[8];
    char szUser[64];
    char szPwd[64];

    unsigned int nConnResult; //Ϊ0���ʾ����ʧ�ܣ���0���ʾ���ӳɹ�
    int nConnectionID;        //����ID
};
//��������MSG_SYS_LOGIN_RESULT
struct TDF_LOGIN_RESULT
{
    unsigned int nLoginResult;//Ϊ0���ʾ��½��֤ʧ�ܣ���0���ʾ��֤�ɹ�

    char szInfo[256];       //��½����ı�
    int nMarkets;           //�г�����
    char szMarket[256][8];  //�г����� SZ, SH, CF, SHF, CZC, DCE
    int nDynDate[256];      //��̬��������
};
//��������MSG_SYS_CODETABLE_RESULT
struct TDF_CODE_RESULT
{
    char szInfo[128];       //��������ı�
    int nMarkets;           //�г�����
    char szMarket[256][8];  //�г�����
    int nCodeCount[256];    //���������
    int nCodeDate[256];     //���������
};
//��������MSG_SYS_SINGLE_CODETABLE_RESULT
struct TDF_SINGLE_CODE_RESULT
{
	char szMarket[8];  //�г�����
	int nCodeCount;    //���������
	int nCodeDate;     //���������
};
//��������MSG_SYS_QUOTATIONDATE_CHANGE
struct TDF_QUOTATIONDATE_CHANGE
{
    char szMarket[8];	    //�г�����
    int nOldDate;	        //ԭ��������
    int nNewDate;	        //����������
};
//��������MSG_SYS_MARKET_CLOSE
struct TDF_MARKET_CLOSE
{
    char    szMarket[8];        //����������
    int		nTime;				//ʱ��(HHMMSSmmm)
    char	chInfo[64];			//������Ϣ
};


enum MARKET_EVENT_NUM
{
	ID_BREAKPOINT_RECV = -1,				//�ϵ㷽ʽ��������
	ID_SNAP_SHOT,							//����
	ID_MARKET_OPEN,							//����
	ID_MARKET_CLEAR,						//����
	ID_MARKET_CLOSE,						//����
	ID_MARKET_TRANS,						//ת��
	ID_MARKET_SAVE,							//����
	ID_CLEAR_CODES,							//���ִ�������  
	ID_SINGLE_CODE_CLEAR,					//������������
	ID_MARKET_HEART_BEAT,					//�г�����(һ����һ��)
	ID_CODE_HEART_BEAT,						//��������(������һ����һ��)
	ID_TRANS_CODES,							//���ִ���ת��
};

struct TDF_CODE;
//�г��¼�
struct TDF_MARKET_EVENT
{
	char    szMarket[12];       //�г�Key
	__int64 market_time;		//�г�ʱ�䣬LTCʱ��(��19000101�𵽵�ǰ���ڵĺ�����)
	__int64 vss_time;			//VSS����ʱ�䣬LTCʱ��(��19000101�𵽵�ǰ���ڵĺ�����)
	int		nEvent;				//�ο�MARKET_EVENT_NUM
	int		nDate;				//�г�����
	int     nCodeSize;			//������(���ִ���͵�����������ʱ��ֵ)
	TDF_CODE* pCode;			//������Ϣ(���ִ���͵�����������ʱ��ֵ)
};

//�۲���
struct TDF_QUOTEUNIT_CHANGE
{
	char szMarket[12];
	char szWindCode[32];
};

//�ط�ʧ��֪ͨ
//��Ϣ����MSG_SYS_FAIL_REPLAY
struct TDF_FAIL_REPLAY
{
	char szMarket[12];	//�г���.SH-2-0��ʽ
	int nDate;			//�ط����ڡ���ʽYYMMDD
	char szInfo[64];	//������Ϣ
};
/******************************************************************* ����� *********************************************************************/
//ʹ��TDF_GetCodeTable��ȡ���������ʹ��TDF_GetOptionCodeInfo��ȡ��Ȩ�������Ȩ��������Ϣ��
struct TDF_CODE
{
    char szWindCode[32];    //Wind Code: AG1302.SHF
    char szMarket[8];       //market code: SHF-1-0
    char szCode[32];        //ԭʼ code:ag1302
    char szENName[32];
    char szCNName[32];      //chinese name: ����1302
    int nType;                            
	char szWindType[8];		//windType : FUR(�ڻ�),����
	int nRecord;			//���ձ��(marketID*64*1024+record)��������Ч
	int nLotSize;			//��������,�۹���Ч
};
struct TDF_OPTION_CODE
{
    TDF_CODE basicCode;
    
    char szContractID[32];// ��Ȩ��Լ����
    char szUnderlyingSecurityID[32];//// ���֤ȯ����
    char chCallOrPut;               // �Ϲ��Ϲ�C1        �Ϲ������ֶ�Ϊ��C������Ϊ�Ϲ������ֶ�Ϊ��P��
    int  nExerciseDate;             // ��Ȩ��Ȩ�գ�YYYYMMDD    
    //�����ֶ�
    char chUnderlyingType;			// ���֤ȯ����C3    0-A�� 1-ETF (EBS �C ETF�� ASH �C A ��)
	char chOptionType;              // ŷʽ��ʽC1        ��Ϊŷʽ��Ȩ�����ֶ�Ϊ��E������Ϊ��ʽ��Ȩ�����ֶ�Ϊ��A��
	
	char chPriceLimitType;          // �ǵ�����������C1 ��N����ʾ���ǵ�����������, ��R����ʾ���ǵ�����������
	int  nContractMultiplierUnit;	// ��Լ��λ,         ������Ȩ��Ϣ������ĺ�Լ��λ, һ��������
	__int64  nExercisePrice;            // ��Ȩ��Ȩ��,       ������Ȩ��Ϣ���������Ȩ��Ȩ�ۣ��Ҷ��룬��ȷ����
	int  nStartDate;                // ��Ȩ�׸�������,YYYYMMDD
	int  nEndDate;                  // ��Ȩ�������/��Ȩ�գ�YYYYMMDD
	int  nExpireDate;               // ��Ȩ�����գ�YYYYMMDD
};
union TD_EXCODE_INFO
{
	struct TD_OptionCodeInfo             //futures options ר�� (nType >= 0x90 && nType <= 0x95),����Ȩ�����ֶ���Ч
	{
		char chContractID[32];           // ��Ȩ��Լ����C19
		char szUnderlyingSecurityID[32]; // ���֤ȯ����
		char chUnderlyingType;			 // ���֤ȯ����C3    0-A�� 1-ETF (EBS �C ETF�� ASH �C A ��)
		char chOptionType;               // ŷʽ��ʽC1        ��Ϊŷʽ��Ȩ�����ֶ�Ϊ��E������Ϊ��ʽ��Ȩ�����ֶ�Ϊ��A��
		char chCallOrPut;                // �Ϲ��Ϲ�C1        �Ϲ������ֶ�Ϊ��C������Ϊ�Ϲ������ֶ�Ϊ��P��
		char chPriceLimitType;           // �ǵ�����������C1 ��N����ʾ���ǵ�����������, ��R����ʾ���ǵ�����������
		int  nContractMultiplierUnit;	 // ��Լ��λ,         ������Ȩ��Ϣ������ĺ�Լ��λ, һ��������
		__int64  nExercisePrice;             // ��Ȩ��Ȩ��,       ������Ȩ��Ϣ���������Ȩ��Ȩ�ۣ��Ҷ��룬��ȷ����
		int  nStartDate;                 // ��Ȩ�׸�������,YYYYMMDD
		int  nEndDate;                   // ��Ȩ�������/��Ȩ�գ�YYYYMMDD
		int  nExerciseDate;              // ��Ȩ��Ȩ�գ�YYYYMMDD
		int  nExpireDate;                // ��Ȩ�����գ�YYYYMMDD
	}Option;
};
struct TDF_CODE_INFO
{
	int  nMarketID;           //������ID,��������
	int  nDataLevel;          //����Level
	int  nFlag;               //������Դ
	int  nType;				  //֤ȯ����
	char chCode[32];          //֤ȯ����(ԭʼ����)
	char chName[64];		  //����֤ȯ����
	TD_EXCODE_INFO exCodeInfo;
	char chWindType[8];		  //windType : FUR(�ڻ�),����
	int nLotSize;			  //��������,�۹���Ч
	char  chMarket[4];		  //�г����(��:SHF)
};

//������ID����///////////////////////////////
#ifndef ID_MARKET_DEFINE
#define ID_MARKET_DEFINE
#define ID_MARKET_SZ	0	//����
#define ID_MARKET_SH	1	//�Ϻ�
#define ID_MARKET_PT	2	//PT
#define ID_MARKET_HZ	3	//���ָ��
#define ID_MARKET_SL	4	//�Զ����г�
#define ID_MARKET_MI	5	//�Զ���ָ��
#define ID_MARKET_HK	6	//�۹�
#define ID_MARKET_QH	7	//�ڻ�
#define ID_MARKET_FE	8	//���
#define ID_MARKET_KF	9	//���(�۹�)
#define ID_MARKET_CF	10	//�й�����ָ��

#define ID_MARKET_SHF	11	//�Ϻ�������Ʒ������
#define ID_MARKET_CZC	12	//֣����Ʒ
#define ID_MARKET_DCE	13	//������Ʒ
/////////////////////////////////////////////
#define ID_MARKET_CME	14  //֥�Ӹ���ҵ������
#define ID_MARKET_CBT	15  //֥�Ӹ��ڻ�������
#define ID_MARKET_CMX	16  //ŦԼ��Ʒ�ڻ�������
#define ID_MARKET_NYM	17  //ŦԼ��Ʒ������
#define ID_MARKET_NYB	18  //ŦԼ�ڻ�������
#define ID_MARKET_IPE	19	//�׶ع���ʯ�ͽ�����
#define ID_MARKET_LME	20	//�׶ؽ���������
#define ID_MARKET_N		21  //ŦԼ������
#define ID_MARKET_O		22  //��˹��˽�����
#define ID_MARKET_HKF	23  //��۽������ڻ�/��Ȩ
/////////////////////////////////////////////
#endif
/******************************************************************* ������Ϣ *********************************************************************/
//��������MSG_DATA_BBQTRANSACTION,     
struct TDF_BBQTRANSACTION_DATA
{
	char        szWindCode[32];         //Wind Code: AG1302.SHF
	int         nActionDay;             //ҵ������(��Ȼ��)
	int			nTime;					//ʱ��(HHMMSSmmm)
	int         nDoneID;                //�ɽ����۱��
	__int64			nPrice;				//�ɽ�������(%)��۸� *10000
	char        chPriceStatus;          //�����ʻ�۸��ʶ	1�������� 	2���۸�
	char        chStatus;               //ɾ��/�ɽ�
	char        chDirection;            //�ɽ�����	1��done	2��gvn		3��tkn		4������
	char        chSource;               //���ۻ���	1������ 2��ƽ����˳ 3���г� 4���������	5������
	char        chSpecialFlag;          //��ʶ�����Ƿ�����Ȩ����	0�ޱ��	1��ע�а�������		2��ע�а�����Ȩ
};
//������MSG_DATA_BBQBID,    
struct TDF_BBQBID_DATA
{
	char        szWindCode[32];         //Wind Code: AG1302.SHF 
	int         nActionDay;             //ҵ������(��Ȼ��)
	int			nTime;					//ʱ��(HHMMSSmmm)
	char        chSource;               //���ۻ���

	__int64     nBidPrice;				//���������ʻ�۸� x10000
	__int64     nBidVolume;				//������ x10000
	char        chBidPriceStatus;		//�����ʻ�۸��ʶ	1�������� 	2���۸�
	char        chIsBid;				//�Ƿ�bid	0������bid����ͨ���ۣ�	1����bid�����򱨼ۣ�
	char        chBidSpecialFlag;		//��ʶ�����Ƿ�����Ȩ����	0�ޱ��	1��ע�а�������		2��ע�а�����Ȩ
	char        chBidStatus;			//�������ű���״̬	0����������	1����������

	__int64     nOfrPrice;				//���������ʻ�۸� x10000
	__int64     nOfrVolume;				//������	�������ļ��ܣ���2000+1000���ͺϲ�Ϊ3000�����ַ����к�- - ����Ϊ0. x10000
	char        chOfrPriceStatus;		//�����ʻ�۸��ʶ	1�������� 	2���۸�
	char        chIsOfr;				//�Ƿ�ofr	0������ofr����ͨ���ۣ�	  1����ofr�����򱨼ۣ�
	char        chOfrSpecialFlag;		//��ʶ�����Ƿ�����Ȩ����	0�ޱ��	1��ע�а�������		2��ע�а�����Ȩ
	char        chOfrStatus;			//�������ű���״̬	0����������	1����������
	int         nDoneID;                //�ɽ����۱��
	char		aucInfo[420];			//������ԭʼ��Ϣ
};
//��㽻������ծȯ�����Ϣ
struct TDF_NoSSE_Bond{
	char    chBidQuoteOrg[32];   //�򱨼۷�
	char    chAskQuoteOrg[32];   //�����۷�
	int     nBidCleanPrice;      //�������뾻��
	int     nAskCleanPrice;      //������������
	int     nBidYield;           //�������뵽��������
	int     nAskYield;           //������������������
	__int64 iBidTradeVolume;     //�������뱨����
	__int64 iAskTradeVolume;     //��������������
	int     nBidTotalPrice;      //��������ȫ��
	int     nAskTotalPrice;      //��������ȫ��
};

//��������MSG_DATA_MARKET
struct TDF_MARKET_DATA
{
    char        szWindCode[32];         //Wind Code: AG1302.SHF
    char        szCode[32];             //ԭʼ Code
    int         nActionDay;             //ҵ������(��Ȼ��)
    int         nTradingDay;            //������
    int			 nTime;					//ʱ��(HHMMSSmmm)
    int			 nStatus;				//״̬
    __int64 nPreClose;					//ǰ���̼�
    __int64 nOpen;						//���̼�
    __int64 nHigh;						//��߼�
    __int64 nLow;						//��ͼ�
    __int64 nMatch;						//���¼�
    __int64 nAskPrice[10];				//������
    __int64 nAskVol[10];				//������
    __int64 nBidPrice[10];				//�����
    __int64 nBidVol[10];				//������
    int nNumTrades;						//�ɽ�����
    __int64		 iVolume;				//�ɽ�����
    __int64		 iTurnover;				//�ɽ��ܽ��
    __int64		 nTotalBidVol;			//ί����������
    __int64		 nTotalAskVol;			//ί����������
    __int64 nWeightedAvgBidPrice;		//��Ȩƽ��ί��۸�
    __int64 nWeightedAvgAskPrice;		//��Ȩƽ��ί���۸�
    int			 nIOPV;					//IOPV��ֵ��ֵ
    int			 nYieldToMaturity;		//����������
    __int64 nHighLimited;				//��ͣ��
    __int64 nLowLimited;				//��ͣ��
    char		 chPrefix[4];			//֤ȯ��Ϣǰ׺
    int			 nSyl1;					//��ӯ��1
    int			 nSyl2;					//��ӯ��2
    int			 nSD2;					//����2���Ա���һ�ʣ�
	TDF_NoSSE_Bond TDF_Bond;			//��㽻������ծȯ�����Ϣ
	int			 nTradeFlag;			//���ױ�־,���庬��ο��ĵ�
	
	__int64 iAfterPrice;    //�̺�۸�(�ƴ�����ʹ�õ�)
	int nAfterVolume;		//�̺���(�ƴ�����ʹ�õ�)
	__int64 iAfterTurnover;	//�̺�ɽ����(�ƴ�����ʹ�õ�)
	int nAfterMatchItems;   //�̺�ɽ�����(�ƴ�����ʹ�õ�)
	
	const TDF_CODE_INFO *  pCodeInfo;   //������Ϣ�� TDF_Close�����������󣬴�ָ����Ч
};
//��������MSG_DATA_INDEX
struct TDF_INDEX_DATA
{
    char        szWindCode[32];         //Wind Code: AG1302.SHF 
    char        szCode[32];             //ԭʼCode
    int         nActionDay;             //ҵ������(��Ȼ��)
    int         nTradingDay;            //������
    int         nTime;			        //ʱ��(HHMMSSmmm)
	int			nStatus;				//״̬��20151223����
    __int64		    nOpenIndex;		    //����ָ��
    __int64 	    nHighIndex;		    //���ָ��
    __int64 	    nLowIndex;		    //���ָ��
    __int64 	    nLastIndex;		    //����ָ��
    __int64	    iTotalVolume;	        //���������Ӧָ���Ľ�������
    __int64	    iTurnover;		        //���������Ӧָ���ĳɽ����
    __int64		    nPreCloseIndex;	    //ǰ��ָ��
	const TDF_CODE_INFO *  pCodeInfo;   //������Ϣ�� TDF_Close�����������󣬴�ָ����Ч
};
//��������MSG_DATA_FUTURE
struct TDF_FUTURE_DATA
{
    char        szWindCode[32];         //Wind Code: AG1302.SHF
    char        szCode[32];             //ԭʼ Code
    int         nActionDay;             //ҵ������(��Ȼ��)
    int         nTradingDay;            //������
    int			 nTime;					//ʱ��(HHMMSSmmm)	
    int			 nStatus;				//״̬
    __int64		 iPreOpenInterest;		//��ֲ�
    __int64 nPreClose;					//�����̼�
    unsigned int nPreSettlePrice;		//�����
    __int64 nOpen;						//���̼�	
    __int64 nHigh;						//��߼�
    __int64 nLow;						//��ͼ�
    __int64 nMatch;						//���¼�
    __int64		 iVolume;				//�ɽ�����
    __int64		 iTurnover;				//�ɽ��ܽ��
    __int64		 iOpenInterest;			//�ֲ�����
    __int64 nClose;						//������
    unsigned int nSettlePrice;			//�����
    __int64 nHighLimited;				//��ͣ��
    __int64 nLowLimited;				//��ͣ��
    int			 nPreDelta;			    //����ʵ��
    int			 nCurrDelta;            //����ʵ��
    __int64 nAskPrice[5];				//������
    __int64 nAskVol[5];					//������
    __int64 nBidPrice[5];				//�����
    __int64 nBidVol[5];					//������
	//Add 20140605
	__int64	nAuctionPrice;		        //�������жϲο���
	int	nAuctionQty;		            //�������жϼ��Ͼ�������ƥ����	
	int nAvgPrice;                      //֣�����ڻ�����
	const TDF_CODE_INFO *  pCodeInfo;   //������Ϣ�� TDF_Close�����������󣬴�ָ����Ч
};
//��������MSG_DATA_TRANSACTION
struct TDF_TRANSACTION
{
    char    szWindCode[32];             //Wind Code: AG1302.SHF
    char    szCode[32];                 //ԭʼCode
    int     nActionDay;                 //��Ȼ��
    int 	nTime;		                //�ɽ�ʱ��(HHMMSSmmm)
    int 	nIndex;		                //�ɽ����
    __int64		nPrice;		            //�ɽ��۸�
    int 	nVolume;	                //�ɽ�����
    __int64		nTurnover;	            //�ɽ����
    int     nBSFlag;                    //��������(��'B', ����'S', ������' ')
    char    chOrderKind;                //�ɽ����
    char    chFunctionCode;             //�ɽ�����
    int	    nAskOrder;	                //������ί�����
    int	    nBidOrder;	                //����ί�����
	const TDF_CODE_INFO *  pCodeInfo;   //������Ϣ�� TDF_Close�����������󣬴�ָ����Ч
};
//��������MSG_DATA_ORDER
struct TDF_ORDER
{
	char    szWindCode[32]; //Wind Code: AG1302.SHF
	char    szCode[32];     //ԭʼCode
	int 	nActionDay;	    //ί������(YYMMDD)
	int 	nTime;			//ί��ʱ��(HHMMSSmmm)
	int 	nOrder;	        //ί�к�
	__int64		nPrice;		//ί�м۸�
	int 	nVolume;		//ί������
	char    chOrderKind;	//ί�����
	char    chFunctionCode;	//ί�д���('B','S','C')
	int     nBroker;		//�����̱���(PT�г���Ч)
	char    chStatus;		//ί��״̬(PT�г���Ч)
	char	chFlag;			//��־(PT�г���Ч)
	const TDF_CODE_INFO *  pCodeInfo;     //������Ϣ�� TDF_Close�����������󣬴�ָ����Ч
};
//��������MSG_DATA_ORDERQUEUE
struct TDF_ORDER_QUEUE
{
	char    szWindCode[32]; //Wind Code: AG1302.SHF 
	char    szCode[32];     //ԭʼ Code
	int     nActionDay;     //��Ȼ��
	int 	nTime;			//ʱ��(HHMMSSmmm)
	int     nSide;			//��������('B':Bid 'A':Ask)
	__int64		nPrice;		//ί�м۸�
	int 	nOrders;		//��������
	int 	nABItems;		//��ϸ����
	int 	nABVolume[200];	//������ϸ
	const TDF_CODE_INFO *  pCodeInfo;     //������Ϣ�� TDF_Close�����������󣬴�ָ����Ч
};

//������Ȩ����������MSG_DATA_OTC_OPTION
struct TDF_OTC_Option{

	char  szWindCode[32];		//Wind Code: AG1302.SHF
	char  szCode[32];			//ԭʼ Code
	char  szWindType[8];		//windType : FUR.SHF(�������ڻ�)

	char chOptionType;			// ŷʽ��ʽC1        ��Ϊŷʽ��Ȩ�����ֶ�Ϊ��E������Ϊ��ʽ��Ȩ�����ֶ�Ϊ��A��
	char chCallOrPut;			// �Ϲ��Ϲ�C1        �Ϲ������ֶ�Ϊ��C������Ϊ�Ϲ������ֶ�Ϊ��P��
	__int64  iExercisePrice;	// ��Ȩ��Ȩ��,  

	int  nTime;					// �ɽ�ʱ��
	int  nVolume;				// �ɽ���
	__int64 iMatch;				// �ɽ���
	__int64 iPreSettlement;		// ǰ�ս����
	__int64 iSettlement;		// �����
	int nPrePosition;			// ǰ�ճֲ���
	int nPosition;				// �ֲ���
	__int64 iAskPrice;			// ί���۸�
	__int64 iBidPrice;			// ί��۸�
	int nAskVol;				// ί������
	int nBidVol;				// ί������
	const TDF_CODE_INFO *  pCodeInfo;     //������Ϣ�� TDF_Close�����������󣬴�ָ����Ч
};

/******************************************************************* ������� *********************************************************************/
enum DATA_TYPE_FLAG
{
	DATA_TYPE_TRANSACTION = 0x2,           //��ʳɽ�
	DATA_TYPE_ORDER       = 0x4,           //���ί��
	DATA_TYPE_ORDERQUEUE  = 0x8,           //ί�ж��� 
	DATA_TYPE_NONE = 0,                    //ֻҪ���飬�����������ģ����Ķ������DATA_TYPE_TRANSACTION | DATA_TYPE_ORDER | DATA_TYPE_ORDERQUEUE
};
enum SUBSCRIPTION_STYLE
{
	SUBSCRIPTION_FULL = 3,                //ȫ�г�����
	SUBSCRIPTION_SET=0,                   //����Ϊ�����б��й�Ʊ��
	SUBSCRIPTION_ADD=1,                   //���Ӷ����б��й�Ʊ
	SUBSCRIPTION_DEL=2,                   //ɾ���б��еĶ���
};

/******************************************************************* ���� *********************************************************************/
//�������ã��ڵ���TDF_Open֮ǰ����k
enum TDF_ENVIRON_SETTING
{
	TDF_ENVIRON_HEART_BEAT_INTERVAL,		//Heart Beat�����������, ��ֵΪ0���ʾĬ��ֵ10����
	TDF_ENVIRON_MISSED_BEART_COUNT,			//���û���յ����������������ֵ����û�յ������κ����ݣ����ж�Ϊ���ߣ���ֵ0ΪĬ�ϴ���2��
	TDF_ENVIRON_OPEN_TIME_OUT,				//�ڵ�TDF_Open�ڼ䣬����ÿһ�����ݰ��ĳ�ʱʱ�䣨����������TDF_Open�����ܵ����ȴ�ʱ�䣩����ֵΪ0��Ĭ��30��
	TDF_ENVIRON_USE_PACK_OVER,				//�����������Ϣ
	TDF_ENVIRON_HEART_BEAT_FLAG,			//�������ݷ��ͷ�ʽ(0: ȡ������������ 1��û�����ݷ��͵�ʱ�����������ݣ�2���й��ɷ�����������s)
	TDF_ENVIRON_SOURCE_MODE,				//˫������Դģʽ,ֵ�ο�TDF_SOURCE_SETTINGö����
	TDF_ENVIRON_SOURCE_MODE_VALUE,			//˫������Դģʽ�²�����ֵ
	TDF_ENVIRON_OUT_LOG,					//1����ǰĿ¼�´���log�������ڵ�ǰ·����log�ļ���ʱ������log�ļ�����
	TDF_ENVIRON_SNAPSHOT_ENENT,				//1�����Ϳ����¼�������֪ͨ�����ղ����͵��ص�����������ֱ�ӷ��Ϳ��յ��ص��������޿���֪ͨ, ������
	TDF_ENVIRON_ORIGINAL_VOL,				//ԭʼ�ṹ��Ч��1��ָ���ɽ����ͳɽ���ĵ�λΪ�ɺ�Ԫ��Ĭ��Ϊ100�ɺ�100Ԫ
	TDF_ENVIRON_BREAKPOINT,					//1.����Ͽ���ϵ�������else.�������¿���
	TDF_ENVIRON_SWITCH_SAME,				//switch˫��ģʽ����С����֤һ�������յ�ǰ�պͿ���ֻ��һ����Чֵ
};
//˫��ģʽ����
enum TDF_SOURCE_SETTING
{
	TDF_SOURCE_MERGE,						//�ϲ�ģʽ��ѡ�����ݸ����һ·����Դ����
	TDF_SOURCE_SWITCH,						//�ӳ��л�ģʽ���յ����ݼ�������趨��ֵ�����л���Ĭ��ֵΪ5000���룬����ͨ��TDF_ENVIRON_SOURCE_MODE_VALUE��������
};
//�����������ã����ö������ʵ�֣������������������Լ��ݾ�APIʹ�÷�����
//��Ⱦ�API�����ˣ�nReconnectCount��nReconnectGap��nProtocol��nDate
struct TDF_OPEN_SETTING
{
    char szIp[32];
    char szPort[8];
    char szUser[64];
    char szPwd[64];
	//�ص���������
	TDF_DataMsgHandler pfnMsgHandler;			//������Ϣ����ص�
	TDF_SystemMsgHandler pfnSysMsgNotify;		//ϵͳ��Ϣ֪ͨ�ص�

	//��������									--ע�⣺�������г������ģ�����ÿ���г����붩���г�����룬ֻ�����г������ʹ��г�ȫ�����롣
	const char* szMarkets;						// �г�����(��)������"SZ;SH;CF;SHF;DCE;SHF"����Ҫ���ĵ��г��б��ԡ�;���ָ�
	const char* szResvMarkets;					// �г�����(ȱ)������"SZ;SH;CF;SHF;DCE;SHF"����Ҫ���ĵ��г��б��ԡ�;���ָ����Ϊ""	
	const char* szSubScriptions;				// ���붩��,����"600000.sh;ag.shf;000001.sz"����Ҫ���ĵĹ�Ʊ(������Ʊ��ʽΪԭʼCode+.+�г�����999999.SH)���ԡ�;���ָΪ������ȫ�г�
	unsigned int nTypeFlags;					// �������Ͷ��ģ�֧�ֶ���3������TRANSACTION;ORDER;ORDERQUEUE�� ��ע�⣺���������κ�ʱ�򶼷��ͣ�����Ҫ����! �μ�enum DATA_TYPE_FLAG

	unsigned int nTime;							//Ϊ0������ʵʱ���飬(0xffffffff��ͷ����,������)
	unsigned int nConnectionID;					//����ID�����ӻص���Ϣ�ĸ��ӽṹ TDF_CONNECT_RESULT�� ��������ID
};

//�����������
struct TDF_SERVER_INFO
{
	char szIp[32];		//IP
	char szPort[8];		//�˿�
	char szUser[64];	//�û���
	char szPwd[64];		//����
};
struct TDF_OPEN_SETTING_EXT
{
	//����������
	TDF_SERVER_INFO	siServer[MAXSERVER];	//��������Ϣ
	unsigned int nServerNum;				//����������
	//�ص���������
	TDF_DataMsgHandler pfnMsgHandler;       //������Ϣ����ص�
	TDF_SystemMsgHandler pfnSysMsgNotify;   //ϵͳ��Ϣ֪ͨ�ص�
	
	//unsigned int nProtocol;               //Э��ţ�Ϊ0��ΪĬ�ϣ�����0x6001
	//��������                              --ע�⣺�������г������ģ�����ÿ���г����붩���г�����룬ֻ�����г������ʹ��г�ȫ�����롣
	const char* szMarkets;					// �г�����(��)������"SZ-2-0;SH-1-1;"����Ҫ���ĵ��г��б��ԡ�;���ָ�
	const char* szResvMarkets;              // �г�����(ȱ)������"SZ-2-0;SH-1-1;"����Ҫ���ĵ��г��б��ԡ�;���ָ�,����Ϊ""
	const char* szSubScriptions;            // ���붩��,����"600000.sh;ag.shf;000001.sz"����Ҫ���ĵĹ�Ʊ(������Ʊ��ʽΪԭʼCode+.+�г�����999999.SH)���ԡ�;���ָΪ������ȫ�г�
	unsigned int nTypeFlags;                // �������Ͷ��ģ�֧�ֶ���3������TRANSACTION;ORDER;ORDERQUEUE�� ��ע�⣺���������κ�ʱ�򶼷��ͣ�����Ҫ����! �μ�enum DATA_TYPE_FLAG

	unsigned int nTime;                     //Ϊ0������ʵʱ���飬Ϊ0xffffffff��ͷ����
	unsigned int nConnectionID;             //����ID�����ӻص���Ϣ�ĸ��ӽṹ TDF_CONNECT_RESULT�� ��������ID����ϢͷҲ�������ID
};
//�������������
enum TDF_PROXY_TYPE
{
	TDF_PROXY_SOCK4,
	TDF_PROXY_SOCK4A,
	TDF_PROXY_SOCK5,
	TDF_PROXY_HTTP11,
};
struct TDF_PROXY_SETTING
{
	TDF_PROXY_TYPE nProxyType;	//��������
	char szProxyHostIp[32];		//����IP
	char szProxyPort[8];		//����˿�
	char szProxyUser[32];		//�����û���
	char szProxyPwd[32];		//��������
};
//�ܰ������������Ӵ��������һ������ʱ��+����
struct ConStatInfo
{
	__int64  iTotalPacks;		//�ܰ�
	__int64  iTotalTraffic;		//������
	int      nToalConTimes;		//���Ӵ���
	__int64  ilastConTime;		//���һ������ʱ��+����
};

//�򿪲ο����ݷ���
struct TDF_OPEN_REFDATA_SETTING
{
	char szIp[32];
	char szPort[8];
	char szUser[64];
	char szPwd[64];
	//�ص���������
	TDF_RefDataMsgHandler pfnRefDataHandler;       //�ο�������Ϣ����ص�
};

//����ͨ�����ͨ��
struct TDF_NON_MD_DATA
{
	int nItemSize;					//��������Ϣ��С
	int nMsgType;					//��������Ϣ����
	char pData[1024*1024 - 8];		//��������Ϣ����
};

//�����̶���(HK)/////////////////////////////////////
struct TD_BrokerQueue
{
	char  szWindCode[32];				//Wind Code: AG1302.SHF
	char  szCode[32];					//ԭʼ Code
	int   nActionDay;					//��Ȼ��
	int   nAskTime;						// ����ʱ�䣨HHMMSSmmm��
	int   nBidTime;						// ����ʱ�䣨HHMMSSmmm��
	int   nAskBrokers;					// �������͸���
	int   nBidBrokers;					// ���򾭼͸���
	short sAskBroker[40];				// ����ǰ40����
	short sBidBroker[40];				// ����ǰ40����
	const TDF_CODE_INFO *  pCodeInfo;    //������Ϣ�� TDF_Close�����������󣬴�ָ����Ч
};

#pragma pack(pop)
#endif
