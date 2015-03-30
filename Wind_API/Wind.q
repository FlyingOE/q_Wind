/==============================================================================
/ This is the q loader for WindQuantAPI.dll
/ NOTE: Make sure Wind's DLLs are reachable within your %PATH%.
/==============================================================================

/q) \l Wind.q
\d .wind

DLL:hsym`$("Wind_API");

/q) .wind.getTimeout[]
/q) .wind.setTimeout 30*1000
getTimeout:DLL 2:(`getTimeout;1);
setTimeout:DLL 2:(`setTimeout;1);

/q) .wind.login[`w*******;"********"]
/q) .wind.logout`
/ OR
/q) .wind.start hsym`password
login: DLL 2:(`Wind_login ;2);
logout:DLL 2:(`Wind_logout;1);
start:{login .@[;0 2](0,k,1+k:p?":")_p:first read0 x};

/q) .wind.WSD[`000001.SZ;`open`high`low`close`volume;2014.01.01;.z.D;()]
WSD:dateSeq:{[F;c;i;b;e;p]
     delete code from
        update`date$ts,sym:`$code from
            impl.quantData2Table F[c;(),i;b;e;impl.dict2Strings p]
    }DLL 2:(`Wind_wsd;5);

/q) .wind.WSS[`000001.SZ`000002.SZ`000003.SZ;`open`high`low`close`volume;`tradeDate`cycle!(2015.02.12;`W)]
WSS:snapshot:{[F;c;i;p]
    delete code from
        update sym:`$code from
            impl.quantData2Table F[(),c;(),i;impl.dict2Strings p]
    }DLL 2:(`Wind_wss;3);

/q) .wind.WSI[`000001.SZ;`open`high`low`close`volume;2014.01.01T00:00:00;.z.Z;(1#`BarSize)!1#1]
WSI:intradaySeq:{[F;c;i;b;e;p]
    delete code from
        update sym:`$code from
            impl.quantData2Table F[c;(),i;b;e;impl.dict2Strings p]
    }DLL 2:(`Wind_wsi;5);

/q) .wind.WST[`000001.SZ;`last`bid`ask`bid1`bsize1`ask1`asize1;2015.01.01T00:00:00;.z.Z;()]
WST:intradayTicks:{[F;c;i;b;e;p]
    delete code from
        update sym:`$code from
            impl.quantData2Table F[c;(),i;b;e;impl.dict2Strings p]
    }DLL 2:(`Wind_wst;5);

/q) .wind.WSET[`SectorConstituent;`date`sector!(2015.02.13;"全部A股")]
/q) .wind.WSET[`IndexConstituent;`date`windcode!(2015.02.13;`000300.SH)]
WSET:dataSet:{[F;r;p]
    delete wind_code,sec_name from
        update"J"$code,sym:`$wind_code,name:`$sec_name from
            impl.quantData2Table F[r;impl.dict2Strings p]
    }DLL 2:(`Wind_wset;2);

/q) cb:.wind.rtCallback{show(x;.z.P;y);show z};
/q) qid:.wind.WSQ[`000001.SZ`000002.SZ`000003.SZ;`rt_date`rt_time`rt_last`rt_vol;();`cb]
WSQ:rtQuotes:{[F;c;i;p;f]
	F[(),c;(),i;impl.dict2Strings$[p~();()!();p],(1#`REALTIME)!1#1b;f]
	}DLL 2:(`Wind_wsq;4);
rtCallback:{[f;q;d]
    f[q;]delete code from
        update sym:`$code from impl.quantData2Table d
    };

/q) .wind.unsub 3
/q) .wind.unsubAll[]
unsub:DLL 2:(`Wind_cr;1);
unsubAll:DLL 2:(`Wind_car;1);

/q) .wind.tDays[2014.01.01;.z.D;()]
/q) .wind.tDays[2000.01.01;.z.D;`Days`Period!`Weekdays`Q]
tDays:{[F;b;e;p]
    (raze/)last F[b;e;p]
    }DLL 2:(`Wind_tdays;3);

/q) .wind.tDaysOffset[.z.D;-10;()]
/q) .wind.tDaysOffset[.z.D;-2;`Days`Period!`Weekdays`W]
tDaysOffset:{[F;b;o;p]
    (first/)last F[b;o;p]
    }DLL 2:(`Wind_tdaysoff;3)

/q) .wind.tDaysCount[2014.01.01;.z.D;()]
/q) .wind.tDaysCount[2000.01.01;.z.D;`Days`Period!`Weekdays`Q]
tDaysCount:{[F;b;e;p]
    (first/)last F[b;e;p]
    }DLL 2:(`Wind_tdayscnt;3);

/q) .wind.WUPF[... ...
WUPF:portfUpdate:{[F;d;c;q;x;p]
    F[(),d;(),c;(),q;(),x;p]
    }DLL 2:(`Wind_wupf;5);

/==============================================================================
impl.quantData2Table:{
    x:`ts`code`field`data!x;
    flip(`ts`code!flip raze x[`ts],\:/:enlist'[x`code]),
        (x[`field]!raze each flip flip each flip x`data)
    };
    
impl.dict2Strings:{
    if[(0=count x)and(0h=type x);:x];
    if[not 99h=type x;'"not a dict or empty list"];
    :key[x]!{
        $[10h=t:type x;
            x;
          0h<=t;
            '"nyi - complex type";
          -1h=t;
            1#$[x;"Y";"N"];
          -11h<=t;
            string[x];
          -12h=t;
            .z.s`datetime$x;
          -14h=t;
            string[x]_/4 6;
          -15h=t:type x;
            @[string[x]_/4 6;8;:;" "];
          /default;
            '"nyi - unsupported type"]
        }each value x;
    };
    
\d .

\
__EOD__
===============================================================================
