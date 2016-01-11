/==============================================================================
/ This is the q loader for TDB_API_Windows_v2.dll
/ NOTE: Make sure Wind TDB's DLLs are reachable within your %PATH%.
/==============================================================================

/q) \l Wind_TDB.q
\d .tdb

DLL:hsym`$("TDB_API");

/q) .tdb.getTimeout[]
/q) .tdb.setTimeout`timeout`retries`gap!120000 3 1000
getTimeout:{[F;x]
    `timeout`retries`gap!F[x]*1000 1 1000
    }DLL 2:(`getTimeout;1);
setTimeout:{[F;x]
    F . x[`timeout`retries`gap]div 1000 1 1000;
    getTimeout`
    }DLL 2:(`setTimeout;3);

/q) .tdb.getDataSource[]
/q) .tdb.setDataSource 0
getDataSource:DLL 2:(`getDataSource;1);
setDataSource:DLL 2:(`setDataSource;1);

/q) h:.tdb.login[2;`host:port;`w*******;"********"]
/q) .tdb.logout h
/ OR
/q) h:.tdb.start hsym`:.tdb3.connect
login:{[F;l;s;u;p]
    F . l,@[;1;"I"$](":"vs string s),(u;p)
    }DLL 2:(`TDB_login;5);
logout:DLL 2:(`TDB_logout;1);
start:{
    login["H"$ll 2;`$":"sv 2#ll:":"vs l 0;;].@[;0 2](0,k,1+k:p?":")_p:(l:2#read0 x)1
    };

/q) .tdb.codeTable[h]`      /all markets
/q) .tdb.codeTable[h]`SH
codeTable:{[F;h;m]
    flip`WindCode`Code`Market`Name`NameEN`Type`Level`DataSrc!F[h;m]
    }DLL 2:(`TDB_codeTable;2);
    
/q) .tdb.codeInfo[h][`600000.SH;`SH]
codeInfo:{[F;h;c;m]
    `WindCode`Code`Market`Name`NameEN`Type`Level`DataSrc!F[h;c;m]
    }DLL 2:(`TDB_codeInfo;3);

/q) .tdb.txn_fields[]
/q) .tdb.txn[h][`600000.SH;`WindCode`Code`Date`Time`Side`Price`Size;2015.07.01;00:00;23:59:59.999]
/q) .tdb.txn[h][`600000.SH;`WindCode`Code`Date`Time`Side`Price`Size; 0        ;00:00;23:59:59.999]  /today
/q) .tdb.txn[h][`600000.SH;`WindCode`Code`Date`Time`Side`Price`Size;-1        ;00:00;23:59:59.999]  /IPO day
/q) .tdb.txn[h][`600000.SH;`WindCode`Code`Date`Time`Side`Price`Size;2015.07.01;0    ;23:59:59.999]  /from SOD
/q) .tdb.txn[h][`600000.SH;`WindCode`Code`Date`Time`Side`Price`Size;2015.07.01;10:00;0           ]  /till EOD
txn_fields:DLL 2:(`TDB_transaction_fields;1);
txn:{[F;h;c;i;d;b;e]
    flip i!F[h;c;i:(),i;
            $[-14h=t:type d;d;d<=0;d;$[t<0;"d";"D"]$d];
            $[-19h=t:type b;b;b<=0;b;$[t<0;"t";"T"]$b];
            $[-19h=t:type e;e;e<=0;e;$[t<0;"t";"T"]$e]]
    }DLL 2:(`TDB_transaction;6);

/q) .tdb.order_fields[]
/q) .tdb.order[h][`000001.SZ;`Date`Time`WindCode`Code`Index`OrderID`OrderType`Price`Size;2015.07.01;00:00;23:59:59.999]
/q) .tdb.order[h][`000001.SZ;`Date`Time`WindCode`Code`Index`OrderID`OrderType`Price`Size; 0        ;00:00;23:59:59.999] /today
/q) .tdb.order[h][`000001.SZ;`Date`Time`WindCode`Code`Index`OrderID`OrderType`Price`Size;-1        ;00:00;23:59:59.999] /IPO day
/q) .tdb.order[h][`000001.SZ;`Date`Time`WindCode`Code`Index`OrderID`OrderType`Price`Size;2015.07.01;0    ;23:59:59.999] /from SOD
/q) .tdb.order[h][`000001.SZ;`Date`Time`WindCode`Code`Index`OrderID`OrderType`Price`Size;2015.07.01;00:00;0           ] /till EOD
order_fields:DLL 2:(`TDB_order_fields;1);
order:{[F;h;c;i;d;b;e]
    flip i!F[h;c;i:(),i;
            $[-14h=t:type d;d;d<=0;d;$[t<0;"d";"D"]$d];
            $[-19h=t:type b;b;b<=0;b;$[t<0;"t";"T"]$b];
            $[-19h=t:type e;e;e<=0;e;$[t<0;"t";"T"]$e]]
    }DLL 2:(`TDB_order;6);


/q) .tdb.orderQueue_fields[]
/q) .tdb.orderQueue[h][`000001.SZ;`Date`Time`WindCode`Code`Side`Price`OrderCount`Sizes;2015.07.01;00:00;23:59:59.999]
/q) .tdb.orderQueue[h][`000001.SZ;`Date`Time`WindCode`Code`Side`Price`OrderCount`Sizes; 0        ;00:00;23:59:59.999]   /today
/q) .tdb.orderQueue[h][`000001.SZ;`Date`Time`WindCode`Code`Side`Price`OrderCount`Sizes;-1        ;00:00;23:59:59.999]   /IPO day
/q) .tdb.orderQueue[h][`000001.SZ;`Date`Time`WindCode`Code`Side`Price`OrderCount`Sizes;2015.07.01;0    ;23:59:59.999]   /from SOD
/q) .tdb.orderQueue[h][`000001.SZ;`Date`Time`WindCode`Code`Side`Price`OrderCount`Sizes;2015.07.01;00:00;0           ]   /till EOD
orderQueue_fields:DLL 2:(`TDB_orderQueue_fields;1);
orderQueue:{[F;h;c;i;d;b;e]
    flip i!F[h;c;i:(),i;
            $[-14h=t:type d;d;d<=0;d;$[t<0;"d";"D"]$d];
            $[-19h=t:type b;b;b<=0;b;$[t<0;"t";"T"]$b];
            $[-19h=t:type e;e;e<=0;e;$[t<0;"t";"T"]$e]]
    }DLL 2:(`TDB_orderQueue;6);

