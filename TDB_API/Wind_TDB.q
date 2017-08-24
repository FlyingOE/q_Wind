//=============================================================================
// This is the q loader for TDB_API_Windows_v2.dll
// NOTE: Make sure Wind TDB's DLLs are reachable within your %PATH%.
//=============================================================================

/q) \l Wind_TDB.q
\d .tdb

/ Path to {@literal TDB_API.dll}<p>
/ NOTE: When loaded from within TorQ, a {@literal $KDBLIB}-based path is constructed.
DLL:hsym`${$[x~"";"";x,y]}[getenv[`KDBLIB];"/",string[.z.o],"/"],"TDB_API"

/ Version of {@literal TDB_API.dll}
version:DLL 2:(`version;1);

/q) .tdb.getTimeout[]
getTimeout:{[F;x]
    `timeout`retries`gap!F[x]*1000 1 1000
    }DLL 2:(`getTimeout;1);

/q) .tdb.setTimeout`timeout`retries`gap!120000 3 1000
setTimeout:{[F;x]
    F . x[`timeout`retries`gap]div 1000 1 1000;
    getTimeout`
    }DLL 2:(`setTimeout;3);

/q) .tdb.getDataSource[]
getDataSource:DLL 2:(`getDataSource;1);

/q) .tdb.setDataSource 0
setDataSource:DLL 2:(`setDataSource;1);

/q) h:.tdb.login[2;`host:port;`w*******;"********"]
login:{[F;l;s;u;p]
    .[F;l,@[;1;"I"$](":"vs string s),(u;p);{'.text.gb18030_utf8 x}]
    }DLL 2:(`TDB_login;5);

/q) h:.tdb.start hsym`:.tdb3.connect
start:{login["H"$ll 2;`$":"sv 2#ll:":"vs l 0;;].@[;0 2](0,k,1+k:p?":")_p:(l:2#read0 x)1};

/q) .tdb.logout h
logout:DLL 2:(`TDB_logout;1);

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
txn_fields:DLL 2:(`TDB_transaction_fields;1);

/q) .tdb.txn[h][`600000.SH;`WindCode`Code`Date`Time`Side`Price`Size;2015.07.01;00:00;23:59:59.999]
/q) .tdb.txn[h][`600000.SH;`WindCode`Code`Date`Time`Side`Price`Size; 0        ;00:00;23:59:59.999]  /today
/q) .tdb.txn[h][`600000.SH;`WindCode`Code`Date`Time`Side`Price`Size;-1        ;00:00;23:59:59.999]  /IPO day
/q) .tdb.txn[h][`600000.SH;`WindCode`Code`Date`Time`Side`Price`Size;2015.07.01;0    ;23:59:59.999]  /from SOD
/q) .tdb.txn[h][`600000.SH;`WindCode`Code`Date`Time`Side`Price`Size;2015.07.01;10:00;0           ]  /till EOD
txn:{[F;h;c;i;d;b;e]
    flip i!F[h;c;i:(),i;
            $[-14h=t:type d;d;d<=0;d;$[t<0;"d";"D"]$d];
            $[-19h=t:type b;b;t in -17 -18h;"t"$b;b<=0;b;$[t<0;"t";"T"]$b];
            $[-19h=t:type e;e;t in -17 -18h;"t"$e;e<=0;e;$[t<0;"t";"T"]$e]]
    }DLL 2:(`TDB_transaction;6);

/q) .tdb.order_fields[]
order_fields:DLL 2:(`TDB_order_fields;1);

/q) .tdb.order[h][`000001.SZ;`Date`Time`WindCode`Code`Index`OrderID`OrderType`Price`Size;2015.07.01;00:00;23:59:59.999]
/q) .tdb.order[h][`000001.SZ;`Date`Time`WindCode`Code`Index`OrderID`OrderType`Price`Size; 0        ;00:00;23:59:59.999] /today
/q) .tdb.order[h][`000001.SZ;`Date`Time`WindCode`Code`Index`OrderID`OrderType`Price`Size;-1        ;00:00;23:59:59.999] /IPO day
/q) .tdb.order[h][`000001.SZ;`Date`Time`WindCode`Code`Index`OrderID`OrderType`Price`Size;2015.07.01;0    ;23:59:59.999] /from SOD
/q) .tdb.order[h][`000001.SZ;`Date`Time`WindCode`Code`Index`OrderID`OrderType`Price`Size;2015.07.01;00:00;0           ] /till EOD
order:{[F;h;c;i;d;b;e]
    flip i!F[h;c;i:(),i;
            $[-14h=t:type d;d;d<=0;d;$[t<0;"d";"D"]$d];
            $[-19h=t:type b;b;t in -17 -18h;"t"$b;b<=0;b;$[t<0;"t";"T"]$b];
            $[-19h=t:type e;e;t in -17 -18h;"t"$e;e<=0;e;$[t<0;"t";"T"]$e]]
    }DLL 2:(`TDB_order;6);

/q) .tdb.orderQueue_fields[]
orderQueue_fields:DLL 2:(`TDB_orderQueue_fields;1);

