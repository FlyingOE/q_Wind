/==============================================================================
/ This is the q loader for WindQuantAPI.dll
/ NOTE: Make sure Wind's DLLs are reachable within your %PATH%.
/==============================================================================

/q) \l Wind.q
\d .wind

DLL:hsym`$("Wind_API");

// Wind 查询速度限制：查询间隔（milliseconds）
QueryGap:3000;

/q) .wind.getTimeout[]
/q) .wind.setTimeout 30*1000
getTimeout:DLL 2:(`getTimeout;1);
setTimeout:DLL 2:(`setTimeout;1);

/q) .wind.login[`w*******;"********"]
/q) .wind.logout`
/ OR
/q) .wind.start`:.wind.pass
login: DLL 2:(`Wind_login ;2);
logout:DLL 2:(`Wind_logout;1);
start:{
    login .@[;0 2](0,k,1+k:p?":")_p:first read0 x
    };

/q) .wind.WSD[`000001.SZ;`open`high`low`close`volume;2014.01.01;.z.D;()]
/q) .wind.WSD[`000001.SZ`600000.SH;`volume;2014.01.01;.z.D;()]
/q) .wind.WSD[`000001.SZ;`open`high`low`close`volume;"ED-1M";.z.D;()]     /Latest 1-month data
WSD:{[F;c;i;b;e;p]
    impl.windHack delete code from update`date$ts,sym:`$code from
        impl.quantData2Table F[(),c;(),i;b;e;impl.dict2Strings p]
    }DLL 2:(`Wind_wsd;5);

/q) .wind.WSS[`000001.SZ`000002.SZ`600000.SH;`open`high`low`close`volume;`tradeDate`cycle!(2015.02.12;`W)]
WSS:{[F;c;i;p]
    delete code from update sym:`$code from
        impl.quantData2Table F[(),c;(),i;impl.dict2Strings p]
    }DLL 2:(`Wind_wss;3);

/q) .wind.WSI[`000001.SZ`000002.SZ`600000.SH;`open`high`low`close`volume;2014.01.01T00:00:00;.z.Z;(1#`BarSize)!1#1]
/@ref 分钟线规则：http://www.dajiangzhang.com/q?d98e4a8e-7e94-4205-8149-b5ab413024b0
WSI:{[F;c;i;b;e;p]
    delete code from update sym:`$code from
        impl.quantData2Table F[(),c;(),i;b;e;impl.dict2Strings p]
    }DLL 2:(`Wind_wsi;5);

/q) .wind.WST[`000001.SZ;`last`bid`ask`bid1`bsize1`ask1`asize1;2015.01.01T00:00:00;.z.Z;()]
WST:{[F;c;i;b;e;p]
    delete code from update sym:`$code from
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
WSET:{[F;r;p]
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
            {$[0h=type y;x;y]}[(10h^(^/)?[0h=t;0Nh;t:type'[x]])$"";]each x
            ]}peach flip impl.quantData2Table F[r;impl.dict2Strings p]
    }DLL 2:(`Wind_wset;2);

/q) cb:.wind.rtCallback{show(x;.z.P;y)}
/q) qid:.wind.WSQ[`000001.SZ`000002.SZ`600000.SH;`rt_date`rt_time`rt_last`rt_vol;();`cb]    //subscribe
/q)     .wind.WSQ[`000001.SZ`000002.SZ`600000.SH;`rt_date`rt_time`rt_last`rt_vol;();::]     //strike
/q) qid:.wind.TDQ[`000001.SZ`000002.SZ`600000.SH;`rt_date`rt_time`rt_last`rt_vol;();`cb]
WSQ:{[F;c;i;p;f]
    $[not(::)~f;(::);
        {delete code from update sym:`$code from impl.quantData2Table x}]
        F[(),c;(),i;impl.dict2Strings$[p~();()!();p],(1#`REALTIME)!1#not(::)~f;f]
	}DLL 2:(`Wind_wsq;4);
TDQ:{[F;c;i;p;f]
	F[(),c;(),i;impl.dict2Strings$[p~();()!();p],(1#`REALTIME)!1#1b;f]
	}DLL 2:(`Wind_tdq;4);
rtCallback:{[f;q;d]
    if[2<>count value[f]1;'"callback should take 2 arguments"];
    {[f;q;d]
        f[q;]delete code from
            update sym:`$code from impl.quantData2Table d
        }[f;;]
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

