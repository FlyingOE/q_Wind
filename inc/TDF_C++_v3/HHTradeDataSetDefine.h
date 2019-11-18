#ifndef _HHTRADEDATASET_DEFINE_H_
#define _HHTRADEDATASET_DEFINE_H_

//������ID����///////////////////////////////
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

#define ID_MARKET_SHF	11	//�Ϻ�������Ʒ��������2012.8.12��һ��������Ʒ��������Ϊ3����������
#define ID_MARKET_CZC	12	//֣����Ʒ��2012.8.12��һ��������Ʒ��������Ϊ3����������
#define ID_MARKET_DCE	13	//������Ʒ��2012.8.12��һ��������Ʒ��������Ϊ3����������

#define ID_MARKET_CME	14  //֥�Ӹ���ҵ������(2013.11.20����۹ɼ���������)
#define ID_MARKET_CBT	15  //֥�Ӹ��ڻ�������(2013.11.20����۹ɼ���������)
#define ID_MARKET_CMX	16  //ŦԼ��Ʒ�ڻ�������(2013.11.20����۹ɼ���������)
#define ID_MARKET_NYM	17  //ŦԼ��Ʒ������(2013.11.20����۹ɼ���������)
#define ID_MARKET_NYB	18  //ŦԼ�ڻ�������(2013.11.20����۹ɼ���������)
#define ID_MARKET_IPE	19	//�׶ع���ʯ�ͽ�����(2013.11.20����۹ɼ���������)
#define ID_MARKET_LME	20	//�׶ؽ���������(2013.11.20����۹ɼ���������)
#define ID_MARKET_N		21  //ŦԼ������(2013.11.20����۹ɼ���������)
#define ID_MARKET_O		22  //��˹��˽�����(2013.11.20����۹ɼ���������)
#define ID_MARKET_HKF	23  //��۽������ڻ�/��Ȩ(2013.11.20����۹ɼ���������)
#define ID_MARKET_BBQ	30  //��۽������ڻ�/��Ȩ(2013.11.20����۹ɼ���������)

//�г�������Ϣ��2013.1.23��
#define MARKETSETFLAGS_ADD(MARKETSET, MARKETID)	 (((char*)MARKETSET)[MARKETID/8] |= 1<<(MARKETID%8))	///<����һ���г�
#define MARKETSETFLAGS_DEL(MARKETSET, MARKETID)	 (((char*)MARKETSET)[MARKETID/8] &= ~(1<<(MARKETID%8)))	///<ɾ��һ���г�
#define MARKETSETFLAGS_OK(MARKETSET, MARKETID)	 (((char*)MARKETSET)[MARKETID/8] & (1<<(MARKETID%8)))	///<�ж��Ƿ��д��г�

