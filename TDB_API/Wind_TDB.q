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

\

/q) .tdb.codeTable[h]`      /all markets
/q) .tdb.codeTable[h]`SH
codeTable:{[F;h;m]
    flip`WindCode`Code`Market`Name`NameEN`Type!F[h;m]
    }DLL 2:(`TDB_codeTable;2);
    
/q) .tdb.codeInfo[h]`600000.SH
codeInfo:{[F;h;c]
    `WindCode`Code`Market`Name`NameEN`Type!F[h;c]
    }DLL 2:(`TDB_codeInfo;2);

/q) .tdb.tickAB_fields h
/q) .tdb.tickAB[h][`600000.SH;`WindCode`Code`Date`Time`Side`Asks`AskSizes`Bids`BidSizes;2015.07.01T00:00;2015.07.03T23:59:59.999]
tickAB_fields:DLL 2:(`TDB_tickAB_fields;1);
tickAB:{[F;h;c;i;b;e]
    flip i!F[h;c;i:(),i;$[-15h=type b;b;"z"$b];$[-15h=type e;e;"z"$e]]
    }DLL 2:(`TDB_tickAB;5);

/q) .tdb.futureAB_fields h
/q) .tdb.futureAB[h][`IF1507.CF;`WindCode`Code`Date`Time`Delta`Asks`AskSizes`Bids`BidSizes;2015.07.01T00:00;2015.07.03T23:59:59.999;0b]
futureAB_fields:DLL 2:(`TDB_futureAB_fields;1);
futureAB:{[F;h;c;i;b;e;ac]
    flip i!F[h;c;i:(),i;$[-15h=type b;b;"z"$b];$[-15h=type e;e;"z"$e];ac]
    }DLL 2:(`TDB_futureAB;6);

/q) .tdb.txn_fields h
/q) .tdb.txn[h][`600000.SH;`WindCode`Code`Date`Time`Side`Price`Size;2015.07.01T00:00;2015.07.03T23:59:59.999]
txn_fields:DLL 2:(`TDB_transaction_fields;1);
txn:{[F;h;c;i;b;e]
    flip i!F[h;c;i:(),i;$[-15h=type b;b;"z"$b];$[-15h=type e;e;"z"$e]]
    }DLL 2:(`TDB_transaction;5);

/q) .tdb.order_fields h
/q) .tdb.order[h][`000001.SZ;`Date`Time`WindCode`Code`Index`OrderID`OrderType`Price`Size;2015.07.01T00:00;2015.07.03T23:59:59.999]
order_fields:DLL 2:(`TDB_order_fields;1);
order:{[F;h;c;i;b;e]
    flip i!F[h;c;i:(),i;$[-15h=type b;b;"z"$b];$[-15h=type e;e;"z"$e]]
    }DLL 2:(`TDB_order;5);

/q) .tdb.orderQueue_fields h
/q) .tdb.orderQueue[h][`000001.SZ;`Date`Time`WindCode`Code`Side`Price`OrderCount`Sizes;2015.07.01T00:00;2015.07.03T23:59:59.999]
orderQueue_fields:DLL 2:(`TDB_orderQueue_fields;1);
orderQueue:{[F;h;c;i;b;e]
    flip i!F[h;c;i:(),i;$[-15h=type b;b;"z"$b];$[-15h=type e;e;"z"$e]]
    }DLL 2:(`TDB_orderQueue;5);
    
/TODO
/ Provide support for other Wind TDB API functions...

\d .
\
__EOD__
===============================================================================

h:.tdb.start`:.tdb3.connect
.tdb.getDataSource`
update string Type from .tdb.codeTable[h]`
reverse update string Type from .tdb.codeTable[h]`CF
select count Code by Market,string Type from .tdb.codeTable[h]`