//=============================================================================
// This is the q loader for Wind RD
// NOTE: Make sure odbc.k/odbc.dll have both been properly installed in %QHOME% and %QHOME%\w32.
//=============================================================================

/q) \l Wind_RD.q
\d .rd

if[()~key`.odbc;
    @[system;"l odbc.k";{'"Failed to load odbc.k: ",x}]
    ];

/ Number of rows to insert per batch in {@see .rd.bulkload}.
BULK_SIZE:1000;     /max for T-SQL

/q) h:.rd.login`DSN_name
/q) h:.rd.login"DRIVER=SQL Server;SERVER=...;DATABASE=filesync;UID=...;PWD=...;"
/q) h:.rd.login"DSN=DSN name;...;"
login:{
    postLogin:{[o]
        tester:{[o;dbType;test] .odbc.eval[o;test];-2"ODBC: ",dbType;1b};
        okay:       @[tester[o;"SQL Server"];"SET QUOTED_IDENTIFIER ON;";{0b}];
        okay:okay or@[tester[o;"MySQL"     ];"SET SQL_MODE=ANSI_QUOTES;";{0b}];
        if[not okay;-2"ODBC: Oracle??"];
        :o};
    dsn:$[-11h=t:type x;
            :postLogin .odbc.open x;
        10h=t;
            x;
        /default;
            'string x];
    if[not upper[dsn]like"*APP=*";
        dsn,:"APP=Wind_RD.q;"];
    if[not upper[dsn]like"*WSID=*";
        dsn,:"WSID=",("."sv string`int$0x0 vs .z.a),";"];
    :postLogin .odbc.open dsn;
    };

/q) h:.rd.start`:.rd.connect
start:{login first read0 hsym x};

/q) .rd.logout h
logout:.odbc.close;

/q) .rd.use[h]`filesync
use:{[h;db]
    .rd.eval[h;]"USE ",impl.stringize[db],";"
    };

/q) .rd.tables h        /all tables
.rd.tables:.odbc.tables;

/q) .rd.views h         /all views
.rd.views :.odbc.views;

/q) .rd.eval[h]"SELECT ..."
/q) .rd.eval[h]("SELECT * FROM %1 WHERE TRADE_DT=%2";(`AShareEODPrices;2015.12.10))
.rd.eval:{[h;query]
    impl.textEncoding .odbc.eval[h;] .text.utf8_gb18030 .rd.prepare query
    };

/q) .rd.prepare("SELECT * FROM %1 WHERE TRADE_DT=%2";(`AShareEODPrices;2015.12.10))
.rd.prepare:{[query]
    $[10h=type query:(),query;
        query;
      (2=count query)and(10h=type query 0);
          ssr/[query 0;
            reverse"%",/:string 1+til count(),query 1;
            reverse impl.stringize'[(),query 1]];
      /default;
        impl.stringize query
    ]};

/@see .rd.BULK_SIZE
/q) .rd.bulkload[h][`Table;dataset]    /NOTE: dataset must have same column names & compatible types as in Table!
.rd.bulkload:{[h;tbl;data]
    query_templ:"INSERT INTO %1 (",(","sv"%",/:string 2+til count cols data),") VALUES ";
    query:.rd.prepare(query_templ;tbl,cols data);

    value_templ:"(",(","sv"%",/:string 1+til count cols data),")";
    batches:(0N;.rd.BULK_SIZE)#
        ![data;();0b;a!string,/:a:exec c from meta data where t="s"];   /make sure no sym columns
    batches:{[templ;batch]
        (.rd.prepare enlist[templ],enlist value@)each batch
      }/:[value_templ;batches];

    @[.rd.eval[h];"BEGIN TRANSACTION";                  /SQL Server
        {[x;h] .rd.eval[h]"START TRANSACTION" }[;h]     /MySQL | Oracle
     ];
    @[{[h;query;batch] .rd.eval[h] query,(","sv batch),";" }[h;query;]';
        batches;
        {[x;h] .rd.eval[h]"ROLLBACK;"; 'x }[;h]
     ];
    @[.rd.eval h;"COMMIT;";{'x}];

    :count data;
  };

//=============================================================================

/ Convert q value into ANSI SQL/ODBC-compatible strings
impl.stringize:{
    $[-11h=t:type x;                /`column => "column"
        "\"",string[x],"\"";
      t<0h;
        $[null x;                   /0N? => NULL
            "NULL";
          t in -5 -6 -7h;           /remove suffix from integral values
            {$[last[x]in"hij";-1_x;x]}.Q.s1 x;
          t in -8 -9h;              /remove suffix from and add decimal point to floating-point values 
            {x,$[any".e"in x;"";"."]}{$[last[x]in"ef";-1_x;x]}.Q.s1 x;
          t=-10h;                   /"C" => 'C'
            .z.s enlist x;
          t=-12h;                   /YYYY.MM.DDDhh:mm:ss.n*9 => {ts'YYYY-MM-DD hh:mm:ss.n*6'}
            "{ts'",(-3_@[;10;:;" "]@[string x;4 7;:;"-"]),"'}";
          t=-14h;                   /YYYY.MM.DD => {d'YYYY-MM-DD'}
            "{d'",@[string x;4 7;:;"-"],"'}";
          t=-15h;                   /YYYY.MM.DDThh:mm:ss.n*3 => {ts'YYYY-MM-DD hh:mm:ss.n*3'}
            "{ts'",(@[;10;:;" "]@[string x;4 7;:;"-"]),"'}";
          t=-16h;
            '"nyi: cannot map timespan (",.Q.s1[x],") to SQL";
          t=-17h;                   /hh:mm => {t'hh:mm:00'}
            "{t'",string[x],":00'}";
          t in -18 -19h;            /hh:mm:ss.n*3 => {t'hh:mm:ss.n*3'}
            "{t'",string[x],"'}";
          /default;
            .Q.s1 x];
      10h=t;                        /"string's" => N'string''s'
        "N'",ssr[x;"'";"''"],"'";
      11h=t;                        /`schema`table`column => "schema"."table"."column"
        "."sv .z.s'[x];
      /default                      /("CFFEX";"SZSE") => ('CFFEX','SZSE')
        "(",(","sv .z.s'[x]),")"]
    };

/ Deal with Chinesse character encoding in RD database
impl.textEncoding:{
    if[98h<>type x;:x];
    fs:exec c from meta x where t="C";
    $[0>=count fs;::;![;();0b;fs!.text.gb18030_utf8,/:fs]]x
    };

//=============================================================================
\
__EOD__