/////////////////////////////////////////////
//֤ȯ�������ඨ��(ϸ��)
#define ID_BT_INDEX				0x01	//������ָ��
#define ID_BT_INDEX_ASIA		0x03	//����ָ��
#define ID_BT_INDEX_FOREIGN		0x04	//����ָ��
#define ID_BT_INDEX_HH			0x05	//ϵͳ����ָ��
#define ID_BT_INDEX_USER		0x06	//�û�����ָ��
#define ID_BT_INDEX_FUTURES     0x07    //�ڻ�ָ��
#define ID_BT_INDEX_MD			0x08	//ָ���ֻ�
#define ID_BT_SHARES_A			0x10	//A��
#define ID_BT_SHARES_S			0x11	//��С���
#define ID_BT_SHARES_G			0x12	//��ҵ���
#define ID_BT_SHARES_B			0x16	//B��
#define ID_BT_SHARES_H			0x17	//H��(��������е�A�ɣ�2008.12.4Ϊ�Ϻ�Level2����H������)
#define ID_BT_SHARES_CHADR      0x18   //����CDR
#define ID_BT_SHARES_KCB		0x19   //�ƴ���
#define ID_BT_SHARES_US			0x1a	//US
#define ID_BT_SHARES_USADR		0x1b	//US ADR
#define ID_BT_SHARES_E			0x1e	//��չ����Ʊ(��)
#define ID_BT_FUND				0x20	//����
#define ID_BT_FUND_OPEN			0x21	//δ���п��Ż���
#define ID_BT_FUND_LOF			0x22	//���п��Ż���
#define ID_BT_FUND_ETF			0x23	//�����Ϳ���ʽָ������
#define ID_BT_FUND_ETS			0x25	//��չ������(��)
#define ID_BT_FUND_GRD          0x26    //�ּ��ӻ��� //add 20160525 ����V5
#define ID_BT_FUND_OAR          0x27    //��������� //add 20160525 ����V5
#define ID_BT_BOND_NA			0x30	//����ծȯ
#define ID_BT_BOND_CORP			0x31	//��ҵծȯ
#define ID_BT_BOND_FIN			0x32	//����ծȯ
#define ID_BT_BOND_CON			0x33	//��תծȯ
#define ID_BT_BOND_WI           0x34    //ծȯԤ����(add by cli)
#define ID_BT_BOND_EXCH         0x35    //�ɽ���ծȯ��Exchangeable Bond�����EB��
#define ID_BT_BOND_USA          0x38    //��ծ��1������Чֵ��
#define ID_BT_REPO_NA			0x40	//��ծ�ع�
#define ID_BT_REPO_CORP			0x41	//��ծ�ع�
#define ID_BT_QZ				0x60	//Ȩ֤
#define ID_BT_QZ_C				0x61	//�Ϲ�Ȩ֤
#define ID_BT_QZ_P				0x62	//�Ϲ�Ȩ֤
#define ID_BT_QZ_C_B			0x64	//�Ϲ�Ȩ֤(B��)
#define ID_BT_QZ_P_B			0x65	//�Ϲ�Ȩ֤(B��)
#define ID_BT_QZ_M				0x66	//ţ֤��moo-cow��
#define ID_BT_QZ_B				0x67	//��֤��bear��

#define ID_BT_FUTURES_IDX		0x70	//ָ���ڻ�
#define ID_BT_FUTURES			0x71	//��Ʒ�ڻ�
#define ID_BT_FUTURES_SHA		0x72	//��Ʊ�ڻ�
#define ID_BT_FUTURES_IBOR		0x73	//ͬҵ��������ڻ�
#define ID_BT_FUTURES_EFN		0x74	//������ծȯ(Exchange Fund Note Futures)
#define ID_BT_FUTURES_EFP       0x75    //�ڻ�ת�ֻ�����(Exchange For Physicals)
#define ID_BT_FUTURES_EFS       0x76    //�ڻ�ת���ڽ���(Exchange of Futures For Swaps)
#define ID_BT_FUTURES_CX_IDX	0x78	//ָ���ڻ�����CX
#define ID_BT_FUTURES_CC_IDX	0x79	//ָ���ڻ�����CC
#define ID_BT_FUTURES_CX		0x7a	//��Ʒ�ڻ�����CX
#define ID_BT_FUTURES_CC		0x7b	//��Ʒ�ڻ�����CC
#define ID_BT_FUTURES_CX_SHA	0x7c	//��Ʊ�ڻ�����CX
#define ID_BT_FUTURES_CC_SHA	0x7d	//��Ʊ�ڻ�����CC
#define ID_BT_FUTURES_QX		0x7e	//���ֲ����
#define ID_BT_FUTURES_KQ		0x7f	//���ڲ����

#define ID_BT_FOREX_BASE		0x80	//��������
#define ID_BT_FOREX_CROSE		0x81	//�������
#define ID_BT_FOREX_REVERSE		0x82	//�������
#define ID_BT_FOREX_FORWARD		0x83	//Զ�����
#define ID_BT_FOREX_SWAP		0x84	//�������

