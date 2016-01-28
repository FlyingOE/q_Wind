/==============================================================================
/ This is the q loader for Wind RD
/ NOTE: Make sure odbc.k/odbc.dll have both been properly installed in %QHOME% and %QHOME%\w32.
/==============================================================================

/q) \l Wind_RD.q
\d .rd

if[0>=count key`.odbc;system"l odbc.k"];

/q) h:.rd.login`DSN_name
/q) .rd.logout h
/ OR
/q) h:.rd.login"DRIVER=SQL Server;SERVER=...;DATABASE=filesync;UID=...;PWD=...;"
/ OR
/q) h:.rd.start`:.rd.connect
login:{
    dsn:$[
        -11h=t:type x;
            :.odbc.open x;
        10h=t;
            x;
        /default;
            'string x];
    if[not upper[dsn]like"*APP=*";
        dsn,:"APP=Wind_RD.q;"];
    if[not upper[dsn]like"*WSID=*";
        dsn,:"WSID=",("."sv string`int$0x0 vs .z.a),";"];
    :.odbc.open dsn
    };
logout:.odbc.close;
start:{login first read0 hsym x};

/q) .rd.use[h]`filesync
use:{[h;db]
    .rd.eval[h;]"USE ",impl.stringize[db],";"
    };

/q) .rd.tables h        /all tables
/q) .rd.views h         /all views
.rd.tables:.odbc.tables;
.rd.views :.odbc.views;

/q) .rd.eval[h]"SELECT ..."
/q) .rd.eval[h]("SELECT * FROM %1 WHERE TRADE_DT=%2";(`AShareEODPrices;2015.12.10))
/q) .rd.prepare("SELECT * FROM %1 WHERE TRADE_DT=%2";(`AShareEODPrices;2015.12.10))
.rd.eval:{[h;query]
    impl.textEncoding .odbc.eval[h;] .text.utf8_gb18030 .rd.prepare query
    };
.rd.prepare:{[query]
    $[10h=type query:(),query;
    	query;
      (2=count query)and(10h=type query 0);
      	ssr/[query 0;"%",/:string 1+til count(),query 1;impl.stringize'[(),query 1]];
      /default;
        impl.stringize query
	]};

///////////////////////////////////////////////////////////////////////////////

impl.stringize:{
    $[-11h=t:type x;            /`column => [column]
        "[",string[x],"]";
      -14h=t;                   /YYYY.MM.DD => 'YYYYMMDD'
        "'",string[x][0 1 2 3 5 6 8 9],"'";
      10h=t;                    /"string's" => 'string''s'
        "'",ssr[x;"'";"''"],"'";
      11h=t;                    /`schema`table`column => [schema].[table].[column]
        "."sv .z.s'[x];
      0h<=t;                    /("CFFEX";"SZSE") => ('CFFEX','SZSE')
        "(",(","sv .z.s'[x]),")";
      t in -5 -6 -7h;           /remove suffix from integral values
        ssr[.Q.s1 x;"[hij]";""];
      t in -8 -9h;              /remove suffix from and add decimal point to floating-point values 
        {x,$[0>=count where"."=x;".";""]}ssr[.Q.s1 x;"[ef]";""];
      /default;
        .Q.s1 x
        ]
    };
    
impl.textEncoding:{
    if[98h<>type x;:x];
    fs:exec c from meta x where t="C";
    $[0>=count fs;::;![;();0b;fs!.text.gb18030_utf8,/:fs]]x
    };

\d .
\
__EOD__
===============================================================================

\l Wind_RD.q
h:.rd.start`:.rd.connect;
.rd.use[h]`filesync;
.rd.tables h
.rd.views h

.rd.eval[h]("sp_columns %1";`AShareEODPrices)

.rd.eval[h]("SELECT trade_dt,s_info_windcode,s_dq_open,s_dq_high,s_dq_low,s_dq_close,s_dq_tradestatus,s_dq_pctchange FROM %1 WHERE TRADE_DT=%2";
    (`AShareEODPrices;.z.D-1)   )

.rd.logout h
