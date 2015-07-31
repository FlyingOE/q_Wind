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
start:{
    login .@[;0 2](0,k,1+k:p?":")_p:first read0 x
    };

/q) .wind.WSD[`000001.SZ;`open`high`low`close`volume;2014.01.01;.z.D;()]
/q) .wind.WSD[`000001.SZ`600000.SH;`volume;2014.01.01;.z.D;()]
WSD:dateSeq:{[F;c;i;b;e;p]
     delete code from
        update`date$ts,sym:`$code from
            impl.quantData2Table F[(),c;(),i;b;e;impl.dict2Strings p]
    }DLL 2:(`Wind_wsd;5);

/q) .wind.WSS[`000001.SZ`000002.SZ`600000.SH;`open`high`low`close`volume;`tradeDate`cycle!(2015.02.12;`W)]
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
/q) .wind.WSET["PublicOpinions";`windcode`startdate`enddate!`600000.SH,2015.01.09 2015.04.09]
/q) .wind.WSET["Top10ShareHolders";`windcode`year`period!(`600000.SH;2014;"年报")]
/q) .wind.WSET["StockWEST";`startdate`enddate`windcode`orgname`field!(2012.04.09;2015.04.09;`600000.SH;"全部";`date`organization`researcher`EPSA0`EPSE1`EPSE2`EPSE3`netProfitA0`netProfitE1`netProfitE2`netProfitE3`incomeA0`incomeE1`incomeE2`incomeE3)]
/q) .wind.WSET["GuaranteedBond";`windcode`startdate`enddate!`1380274.IB,2014.04.09 2015.04.09]
/q) .wind.WSET["CTD";`windcode`startdate`enddate!`TF1506.CFE,2015.01.09 2015.04.09]
/q) .wind.WSET["FutureOI";`startdate`enddate`varity`wind_code`member_name`order_by!(2014.04.09;2015.04.09;"铝";"全部";"全部";"多")]
/q) .wind.WSET["OptionChain";`date`us_code`position_var`month`call_put!(2015.04.09;`0001.HK;`;"全部";"全部")]
/q) .wind.WSET["ETFConstituent";`date`windcode!2015.04.09,`159901.OF]
/q) .wind.WSET["LeveragedFundInfo";`date`windcode!2015.04.09,`161812.OF]
/q) .wind.WSET["FundMarketSizeChange";`startdate`enddate`frequency!(2014.04.09;2015.04.09;"日")]
WSET:dataSet:{[F;r;p]
    / Field data types normalization logic
    mapping:([]
        C:`code`enddate`wind_code`sec_name,syms;
        a:`code`enddate`sym`name,syms;
        e:(("J"$;`code);(('[;]/)("M"$;ssr[;"月";""]';ssr[;"年";"-"]');`enddate)),
            (`$),/:`wind_code`sec_name,
                syms:`name`type`organization`researcher,
                    `ctd_ib`ctd_sh`ctd_sz,`member_name`us_code`us_name`cash_substitution_mark,
                    `option_var`option_code`option_name`exe_type`call_put`settle_method,
                    `fund_type`class_a_code`class_a_name`class_b_code`class_b_name);
    /NOTE: Need to handle VT_EMPTY/VT_NULL fields...
    (cols[t]inter exec C except a from mapping)_
    ![;();0b;exec a!e from mapping where C in cols t]
        t:flip{$[(not 0h=type x)or(10h=type first x);x;
            {$[0h=type y;x;y]}[((^/)?[0h=t;0Nh;t:type'[x]])$"";]each x
            ]}peach flip impl.quantData2Table F[r;impl.dict2Strings p]
    }DLL 2:(`Wind_wset;2);

/q) cb:.wind.rtCallback{show(x;.z.P;y)}
/q) qid:.wind.WSQ[`000001.SZ`000002.SZ`600000.SH;`rt_date`rt_time`rt_last`rt_vol;();`cb]
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

/q) .wind.WUPF[`TestStrategy;2015.06.01;`600000.SH;1000;17.50;`Owner`HedgeType`Direction!`Wxxxxxxx`Spec`Long]
/q) .wind.WUPF[`TestStrategy;2015.06.01 2015.06.02;`600000.SH`600004.SH;1000 3000;17.50 3.45;`Owner`HedgeType`Direction!(`Wxxxxxx;`Spec`Spec;`Long`Short)]
WUPF:portfUpdate:{[F;n;d;c;q;x;p]
    F[n;(),d;(),c;(),q;(),x;impl.dict2Strings p]
    }DLL 2:(`Wind_wupf;6);

/q) .wind.EDB[`M0009808;2011.01.01;2015.06.29;()]
EDB:{[F;c;b;e;p]
    delete ts,code from
        update date:"d"$ts,sym:`$code from
            impl.quantData2Table F[c;b;e;impl.dict2Strings p]
    }DLL 2:(`Wind_edb;4);

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
          11h=t;
            ","sv string x;
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
