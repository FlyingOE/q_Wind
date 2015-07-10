/==============================================================================
/ This is the q loader for TDB_API_Windows_v2.dll
/ NOTE: Make sure Wind TDB's DLLs are reachable within your %PATH%.
/==============================================================================

/q) \l Wind_TDB.q
\d .tdb

DLL:hsym`$("TDB_API");

/q) .tdb.getTimeout[]
/q) .tdb.setTimeout 30*1000
getTimeout:{[F;x]
    `timeout`retries`gap!F[x]*1000 1 1000
    }DLL 2:(`getTimeout;1);
setTimeout:{[F;x]
    F . x[`timeout`retries`gap]div 1000 1 1000;
    getTimeout`
    }DLL 2:(`setTimeout;3);

/q) h:.tdb.login[`host:port;`w*******;"********"]
/q) .tdb.logout[]
/ OR
/q) h:.tdb.start hsym`password
login:{[F;s;u;p]
    F .@[;1;"I"$](":"vs string s),(u;p)
    }DLL 2:(`TDB_login ;4);
logout:DLL 2:(`TDB_logout;1);
start:{
    login[`$l 0;;].@[;0 2](0,k,1+k:p?":")_p:(l:2#read0 x)1
    };

/q) .tdb.codeTable[h]`      /all markets
/q) .tdb.codeTable[h]`SH
codeTable:{[F;h;m]
    flip`WindCode`Code`Market`CNName`ENName`Type!F[h;m]
    }DLL 2:(`TDB_codeTable;2);
    
/q) .tdb.codeInfo[h]`600000.SH
codeInfo:{[F;h;c]
    `WindCode`Code`Market`CNName`ENName`Type!F[h;c]
    }DLL 2:(`TDB_codeInfo;2);

/q) .tdb.tickAB_fields h
/q) .tdb.tickAB[h][`600000.SH;`WindCode`Code`Date`Time`BSFlag`AskPrices`AskVolumes`BidPrices`BidVolumes;2015.07.01T00:00;2015.07.03T23:59:59.999]
tickAB_fields:DLL 2:(`TDB_tickAB_fields;1);
tickAB:{[F;h;c;i;b;e]
    flip i!F[h;c;i:(),i;$[-15h=type b;b;"z"$b];$[-15h=type e;e;"z"$e]]
    }DLL 2:(`TDB_tickAB;5);

/q) .tdb.futureAB_fields h
/q) .tdb.futureAB[h][`IF1507.CF;`WindCode`Code`Date`Time`CurDelta`AskPrices`AskVolumes`BidPrices`BidVolumes;2015.07.01T00:00;2015.07.03T23:59:59.999;0b]
futureAB_fields:DLL 2:(`TDB_futureAB_fields;1);
futureAB:{[F;h;c;i;b;e;ac]
    flip i!F[h;c;i:(),i;$[-15h=type b;b;"z"$b];$[-15h=type e;e;"z"$e];ac]
    }DLL 2:(`TDB_futureAB;6);

/q) .tdb.txn_fields h
/q) .tdb.txn[h][`600000.SH;`WindCode`Code`BSFlag`TradePrice`TradeVolume;2015.07.01T00:00;2015.07.03T23:59:59.999]
txn_fields:DLL 2:(`TDB_transaction_fields;1);
txn:{[F;h;c;i;b;e]
    flip i!F[h;c;i:(),i;$[-15h=type b;b;"z"$b];$[-15h=type e;e;"z"$e]]
    }DLL 2:(`TDB_transaction;5);

\d .
\
__EOD__
===============================================================================

h:.tdb.start`:.tdb.pass
update string Type from .tdb.codeTable[h]`
reverse update string Type from .tdb.codeTable[h]`CF
select count Code by Market,string Type from .tdb.codeTable[h]`