/q) .tdb.tick_fields`
/q) .tdb.tick_fields`FUTURES
/q) .tdb.tick_fields`BIDASK
/q) .tdb.tick_fields`INDEX
/q) .tdb.tick[h][`600000.SH;`WindCode`Code`Date`Time`Side`Asks`AskSizes`Bids`BidSizes;2015.07.01;00:00;23:59:59.999]
/q) .tdb.tick[h][`600000.SH;`WindCode`Code`Date`Time`Side`Asks`AskSizes`Bids`BidSizes; 0        ;00:00;23:59:59.999]    /today
/q) .tdb.tick[h][`600000.SH;`WindCode`Code`Date`Time`Side`Asks`AskSizes`Bids`BidSizes;-1        ;00:00;23:59:59.999]    /IPO day
/q) .tdb.tick[h][`600000.SH;`WindCode`Code`Date`Time`Side`Asks`AskSizes`Bids`BidSizes;2015.07.01;0    ;23:59:59.999]    /from SOD
/q) .tdb.tick[h][`600000.SH;`WindCode`Code`Date`Time`Side`Asks`AskSizes`Bids`BidSizes;2015.07.01;00:00;0           ]    /till EOD
tick_fields:DLL 2:(`TDB_tick_fields;1);
tick:{[F;h;c;i;d;b;e]
    flip i!F[h;c;i:(),i;
            $[-14h=t:type d;d;d<=0;d;$[t<0;"d";"D"]$d];
            $[-19h=t:type b;b;b<=0;b;$[t<0;"t";"T"]$b];
            $[-19h=t:type e;e;e<=0;e;$[t<0;"t";"T"]$e]]
    }DLL 2:(`TDB_tick;6);