/q) .wind.EDB[`M0009808;2011.01.01;2015.06.29;()]
EDB:{[F;c;b;e;p]
    delete ts,code from update date:"d"$ts,sym:`$code from
        impl.quantData2Table F[c;b;e;impl.dict2Strings p]
    }DLL 2:(`Wind_edb;4);

/q) .wind.WPF["历史放量组合";`PMS.PortfolioDaily;`owner`reportcurrency`startdate`enddate!`W4351010`CNY,2015.07.13 2015.08.13]
/q) .wind.WPF["历史放量组合";`PMS.HoldingDaily;`owner`reportcurrency`tradedate!`W4351010`CNY,2015.08.13]
/q) .wind.WPF["历史放量组合";`PMS.PortfolioInterval;`owner`reportcurrency`startdate`enddate!`W4351010`CNY,2015.07.13 2015.08.13]
/q) .wind.WPF["历史放量组合";`PMS.HoldingInterval;`owner`reportcurrency`startdate`enddate!`W4351010`CNY,2015.07.13 2015.08.13]
/q) .wind.WPF["体验产品";`AMS.PortfolioDailySerial;`reportcurrency`startdate`enddate!`CNY,2015.07.13 2015.08.13]
/q) .wind.WPF["体验产品";`AMS.HoldingDaily;`reportcurrency`tradedate!`CNY,2015.08.13]
/q) .wind.WPF["体验产品";`AMS.PortfolioInterval;`reportcurrency`startdate`enddate!`CNY,2015.07.13 2015.08.13]
/q) .wind.WPF["体验产品";`AMS.HoldingInterval;`reportcurrency`startdate`enddate!`CNY,2015.07.13 2015.08.13]
WPF:{[F;n;v;p]
    fieldMaps:(`AMS.PortfolioDailySerial`AMS.HoldingDaily`AMS.HoldingInterval`AMS.PortfolioInterval,
        `PMS.PortfolioDaily`PMS.PortfolioInterval`PMS.HoldingDaily`PMS.HoldingInterval)!{
            update"D"$string trade_date from x
        },(2#{
            update`$asset_code,`$asset_name,{$[x~();`;`$x]}'[asset_classification],`$asset_account,`$brokers_name,`$trade_currency from x
        }),(
        ::
        ),{
            update`$portfolio_name,`$report_currency,"D"$string trade_date from x
        },{
            update`$report_currency from x
        },2#{
            update`$asset_code,`$asset_name,{$[x~();`;`$x]}'[asset_classification],`$trade_currency from x
        };
    fieldMaps[v]impl.quantData2Table F[n;v;impl.dict2Strings p]
    }DLL 2:(`Wind_wpf;3);

/q) .wind.WUPF[`TestStrategy;2015.06.01;`600000.SH;1000;17.50;`Owner`HedgeType`Direction!`Wxxxxxxx`Spec`Long]
/q) .wind.WUPF[`TestStrategy;2015.06.01 2015.06.02;`600000.SH`600004.SH;1000 3000;17.50 3.45;`Owner`HedgeType`Direction!(`Wxxxxxx;`Spec`Spec;`Long`Short)]
WUPF:{[F;n;d;c;q;x;p]
    impl.quantData2Table F[n;(),d;(),c;(),q;(),x;impl.dict2Strings p]
    }DLL 2:(`Wind_wupf;6);

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
          -15h=t;
            @[string[x]_/4 6;8;:;" "];
          t in -17 -18 -19h;
            string`second$x;
          /default;
            '"nyi - unsupported type"]
        }each value x;
    };

// Hacks to deal with quirks in the data set returned from WindQuantAPI.
impl.windHack:{[T]
    if[`MAXUPORDOWN in cols T;
        T:update{$[()~x;0Ni;-6h=type x;x;`int$x]}each MAXUPORDOWN
            from T];
    :T
    };

\d .

\
__EOD__
===============================================================================