/q) .tdb.orderQueue[h][`000001.SZ;`Date`Time`WindCode`Code`Side`Price`OrderCount`Sizes;2015.07.01;00:00;23:59:59.999]
/q) .tdb.orderQueue[h][`000001.SZ;`Date`Time`WindCode`Code`Side`Price`OrderCount`Sizes; 0        ;00:00;23:59:59.999]   /today
/q) .tdb.orderQueue[h][`000001.SZ;`Date`Time`WindCode`Code`Side`Price`OrderCount`Sizes;-1        ;00:00;23:59:59.999]   /IPO day
/q) .tdb.orderQueue[h][`000001.SZ;`Date`Time`WindCode`Code`Side`Price`OrderCount`Sizes;2015.07.01;0    ;23:59:59.999]   /from SOD
/q) .tdb.orderQueue[h][`000001.SZ;`Date`Time`WindCode`Code`Side`Price`OrderCount`Sizes;2015.07.01;00:00;0           ]   /till EOD
orderQueue:{[F;h;c;i;d;b;e]
    flip i!F[h;c;i:(),i;
            $[-14h=t:type d;d;d<=0;d;$[t<0;"d";"D"]$d];
            $[-19h=t:type b;b;t in -17 -18h;"t"$b;b<=0;b;$[t<0;"t";"T"]$b];
            $[-19h=t:type e;e;t in -17 -18h;"t"$e;e<=0;e;$[t<0;"t";"T"]$e]]
    }DLL 2:(`TDB_orderQueue;6);

/q) .tdb.tick_fields`
/q) .tdb.tick_fields`FUTURES  /期货指标
/q) .tdb.tick_fields`BIDASK   /盘口指标
/q) .tdb.tick_fields`INDEX    /指数指标
tick_fields:DLL 2:(`TDB_tick_fields;1);

/q) .tdb.tick[h][`600000.SH;`WindCode`Code`Date`Time`Side`Asks`AskSizes`Bids`BidSizes;2015.07.01;00:00;23:59:59.999;0b]
/q) .tdb.tick[h][`600000.SH;`WindCode`Code`Date`Time`Side`Asks`AskSizes`Bids`BidSizes; 0        ;00:00;23:59:59.999;0b]    /today
/q) .tdb.tick[h][`600000.SH;`WindCode`Code`Date`Time`Side`Asks`AskSizes`Bids`BidSizes;-1        ;00:00;23:59:59.999;0b]    /IPO day
/q) .tdb.tick[h][`600000.SH;`WindCode`Code`Date`Time`Side`Asks`AskSizes`Bids`BidSizes;2015.07.01;0    ;23:59:59.999;0b]    /from SOD
/q) .tdb.tick[h][`600000.SH;`WindCode`Code`Date`Time`Side`Asks`AskSizes`Bids`BidSizes;2015.07.01;00:00;0           ;1b]    /till EOD, auto-fill
tick:{[F;h;c;i;d;b;e;f]
    flip i!F[h;c;i:(),i;
            $[-14h=t:type d;d;d<=0;d;$[t<0;"d";"D"]$d];
            $[-19h=t:type b;b;t in -17 -18h;"t"$b;b<=0;b;$[t<0;"t";"T"]$b];
            $[-19h=t:type e;e;t in -17 -18h;"t"$e;e<=0;e;$[t<0;"t";"T"]$e];
            f]
    }DLL 2:(`TDB_tick;7);