/TODO
/ Provide support for other Wind TDB API functions...

\d .
\
__EOD__
===============================================================================

h:.tdb.start`:.tdb3_L2.connect
hh:.tdb.start`:.tdb3_L1.connect
.tdb.getTimeout`
.tdb.getDataSource`
.tdb.setDataSource 1
update string Type from .tdb.codeTable[h]`
reverse update string Type from .tdb.codeTable[h]`CF
reverse update string Type from .tdb.codeTable[hh]`DCE
select count i by Market,string Type,Level,DataSrc from .tdb.codeTable[h]`
select count i by Market,string Type,Level,DataSrc from .tdb.codeTable[hh]`
.tdb.codeInfo[h][`000001.SZ;`SZ]
.tdb.txn[h][`000001.SZ;.tdb.txn_fields`;0;0;0]
.tdb.txn[h][`000001.SZ;.tdb.txn_fields`;-1;0;0]
.tdb.txn[h][`000001.SZ;.tdb.txn_fields`;2016.01.07;0;0]
.tdb.txn[h][`000001.SZ;.tdb.txn_fields`;2016.01.07;09:30;09:45]
.tdb.txn[hh][`CU1605.SHF;.tdb.txn_fields`;2016.01.07;0;0]
.tdb.order[h][`000001.SZ;.tdb.order_fields`;0;0;0]
.tdb.order[h][`000001.SZ;.tdb.order_fields`;-1;0;0]
.tdb.order[h][`000001.SZ;.tdb.order_fields`;2016.01.07;0;0]
.tdb.order[h][`000001.SZ;.tdb.order_fields`;2016.01.07;09:30;09:45]
.tdb.order[hh][`CU1605.SHF;.tdb.order_fields`;2016.01.07;0;0]
.tdb.orderQueue[h][`000001.SZ;.tdb.orderQueue_fields`;0;0;0]
.tdb.orderQueue[h][`000001.SZ;.tdb.orderQueue_fields`;-1;0;0]
.tdb.orderQueue[h][`000001.SZ;.tdb.orderQueue_fields`;2016.01.07;0;0]
.tdb.orderQueue[h][`000001.SZ;.tdb.orderQueue_fields`;2016.01.07;09:30;09:45]
.tdb.orderQueue[hh][`CU1605.SHF;.tdb.orderQueue_fields`;2016.01.07;0;0]
.tdb.tick_fields each``FUTURES`BIDASK`INDEX
.tdb.tick[h][`000001.SZ;(,/).tdb.tick_fields each``BIDASK;0;0;0]
.tdb.tick[h][`000001.SZ;(,/).tdb.tick_fields each``BIDASK;-1;0;0]
.tdb.tick[h][`000001.SZ;(,/).tdb.tick_fields each``BIDASK;2016.01.07;0;0]
.tdb.tick[h][`000001.SZ;(,/).tdb.tick_fields each``BIDASK;2016.01.07;09:30;09:45]
.tdb.tick[hh][`cu1605.SHF;(,/).tdb.tick_fields each``BIDASK;2016.01.07;0;0]
.tdb.tick[hh][`MA605.CZC;(,/).tdb.tick_fields each``BIDASK`FUTURES;2016.01.07;0;0]
.tdb.tick[hh][`y1605.DCE;(,/).tdb.tick_fields each``BIDASK`FUTURES;2016.01.07;0;0]
\\