#define ID_BT_OPTION_C			0x90	//�Ϲ���Ȩ
#define ID_BT_OPTION_P			0x91	//�Ϲ���Ȩ
#define ID_BT_OPTION_C_IDX      0x92    //ָ���Ϲ���Ȩ
#define ID_BT_OPTION_P_IDX      0x93    //ָ���Ϲ���Ȩ
#define ID_BT_OPTION_C_CMD      0x94	//��Ʒ�Ϲ���Ȩ
#define ID_BT_OPTION_P_CMD      0x95    //��Ʒ�Ϲ���Ȩ
#define ID_BT_OPTION_OTC        0x96    //������Ȩ
#define ID_BT_OPTION_UDO        0x97    //�û��Զ�����Ȩ

#define ID_BT_BANKRATE			0xd0	//��������
#define ID_BT_BANKRATE_HK		0xd1	//��������(HK)
#define ID_BT_BANKRATE_WORLD	0xd2	//��������(Interal)
#define ID_BT_BANKRATE_BOND 	0xd3	//ծȯ����
#define ID_BT_NMETAL			0xe0	//�����(noble metal)
#define ID_BT_OTHERS			0xf0	//����
#define ID_BT_OTHER_SHARES_SG_A	0xf1	//A���¹��깺
#define ID_BT_OTHER_SHARES_ZF_A	0xf2	//A������

#define ID_BT_DIGITALCCY        0xa0	//���ֻ���DIGITALCCY

//֤ȯ�������ඨ��
#define ID_BASECLASS_INDEX		0x00	//ָ��
#define ID_BASECLASS_SHARES		0x10	//��Ʊ
#define ID_BASECLASS_FUND		0x20	//����
#define ID_BASECLASS_BOND		0x30	//ծȯ & ��תծ
#define ID_BASECLASS_REPO		0x40	//�ع�
#define ID_BASECLASS_QZ			0x60	//Ȩ֤
#define ID_BASECLASS_FUTURES	0x70	//�ڻ�
#define ID_BASECLASS_FOREX		0x80	//���
#define ID_BASECLASS_OPTIONS	0x90	//��Ȩ(2012.5.22)
#define ID_BASECLASS_DIGITALCCY 0xa0	//���ֻ���DIGITALCCY
#define ID_BASECLASS_BANKRATE	0xd0	//��������
#define ID_BASECLASS_NMETAL		0xe0	//�����(noble metal)
#define ID_BASECLASS_OTHER		0xf0	//����

//֤ȯ��չ���ඨ��
#define ID_BTEX_HASWARRANT		0x80000000	//����(��Ȩ֤�Ĺ�Ʊ)
#define ID_BTEX_DELETE			0x40000000	//�Ѿ�ɾ��
#define ID_BTEX_DELISTING		0x08000000	//���������飨2012.8.15��
#define ID_BTEX_RISKWARNING		0x04000000	//���վ�ʾ��飨2012.11.30, ԭST��飩
#define ID_BTEX_NOSSEBOUND		0x02000000  //
#define ID_BTEX_BBQBOUND		0x10000000  //BBQ BOUND
#define ID_BTEX_ETFIOPV			0x20000000  //ETF IOPV




//������Ŀ����
#define ID_PRICETYPE_SHARES		0x00	//��Ʊ
#define ID_PRICETYPE_FUND		0x01	//����
#define ID_PRICETYPE_BOND		0x02	//ծȯ
#define ID_PRICETYPE_CBOND		0x03	//תծ
#define ID_PRICETYPE_REPO		0x04	//�ع�
#define ID_PRICETYPE_QZ			0x05	//Ȩ֤
#define ID_PRICETYPE_INDEX		0x06	//ָ��
#define ID_PRICETYPE_FUTURES	0x07	//��Ʒ�ڻ�
#define ID_PRICETYPE_FOREX		0x08	//���
#define ID_PRICETYPE_MB			0x09	//ţ��֤
#define ID_PRICETYPE_NMATEL		0x0a	//�����
#define ID_PRICETYPE_IDXFUTURES 0x0b	//ָ���ڻ�
#define ID_PRICETYPE_DEF1		0x10	//�û�����1
#define ID_PRICETYPE_DEF2		0x11	//�û�����2
#define ID_PRICETYPE_DEF3		0x12	//�û�����3
#define ID_PRICETYPE_DEF4		0x13	//�û�����4

