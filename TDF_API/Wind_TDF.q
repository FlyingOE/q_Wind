/==============================================================================
/ This is the q loader for TDFAPI30.dll
/ NOTE: Make sure Wind TDF's DLLs are reachable within your %PATH%.
/==============================================================================

/q) \l Wind_TDF.q
\d .tdf

DLL:hsym`$("TDF_API");

/q) .tdb.setTimeout[10000;2;30000]
setTimeout:{[F;hbInterval;hbMissing;openTimeout]
    F .(hbInterval;hbMissing;openTimeout)div 1000 1 1000
    }DLL 2:(`setTimeout;3);

/q) h:.tdf.login[([]server:`host1:port1`host2:port2;username:`user1`user2;password:("****";"****"));`SZ`SH;`;`Transaction`Order`OrderQueue;09:00:00]
/q) h:.tdf.login[([]server:`host1:port1`host2:port2;username:`user1`user2;password:("****";"****"));`;`;`;0]
/q) .tdf.logout h
/ OR
/q) h:.tdf.start[hsym`connections;`SZ`SH;`;`;0]
login:{[F;S;m;c;t;s]
    if[()~key`:log;-1"Creating TDF log directory...";system"MKDIR log"];
    F[;(),m;(),c;(),t;$[-19h=type s;s;"t"$s]]
        delete server from S,'exec`host`port!/:.[;(::;1);"I"$]":"vs/:string server from S
    }DLL 2:(`TDF_login;5);
logout:DLL 2:(`TDF_logout;1);
start:{[x;m;c;t;s]
    S:flip`server`username`password!(enlist`$l[;0]),
        flip .[;(::;0 2)](0,/:k,'1+k:p?\:":")_'p:(l:"\001"vs/:"\001;\001"vs"\001"sv read0 x)[;1];
    login[S;m;c;t;s]
    };

x:hsym`.tdf.connect;

T:([]
    server:`114.80.154.34:6221`114.80.154.34:10051;
    username:`TD4351909001`TDcc0001;
    password:("12597050";"13833692")
    );

/

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
/q) .tdb.txn[h][`600000.SH;`WindCode`Code`Date`Time`BSFlag`TradePrice`TradeVolume;2015.07.01T00:00;2015.07.03T23:59:59.999]
txn_fields:DLL 2:(`TDB_transaction_fields;1);
txn:{[F;h;c;i;b;e]
    flip i!F[h;c;i:(),i;$[-15h=type b;b;"z"$b];$[-15h=type e;e;"z"$e]]
    }DLL 2:(`TDB_transaction;5);

/q) .tdb.order_fields h
/q) .tdb.order[h][`000001.SZ;`Date`Time`WindCode`Code`Index`OrderId`OrderKind`OrderPrice`OrderVolume;2015.07.01T00:00;2015.07.03T23:59:59.999]
order_fields:DLL 2:(`TDB_order_fields;1);
order:{[F;h;c;i;b;e]
    flip i!F[h;c;i:(),i;$[-15h=type b;b;"z"$b];$[-15h=type e;e;"z"$e]]
    }DLL 2:(`TDB_order;5);

/q) .tdb.orderQueue_fields h
/q) .tdb.orderQueue[h][`000001.SZ;`Date`Time`WindCode`Code`Side`Price`OrderItems`ABVolumes;2015.07.01T00:00;2015.07.03T23:59:59.999]
orderQueue_fields:DLL 2:(`TDB_orderQueue_fields;1);
orderQueue:{[F;h;c;i;b;e]
    flip i!F[h;c;i:(),i;$[-15h=type b;b;"z"$b];$[-15h=type e;e;"z"$e]]
    }DLL 2:(`TDB_orderQueue;5);

\

\d .
\
__EOD__
===============================================================================

h:.tdb.start`:.tdb.pass
update string Type from .tdb.codeTable[h]`
reverse update string Type from .tdb.codeTable[h]`CF
select count Code by Market,string Type from .tdb.codeTable[h]`