/q) .tdb.ohlc_fields[]
ohlc_fields:DLL 2:(`TDB_ohlc_fields;1);

/q) .tdb.ohlc[h][`600000.SH;`WindCode`Code`Date`Time`Open`High`Low`Close`Size`Amount;2015.07.01T00:00;2015.07.01T23:59:59.999;00:00:01;()]  /秒K线
/q) .tdb.ohlc[h][`600000.SH;`WindCode`Code`Date`Time`Open`High`Low`Close`Size`Amount;2015.07.01T00:00;2015.07.01T23:59:59.999;00:00:30;()]  /30秒K线
/q) .tdb.ohlc[h][`600000.SH;`WindCode`Code`Date`Time`Open`High`Low`Close`Size`Amount;2015.07.01T00:00;2015.07.01T23:59:59.999;00:01;()]     /分钟K线
/q) .tdb.ohlc[h][`600000.SH;`WindCode`Code`Date`Time`Open`High`Low`Close`Size`Amount;2015.07.01T00:00;2015.12.31T23:59:59.999;"1D";()]      /日K线
/q) .tdb.ohlc[h][`600000.SH;`WindCode`Code`Date`Time`Open`High`Low`Close`Size`Amount;2015.07.01T00:00;2015.12.31T23:59:59.999;`1W ;()]      /周K线
/q) .tdb.ohlc[h][`600000.SH;`WindCode`Code`Date`Time`Open`High`Low`Close`Size`Amount;2015.07.01T00:00;2015.12.31T23:59:59.999;"2M";()]      /双月K线
/q) .tdb.ohlc[h][`600000.SH;`WindCode`Code`Date`Time`Open`High`Low`Close`Size`Amount;2014.07.01T00:00;2015.12.31T23:59:59.999;"1Q";()]      /季K线
/q) .tdb.ohlc[h][`600000.SH;`WindCode`Code`Date`Time`Open`High`Low`Close`Size`Amount;2013.07.01T00:00;2015.12.31T23:59:59.999;"1H";()]      /半年K线
/q) .tdb.ohlc[h][`600000.SH;`WindCode`Code`Date`Time`Open`High`Low`Close`Size`Amount;2010.07.01T00:00;2015.12.31T23:59:59.999;`1Y ;()]      /年K线
/q) .tdb.ohlc[h][`002075.SZ;`WindCode`Date`Time`Open`High`Low`Close`Size`Amount;2015.06.23T00:00;2015.07.01T23:59:59.999;00:00;`priceAdj`priceAdjDate`priceType`fill!`B,2015.06.24,`Full,1b]
/q) .tdb.ohlc[h][`002075.SZ;`WindCode`Date`Time`Open`High`Low`Close`Size`Amount;2015.06.23T00:00;2015.07.01T23:59:59.999;00:00;`priceAdj`priceType!`F`Net]
ohlc:{[F;h;c;i;b;e;cyc;p]
    flip i!F[h;c;i:(),i;
            $[-15h=t:type b;b;$[t<0;"z";"Z"]$b];
            $[-15h=t:type e;e;$[t<0;"z";"Z"]$e];
            $[(t:type cyc)in -17 -18h;cyc;10h=t;cyc;string cyc];
            p]
    }DLL 2:(`TDB_ohlc;7);

//=============================================================================
\
__EOD__

h:.tdb.start`:.tdb3_L2.connect
hh:.tdb.start`:.tdb3_L1.connect
.tdb.getTimeout`
.tdb.getDataSource`
.tdb.setDataSource 1
update string Type from .tdb.codeTable[h]`
update string Type from .tdb.codeTable[hh]`
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
.tdb.tick[h][`000001.SZ;(,/).tdb.tick_fields each``BIDASK;2016.01.07;09:30;09:45:00.000]
.tdb.tick[hh][`cu1605.SHF;(,/).tdb.tick_fields each``BIDASK`FUTURES;2016.01.07;0;0]
.tdb.tick[hh][`MA605.CZC;(,/).tdb.tick_fields each``BIDASK`FUTURES;2016.01.07;0;0]
.tdb.tick[hh][`y1605.DCE;(,/).tdb.tick_fields each``BIDASK`FUTURES;2016.01.07;0;0]
.tdb.ohlc[h][`002075.SZ;.tdb.ohlc_fields`;2015.06.24T00:00;2015.06.24T23:59:59.999;00:01;()]
.tdb.ohlc[h][`002075.SZ;.tdb.ohlc_fields`;2015.06.24T00:00;2015.06.24T23:59:59.999;00:00:01;()]
.tdb.ohlc[h][`002075.SZ;.tdb.ohlc_fields`;2015.06.24T00:00;2015.06.24T23:59:59.999;00:00:30;()]
.tdb.ohlc[h][`002075.SZ;.tdb.ohlc_fields`;2015.06.24T00:00;2015.06.24T23:59:59.999;`1T;()]
.tdb.ohlc[h][`000001.SZ;.tdb.ohlc_fields`;2015.01.01T00:00;2016.01.08T23:59:59.999;`1D;()]
.tdb.ohlc[h][`000001.SZ;.tdb.ohlc_fields`;2015.01.01T00:00;2016.01.08T23:59:59.999;`5D;()]
.tdb.ohlc[h][`000001.SZ;.tdb.ohlc_fields`;2015.01.01T00:00;2016.01.08T23:59:59.999;`1W;()]
.tdb.ohlc[h][`000001.SZ;.tdb.ohlc_fields`;2015.01.01T00:00;2016.01.08T23:59:59.999;`1M;()]
.tdb.ohlc[h][`000001.SZ;.tdb.ohlc_fields`;2015.01.01T00:00;2016.01.08T23:59:59.999;`2M;()]
.tdb.ohlc[h][`000001.SZ;.tdb.ohlc_fields`;2015.01.01T00:00;2016.01.08T23:59:59.999;`1Q;()]
.tdb.ohlc[h][`000001.SZ;.tdb.ohlc_fields`;2015.01.01T00:00;2016.01.08T23:59:59.999;`1H;()]
.tdb.ohlc[h][`000001.SZ;.tdb.ohlc_fields`;2015.01.01T00:00;2016.01.08T23:59:59.999;`1Y;()]
.tdb.ohlc[h][`600000.SH;`WindCode`Code`Date`Time`Open`High`Low`Close`Size`Amount;2000.07.01T00:00;2015.12.31T23:59:59.999;`5Y ;()]
.tdb.ohlc[h][`600000.SH;.tdb.ohlc_fields`;2015.06.23T00:00;2015.07.01T23:59:59.999;00:00;(1#`priceAdj)!1#`F]