//�����ඨ��
#define ID_ROOTCLASS_SYSTEM			0x00000000		//ϵͳ����
#define ID_ROOTCLASS_INDUSTRY		0x00000001		//��ҵ����
#define ID_ROOTCLASS_AREA			0x00000002		//��������
#define ID_ROOTCLASS_BLOCK			0x00000003		//���
#define ID_ROOTCLASS_INVEST			0x00010004		//Ͷ��Ʒ��
#define ID_ROOTCLASS_SELFSEL		0x00020005		//��ѡƷ��
#define ID_ROOTCLASS_COMBO			0x00030006		//���Ʒ��
#define ID_ROOTCLASS_INDEXBLOCK		0x00030007		//����ָ��
#define ID_ROOTCLASS_INDEX			0x00040007		//������ָ��
#define ID_ROOTCLASS_INDEXCOMBO		0x00050008		//Ͷ�����
#define ID_ROOTCLASS_HZ				0x00080009		//������"


//ϵͳ�����
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
	int    nType;			//����
	float  fSXFee;			//��������
	float  fYHFee;			//ӡ��˰��
	float  fGHFee;	 		//������
	float  fQTFee;	 		//��������
	float  fMinFee;	 		//��С����
	float  fExchRate; 		//����exchange rate
	float  fPriceRate;		//����ϵ��
};


struct HHSystemSet_IndexDefine
{
	int    nRecord;		//ָ����¼��
	char   sCode[16];	//����
	char   sName[16];	//����
	char   sCodeOrg[8];	//����������
	char   cTjFlag;		//ͳ�Ʊ�־('K'����)
	char   sTjType[16];	//ͳ�Ʒ�Χ
	char   sTjCode[11];	//�ɽ���ͳ�ƴ���
};

struct HHNsdSystemSet_BaseTypeSet
{
	char   sTypeName[12];	//�����������
	int    nType;			//����
	int    nVol;			//�ɽ�����λ
	int    nMinVol;			//�ɽ�����λ
	int    nDec;			//����С����λ��
	char   sTypeDef[95];	//������ඨ��
	float  fSXFee;			//��������
	float  fYHFee;			//ӡ��˰��
	float  fGHFee;	 		//������
	float  fQTFee;	 		//��������
	float  fMinFee;	 		//��С����
	float  fExchRate; 		//����exchange rate
	float  fPriceRate;		//����ϵ��
};

struct exchange_time		//����ʱ��(�˽ṹ�����޸ģ���Ϊ�����ݱ��桢��������ʹ��)
{	
	short times;			//����ʱ�����
	short timedif;			//ʱ����г��Ľ���ʱ�����õ�ʱ������Ͷ�����֮��ʱ���ķ�����������������ʱ��Ϊ-5����������������ʱ�������������Ľ�����timediff�-720��((-5+1-8)*60=-720)�� 
	short opentime[4];		//����ʱ��
	short closetime[4];		//����ʱ��
};
struct exchange_time_n		//����ʱ��(�˽ṹ�����޸ģ���Ϊ�����ݱ��桢��������ʹ��)
{	
	short times;			//����ʱ�����
	short timedif;			//ʱ��
	short opentime[8];		//����ʱ��
	short closetime[8];		//����ʱ��
	char  chExchTimeFlags[8];
};
struct HHNsdSystemSet_DBF
{
	char   ExchFileFlag[4];
	//��Ϣ�ⶨ��	
	char   zq_field_code[12];	//����
	char   zq_field_name[12];	//����
	char   zq_field_csclass[12];//��ҵ���
	char   zq_field_cfclass[12];//�ɷֱ�־
	char   zq_field_fxgb[12];	//���йɱ�
	char   zq_field_ltgb[12];	//��ͨ�ɱ�
	char   zq_field_mgsy[12];	//ÿ������
	char   zq_field_highLim[12];//������ͣ��
	char   zq_field_lowLim[12];	//���յ�ͣ��
	//����ⶨ��
	long   hq_first;			//��ʼ��¼��
	char   hq_field_code[12];	//����	
	char   hq_field_name[12];	//����
	char   hq_field_close[12];	//ǰ����
	char   hq_field_open[12];	//����
	char   hq_field_high[12];	//���
	char   hq_field_low[12];	//���
	char   hq_field_new[12];	//����
	char   hq_field_volume[12];	//�ɽ���
	char   hq_field_money[12];	//�ɽ����
	char   hq_field_syl[12];	//��ӯ��
	char   hq_field_bs[12];		//�ɽ�����
	char   hq_field_syl2[12];	//��ӯ��2
	char   hq_field_hold[12];	//��Լ�ֲ���

