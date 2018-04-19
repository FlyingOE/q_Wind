/* Copyright 2013. Wind Info�Ϻ������Ϣ�����ɷ����޹�˾
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:  The above
 * copyright notice and this permission notice shall be included in all copies
 * or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef _WINDQUANTAPI_H
#define _WINDQUANTAPI_H

#include <windows.h>
#include <tchar.h>
//#include <ATLComTime.h>

#define SMALLSTRING_SIZE 64
#define DATA_DIMENSION_SIZE 64

// ����������Ͷ���
typedef int WQErr;

// ����ID�����Ͷ���
typedef __int64 WQID;

// ������ľ��嶨�壬����ϸ��˵�������Ե���WErr������ȡ

// �����ɹ�
#define WQERR_OK                      0              

#define WQERR_BASE                    -40520000

// һ���Դ���
#define WQERR_GENERAL_CLASS           WQERR_BASE

// δ֪���� 
#define WQERR_UNKNOWN                 WQERR_GENERAL_CLASS - 1          

// �ڲ�����
#define WQERR_INTERNAL_ERROR          WQERR_GENERAL_CLASS - 2          

// ����ϵͳԭ��
#define WQERR_SYSTEM_REASON           WQERR_GENERAL_CLASS - 3           

// ��½ʧ��
#define WQERR_LOGON_FAILED            WQERR_GENERAL_CLASS - 4           

// �޵�½Ȩ��
#define WQERR_LOGON_NOAUTH            WQERR_GENERAL_CLASS - 5           

// �û�ȡ��
#define WQERR_USER_CANCEL             WQERR_GENERAL_CLASS - 6           

// û�п�������
#define WQERR_NO_DATA_AVAILABLE       WQERR_GENERAL_CLASS - 7           

// ����ʱ
#define WQERR_TIMEOUT                 WQERR_GENERAL_CLASS - 8           

// Wbox����
#define WQERR_LOST_WBOX               WQERR_GENERAL_CLASS - 9           

// δ�ҵ��������
#define WQERR_ITEM_NOT_FOUND          WQERR_GENERAL_CLASS - 10          

// δ�ҵ���ط���
#define WQERR_SERVICE_NOT_FOUND       WQERR_GENERAL_CLASS - 11          

// δ�ҵ����ID
#define WQERR_ID_NOT_FOUND            WQERR_GENERAL_CLASS - 12 

// ���ڱ���ʹ�������˺ŵ�¼���������Ʒ�����޷�ʹ��ָ���˺ŵ�¼
#define WQERR_LOGON_CONFLICT          WQERR_GENERAL_CLASS - 13

// δ��¼ʹ��WIM���ߣ����޷���¼
#define WQERR_LOGON_NO_WIM            WQERR_GENERAL_CLASS - 14

// ������¼ʧ�ܴ�������
#define WQERR_TOO_MANY_LOGON_FAILURE  WQERR_GENERAL_CLASS - 15

// ���˰�汾ʧЧ
#define WQERR_PER_VERSION_FAILURE     WQERR_GENERAL_CLASS - 16



// �������ݴ�ȡ����
#define WQERR_IOERROR_CLASS           WQERR_BASE - 1000
// IO��������
#define WQERR_IO_ERROR                WQERR_IOERROR_CLASS - 1

// ��̨������������
#define WQERR_SERVICE_NOT_AVAL        WQERR_IOERROR_CLASS - 2

// ��������ʧ��
#define WQERR_CONNECT_FAILED          WQERR_IOERROR_CLASS - 3

// ������ʧ��
#define WQERR_SEND_FAILED             WQERR_IOERROR_CLASS - 4

// ���ݽ���ʧ��
#define WQERR_RECEIVE_FAILED          WQERR_IOERROR_CLASS - 5

// �������
#define WQERR_NETWORK_ERROR           WQERR_IOERROR_CLASS - 6

// �������ܾ�����
#define WQERR_SERVER_REFUSED          WQERR_IOERROR_CLASS - 7

// �����Ӧ��
#define WQERR_SVR_BAD_RESPONSE        WQERR_IOERROR_CLASS - 8

// ���ݽ���ʧ��
#define WQERR_DECODE_FAILED           WQERR_IOERROR_CLASS - 9

// ���糬ʱ
#define WQERR_INTERNET_TIMEOUT        WQERR_IOERROR_CLASS - 10

// Ƶ������
#define WQERR_ACCESS_FREQUENTLY       WQERR_IOERROR_CLASS - 11

//�������ڲ�����
#define WQERR_SERVER_INTERNAL_ERROR	  WQERR_IOERROR_CLASS - 12


// �����������
#define WQERR_INVALID_CLASS           WQERR_BASE - 2000
// �޺Ϸ��Ự
#define WQERR_ILLEGAL_SESSION         WQERR_INVALID_CLASS - 1

// �Ƿ����ݷ���
#define WQERR_ILLEGAL_SERVICE         WQERR_INVALID_CLASS - 2

// �Ƿ�����
#define WQERR_ILLEGAL_REQUEST         WQERR_INVALID_CLASS - 3

// ��ô����﷨����
#define WQERR_WINDCODE_SYNTAX_ERR     WQERR_INVALID_CLASS - 4

// ��֧�ֵ���ô���
#define WQERR_ILLEGAL_WINDCODE        WQERR_INVALID_CLASS - 5

// ָ���﷨����
#define WQERR_INDICATOR_SYNTAX_ERR    WQERR_INVALID_CLASS - 6

// ��֧�ֵ�ָ��
#define WQERR_ILLEGAL_INDICATOR       WQERR_INVALID_CLASS - 7

// ָ������﷨����
#define WQERR_OPTION_SYNTAX_ERR       WQERR_INVALID_CLASS - 8

// ��֧�ֵ�ָ�����
#define WQERR_ILLEGAL_OPTION          WQERR_INVALID_CLASS - 9

// ������ʱ���﷨����
#define WQERR_DATE_TIME_SYNTAX_ERR    WQERR_INVALID_CLASS - 10

// ��֧�ֵ�������ʱ��
#define WQERR_INVALID_DATE_TIME       WQERR_INVALID_CLASS - 11

// ��֧�ֵ��������
#define WQERR_ILLEGAL_ARG             WQERR_INVALID_CLASS - 12

// �����±�Խ��
#define WQERR_INDEX_OUT_OF_RANGE      WQERR_INVALID_CLASS - 13

// �ظ���WQID
#define WQERR_DUPLICATE_WQID          WQERR_INVALID_CLASS - 14

// ��������ӦȨ��
#define WQERR_UNSUPPORTED_NOAUTH      WQERR_INVALID_CLASS - 15

// ��֧�ֵ���������
#define WQERR_UNSUPPORTED_DATA_TYPE   WQERR_INVALID_CLASS - 16

// ������ȡ������
#define WQERR_DATA_QUOTA_EXCEED       WQERR_INVALID_CLASS - 17

// ��֧�ֵ��������
#define WQERR_ILLEGAL_ARG_COMBINATION WQERR_INVALID_CLASS - 18

// ����ʱ�����鶨��
typedef struct 
{
    int arrLen;
    DATE* timeArray;
} WQDateTimeArray;

typedef char WQWindCode[SMALLSTRING_SIZE];
typedef char WQWindField[SMALLSTRING_SIZE];
// Windcode���鶨��
typedef struct 
{
    int arrLen;
    WQWindCode* codeArray;
} WQWindCodeArray;

// ����ָ�����鶨��
typedef struct 
{
    int arrLen;
    WQWindField* fieldsArray;
} WQWindFieldsArray;

// ���ݽṹ����
// ����ͨ����ú����ӿ�ȡ�������ݶ������������ά����ṹ
// ��ά�ֱ��ǣ�
// ��һάʱ�䣬�ڶ�άƷ�֣�����άָ�ꡣ
typedef struct tagQuantData
{
    WQDateTimeArray ArrDateTime;        // ʱ���б���ͷ��
    WQWindCodeArray ArrWindCode;        // Ʒ���б���ͷ��
    WQWindFieldsArray ArrWindFields;    // ָ���б���ͷ��
    VARIANT MatrixData;                 // ����
} WQData;

// 
// ����Events�ṹ���Ͷ���
typedef enum 
{
    eWQLogin = 1,                       // ��¼�����Ϣ
    eWQResponse,                        // ���ݷ���
    eWQPartialResponse,                 // �������ݷ��أ�����ʱ���ã�
    eWQErrorReport,                     // ������Ϣ
    eWQOthers                           // ������Ϣ
} WQEventType;

// ȡ������Ϣʱ�����Զ���
typedef enum 
{
    eENG = 0,
    eCHN,
} WQLangType;

// ����Events���ݽṹ����
typedef struct tagQuantEvent
{
    int Version;							// �汾�ţ��Ա��������
    WQEventType EventType;                  // Event����
    WQErr ErrCode;							// ������
    WQID RequestID;							// ��Ӧ��request
    WQID EventID;						    // ��ˮ��
    WQData* pQuantData;      			    // ����������
} WQEvent;

// �ص���������
typedef int (WINAPI *IEventHandler) (WQEvent* pEvent, LPVOID lpUserParam);

// ��¼��Ϣ����
typedef struct
{
	int bSilentLogin; ///@param  �Ƿ���ʾ��½�Ի���0����ʾ��1������ʾ
	WCHAR strUserName[SMALLSTRING_SIZE]; ///��¼�û���,��bSilentLogin=trueʱ��Ч
	WCHAR strPassword[SMALLSTRING_SIZE];///��¼����,��bSilentLogin=trueʱ��Ч
} WQAUTH_INFO, *PWQAUTH_INFO;

#ifdef __cplusplus
extern "C" 
{
#endif

    // �������ص�����������һ��ʼ����
    WQErr       WINAPI SetEventHandler(IEventHandler eventHandler);
    // Wind��֤��¼������ɹ���¼���ɵ��ýӿں���
    WQErr       WINAPI WDataAuthorize(WQAUTH_INFO *pAuthInfo);
    // wind��֤�ǳ�
    WQErr       WINAPI WDataAuthQuit();

    // Wind����ʹ��˵����
    // �������е�ȡ���ݵĺ��������첽������
    // һ���ԵĹ�ͨ�﷨Ҫ�����£�
    // reqEventHandler����������������������������ô˻ص������������ݣ�����ʹ��Ԥ��ָ�������ص������������ݡ�
    // lpReqParam�������û��������ص�ʱԭ�����ء�
    // windcode�����������ܹ����ܵ�windcode������ж�����ð�Ƕ��Ÿ���������"000001.SZ,000002.SZ"��
    // indicators�����������ܹ����ܵ�ָ�����ơ�����ж�����ð�Ƕ��Ÿ���������"high,open,low,close"��
    // beginTime��endTime�����������ܹ����ܵ�ʱ��������ַ������ɽ��ܵ��ַ�����������"yyyymmdd"��"yyyy-mm-dd"��"yyyymmdd HHMMSS"����"yyyy-mm-dd HH:MM:SS"
    // params�����������ܹ����ܵĲ���������"param1=value1;param2=value2"����������ð�ǷֺŸ�����
    //
    // WSD������ȡʱ���������ݣ�֧�ֵ�Ʒ�ֶ�ָ���ʱ�䣨���߶�Ʒ�ֵ�ָ���ʱ�䣩
    WQID        WINAPI WSD (LPCWSTR windcode, LPCWSTR indicators, LPCWSTR beginTime, LPCWSTR endTime, LPCWSTR params, IEventHandler reqEventHandler, LPVOID lpReqParam);
    // WSS������ȡ�������ݣ�֧�ֶ�Ʒ�ֶ�ָ�굥ʱ��
    WQID        WINAPI WSS (LPCWSTR windcodes, LPCWSTR indicators, LPCWSTR params, IEventHandler reqEventHandler, LPVOID lpReqParam);
    // WST������ȡ�����������ݣ���Ϊ��Ʒ�ֵĵ�������
    WQID        WINAPI WST (LPCWSTR windcode, LPCWSTR indicators, LPCWSTR beginTime, LPCWSTR endTime, LPCWSTR params, IEventHandler reqEventHandler, LPVOID lpReqParam);
    // WSI������ȡ�����������ݣ���֧�ֵ�Ʒ�����һ������ݣ���������Ϊ�����£�
    WQID        WINAPI WSI (LPCWSTR windcode, LPCWSTR indicators, LPCWSTR beginTime, LPCWSTR endTime, LPCWSTR params, IEventHandler reqEventHandler, LPVOID lpReqParam);
    // WSQ������ȡʵʱ��������(ͨ��speedȡ)��֧�ֶ�Ʒ�����ָ�꣬�ɶ��ģ���ȡ����
    WQID        WINAPI WSQ (LPCWSTR windcodes, LPCWSTR indicators, LPCWSTR params, IEventHandler reqEventHandler, LPVOID lpReqParam);
	// TDQ������ȡʵʱ��������(ͨ��TDFȡ)��֧�ֶ�Ʒ�����ָ�꣬�ɶ��ģ�����ȡ����
	WQID        WINAPI TDQ (LPCWSTR windcodes, LPCWSTR indicators, LPCWSTR params, IEventHandler reqEventHandler, LPVOID lpReqParam);
    // WSET������ȡ������ݼ����ݣ���ָ���ɷֵ�
    WQID        WINAPI WSET (LPCWSTR reportName, LPCWSTR params, IEventHandler reqEventHandler, LPVOID lpReqParam);
    // WSES������ȡ����������� <Flying>
    WQID        WINAPI WSES (LPCWSTR windcode, LPCWSTR indicators, LPCWSTR beginTime, LPCWSTR endTime, LPCWSTR params, IEventHandler reqEventHandler, LPVOID lpReqParam);
    // WSEE������ȡ����ά���� <Flying>
    WQID        WINAPI WSEE (LPCWSTR windcodes, LPCWSTR indicators, LPCWSTR params, IEventHandler reqEventHandler, LPVOID lpReqParam);
    // WST������ȡ�����������ݣ���Ϊ��Ʒ�ֵĵ�������
    // WPF������ȡ��Ϲ�������
	// portfolioName����:֤ȯͶ���������
    WQID		WINAPI WPF (LPCWSTR portfolioName, LPCWSTR viewName, LPCWSTR params, IEventHandler reqEventHandler, LPVOID lpReqParam);
    // WUPF����,����ϴ�
	WQID		WINAPI WUPF(LPCWSTR PortfolioName, LPCWSTR TradeDate, LPCWSTR WindCode, LPCWSTR Quantity, LPCWSTR CostPrice, LPCWSTR params, IEventHandler reqEventHandler, LPVOID lpReqParam);
	// WEQS���������֤ȯɸѡ���ܡ���Ҫ������ն���Ԥ�ȶ���ɸѡ����
    WQID		WINAPI WEQS (LPCWSTR filterName, LPCWSTR params, IEventHandler reqEventHandler, LPVOID lpReqParam);
    // TDays����������ָ������������
    WQID        WINAPI TDays (LPCWSTR beginTime, LPCWSTR endTime, LPCWSTR params, IEventHandler reqEventHandler, LPVOID lpReqParam);
    // TDaysOffset����������һ�����ڼ�һ��ƫ������������һ������
    WQID        WINAPI TDaysOffset (LPCWSTR beginTime, LPCWSTR offset, LPCWSTR params, IEventHandler reqEventHandler, LPVOID lpReqParam);
    // TDaysCount���������������������ڵļ��
    WQID        WINAPI TDaysCount (LPCWSTR beginTime, LPCWSTR endTime, LPCWSTR params, IEventHandler reqEventHandler, LPVOID lpReqParam); 

	// EDB����, ȡ��������
	WQID WINAPI EDB(LPCTSTR windcode, LPCTSTR beginTime, LPCTSTR endTime, LPCTSTR params, IEventHandler reqEventHandler, LPVOID lpReqParam);

	// WPD������ȡ�����������
    WQID        WINAPI WPD (LPCWSTR portfolioName, LPCWSTR indicators, LPCWSTR beginTime, LPCWSTR endTime, LPCWSTR params, IEventHandler reqEventHandler, LPVOID lpReqParam);
    // WPS������ȡ��϶�ά����
    WQID        WINAPI WPS (LPCWSTR portfolioName, LPCWSTR indicators, LPCWSTR params, IEventHandler reqEventHandler, LPVOID lpReqParam);
    

	//�ز⺯��
	WQID  BktStart(LPCWSTR StrategyName, LPCWSTR StartDate, LPCWSTR EndDate, LPCWSTR params, ::IEventHandler reqEventHandler, LPVOID lpReqParam);
	WQID  BktQuery(LPCWSTR qrycode, LPCWSTR qrytime, LPCWSTR params, ::IEventHandler reqEventHandler, LPVOID lpReqParam);
	WQID  BktOrder(LPCWSTR TradeTime, LPCWSTR SecurityCode, LPCWSTR TradeSide, LPCWSTR TradeVol, LPCWSTR params, ::IEventHandler reqEventHandler, LPVOID lpReqParam);
	WQID  BktStatus(LPCWSTR params, ::IEventHandler reqEventHandler, LPVOID lpReqParam);
	WQID  BktEnd(LPCWSTR params, ::IEventHandler reqEventHandler, LPVOID lpReqParam);
	WQID  BktSummary(LPCWSTR BktID, LPCWSTR View, LPCWSTR params, ::IEventHandler reqEventHandler, LPVOID lpReqParam);
	WQID  BktDelete(LPCWSTR BktID, LPCWSTR params, ::IEventHandler reqEventHandler, LPVOID lpReqParam);
	WQID  BktStrategy(LPCWSTR params, ::IEventHandler reqEventHandler, LPVOID lpReqParam);
	WQID  BktFocus(LPCWSTR StrategyID,LPCWSTR params, ::IEventHandler reqEventHandler, LPVOID lpReqParam);
	WQID  BktShare(LPCWSTR StrategyID,LPCWSTR params, ::IEventHandler reqEventHandler, LPVOID lpReqParam);

	// ȡ����������
    WQErr       WINAPI CancelRequest(WQID id);
    // ȡ����������
    WQErr       WINAPI CancelAllRequest();
    // ȡ�������˵����Ϣ
    const WCHAR* WINAPI WErr(WQErr errCode, WQLangType lang);
	// ȡWindCode�б�
	WQID        WINAPI htocode (LPCWSTR code, LPCWSTR sec_type, LPCWSTR params, IEventHandler reqEventHandler, LPVOID lpReqParam);

	//////////////////////////////////////////////////////////////////////////
	//appӦ�����
	//������App��֤����
	WQID WINAPI WAppAuth(LPCWSTR appKey, LPCWSTR appSecret, LPCWSTR params, IEventHandler reqEventHandler, LPVOID lpReqParam);

	//app���ݴ��ͺ���
	//type_id :	message_put; message_get; app_version; app_fps; app_data_get; app_data_put
	WQID WINAPI WAppMessage(LPCWSTR type_id, LPCWSTR message, LPCWSTR params, IEventHandler reqEventHandler, LPVOID lpReqParam);
	//////////////////////////////////////////////////////////////////////////

    // Internal Use
    int         WINAPI GetInternalStatus();

	// BBQ������ȡʵʱ���м�ծȯ����(ͨ��BBQȡ)��֧�ֶ�Ʒ�����ָ�꣬�ɶ��ģ��ɿ���
	WQID        WINAPI BBQ (LPCWSTR windcodes, LPCWSTR indicators, LPCWSTR params, IEventHandler reqEventHandler, LPVOID lpReqParam);

#ifdef __cplusplus
}
#endif

#endif // _WINDQUANTAPI_H