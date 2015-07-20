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

/q) h:.tdf.sub[([]server:`host1:port1`host2:port2;username:`user1`user2;password:("****";"****"));`SZ`SH;`;`Transaction`Order`OrderQueue;09:00:00]
/q) h:.tdf.sub[([]server:`host1:port1`host2:port2;username:`user1`user2;password:("****";"****"));`;`;`;0]
/q) .tdf.unsub h
/ OR
/q) h:.tdf.start[hsym`connections;`SZ`SH;`;`;0]
sub:{[F;S;m;c;t;s]
    if[()~key`:log;-1"Creating TDF log directory...";system"MKDIR log"];
    F[;(),m;(),c;(),t;$[-19h=type s;s;"t"$s]]
        delete server from S,'exec`host`port!/:.[;(::;1);"I"$]":"vs/:string server from S
    }DLL 2:(`TDF_subscribe;5);
unsub:DLL 2:(`TDF_unsubscribe;1);
start:{[x;m;c;t;s]
    S:flip`server`username`password!(enlist`$l[;0]),
        flip .[;(::;0 2)](0,/:k,'1+k:p?\:":")_'p:(l:"\001"vs/:"\001;\001"vs"\001"sv read0 x)[;1];
    sub[S;m;c;t;s]
    };

/q) .tdf.codeTable[h]`SH
codeTable:{[F;h;m]
    flip`WindCode`Market`Code`ENName`CNName`Type!F[h;m]
    }DLL 2:(`TDF_codeTable;2);
    
/q) .tdf.optionCodeInfo[h]`AG1312.SHF
optionCodeInfo:{[F;h;c]
    (`WindCode`Market`Code`ENName`CNName`Type,
        `Contract`Underlying`CallPut`ExecDate`UnderlyingType`OptionType`PxLimitType`Multiplier`ExecPx`StartDate`EndDate`ExpireDate
        )!F[h;c]
    }DLL 2:(`TDF_optionCodeInfo;2);

/q) .tdf.index_fields h
/q) .tdf.market_fields h
/q) .tdf.future_fields h
index_fields:DLL 2:(`TDF_index_fields;1);
/market_fields:DLL 2:(`TDF_market_fields;1);
/future_fields:DLL 2:(`TDF_future_fields;1);

\d .
\
__EOD__
===============================================================================

h:0N!.tdf.start[`:.tdf.connects;`SH`SZ;`;`;0]
.tdf.codeTable[h]`SZ