	//�����̿�
	char   hq_field_sale[10][12];	//������
	char   hq_field_buy[10][12];	//�����
	char   hq_field_salevol[10][12];//������
	char   hq_field_buyvol[10][12];	//������

	//ָ���ⶨ��
	char   idx_field_code[12];	//����
	char   idx_field_close[12];	//ǰ����
	char   idx_field_open[12];	//����
	char   idx_field_high[12];	//���
	char   idx_field_low[12];	//���
	char   idx_field_new[12];	//����
	char   idx_field_volume[12];//�ɽ���
	char   idx_field_money[12];	//�ɽ����
};

struct HHNsdSystemSet_SSE
{ 
	BOOL   bEnable;  
	BOOL   bInterior;
	char   ExchangeName[20];    				//������ȫ��
	char   ExchangeNameCJ[4];  					//���������1
	char   ExchangeNameCJ2[9];  				//���������2
	char   ExchFileFlag[4];     				//ϵͳ�ļ���־ SZ:���� SH:�Ϻ�
	char   bLocalBackup;						//���ر���
	char   mmpitems;							//�����̸���
	char   mmpflags;							//�����̱�־

	long   nBaseTypes;							//��������
	HHNsdSystemSet_BaseTypeDef BaseType[32];	//Ʒ�ַ���
	
	exchange_time ExchTime;						//����ʱ��

	char   Backup_1Minute[9][64];				//1�������ݱ���Ŀ¼
	char   Backup_Tick[9][64];					//Tick���ݱ���Ŀ¼

	char   BaseDataTxtCopy[12];					//�ⲿ�ı����Ͽ�����־
	char   BaseDataTxtName[10][20]; 			//�ⲿ�ı���������
	char   BaseDataTxtPath[10][60]; 			//�ⲿ�ı�����·��

	char   bFJIndexFlag;						//����ָ����־
	char   strFJIndexFile[71];  				//����ָ��������

	char   ZQ_File[2][64];						//��Ϣ��
	char   HQ_File[2][64];						//�����
	char   IDX_File[2][64];						//ָ����

	BOOL   bCheckHQMode;						//���ñ�������ⷽʽ 0, 1, 2
	int    nCheckTime;							//����仯���ʱ��(��) BOOL   checkvolume; //�ɽ����ݴ��־
	BOOL	checkvolume;						//�ɽ����ݴ��־

	char   bCheckFirstRecord[2];				//���������һ����¼�仯

	char   sRecv[62];							//���� 

	int    nIndexDefines;
	int	   nIndexDefinesPointer;				//HHSystemSet_IndexDefine *pIndexDefine;

	int    nBaseTypeSets;
	int	   nBaseTypeSetsPointer;				//HHNsdSystemSet_BaseTypeSet *pBaseTypeSet;

	int	   nExcludeCodePointer;					//char   *pExcludeCode;
	int	   nDBFSetPointer;						//HHNsdSystemSet_DBF	*pDBFSet;				//���ݿⶨ��

	//Ϊ����32λ��64λ, ��ָ�붨�嵽���
	HHSystemSet_IndexDefine *pIndexDefine;
	HHNsdSystemSet_BaseTypeSet *pBaseTypeSet;
	char   *pExcludeCode;
	HHNsdSystemSet_DBF	*pDBFSet;				//���ݿⶨ��
};


#endif