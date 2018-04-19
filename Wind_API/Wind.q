//=============================================================================
// This is the q loader for WindQuantAPI.dll
// NOTE: Make sure Wind's DLLs are reachable within your %PATH%.
//=============================================================================

/q) \l Wind.q
\d .wind

/ Path to {@literal Wind_API.dll}<p>
/ NOTE: When loaded from within TorQ, a {@literal $KDBLIB}-based path is constructed.
DLL:hsym`${$[x~"";"";x,y]}[getenv[`KDBLIB];"/",string[.z.o],"/"],"Wind_API"

/ Version of {@literal Wind_API.dll}
version:DLL 2:(`version;1);

/ Wind查询速度限制：查询间隔
QueryGap:00:00:03t;

/q) .wind.getTimeout[]
getTimeout:{[F;x]
    `time$F x
  }DLL 2:(`getTimeout;1);

/q) .wind.setTimeout 00:00:30
setTimeout:{[F;t]
    `time$F`long$`time$t
  }DLL 2:(`setTimeout;1);

/q) .wind.login[`w*******;"********"]
login: DLL 2:(`Wind_login ;2);

/q) .wind.start`:.wind.pass
start:{login .@[;0 2](0,k,1+k:p?":")_p:first read0 x};

/q) .wind.logout`
logout:DLL 2:(`Wind_logout;1);

/q) .wind.WSD[`000001.SZ;`open`high`low`close`volume;2014.01.01;.z.D;()]
/q) .wind.WSD[`000001.SZ`600000.SH;`volume;2014.01.01;.z.D;()]
/q) .wind.WSD[`000001.SZ;`open`high`low`close`volume;"ED-1M";.z.D;()]     /Latest 1-month data
/<p>
/ 参数{@literal b}和{@literal e}可以使用日期宏来计算相关日期。
/<hr>
/<strong>Date Macros 日期宏（WSD/WSS/WSI/WST）</strong>
/<ul>
/  <li>Abstract dates<p>
/	{@code ""}		系统当前日期<br>
/	{@code SD}		开始日期（{@literal b}）<br>
/	{@code ED}		截止日期（{@literal e}）<br>
/	{@code LQ1}		去年第一季度<br>
/	{@code LQ2}		去年第二季度<br>
/	{@code LQ3}		去年第三季度<br>
/	{@code LYR}		去年年报<br>
/	{@code RQ1}		今年第一季度<br>
/	{@code RQ2}		今年第二季度<br>
/	{@code RQ3}		今年第三季度<br>
/	{@code MRQ}		最新一季度<br>
/	{@code RYF}		本年初<br>
/	{@code RHYF}	下半年初<br>
/	{@code RMF}		本月初<br>
/	{@code RWF}		本周一<br>
/	{@code LWE}		上周末<br>
/	{@code LME}		上月末<br>
/	{@code LHYE}	上半年末<br>
/	{@code LYE}		上年末<br>
/	{@code IPO}		上市首日<br>
/  </li>
/  <li>Date offset units<br>
/	{@code TD}	交易日<br>
/	{@code D}	日历日<br>
/	{@code W}	日历周<br>
/	{@code M}	日历月<br>
/	{@code Q}	日历季度<br>
/	{@code S}	日历半年<br>
/	{@code Y}	日历年<br>
/  </li>
/  <li>Sample usage<br>
/	{@code -1W}		当前日的一周前<br>
/	{@code SD+5TD}	开始日后第5个交易日<br>
/  </li>
/</ul>
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
/@see <a href="http://www.dajiangzhang.com/q?d98e4a8e-7e94-4205-8149-b5ab413024b0">分钟线规则</a>
WSI:{[F;c;i;b;e;p]
    r:impl.quantData2Table F[(),c;(),i;b;e;impl.dict2Strings p];
    $[`windcode in cols r;
        delete code,windcode from update sym:`$windcode from r;
        delete code from update sym:`$code from r]
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

/q) callback:.wind.makeCallback{show(x;.z.P;y)}
/q) qid:.wind.WSQ[`000001.SZ`000002.SZ`600000.SH;`rt_date`rt_time`rt_last`rt_vol;();`callback]  //subscribe
/q)     .wind.WSQ[`000001.SZ`000002.SZ`600000.SH;`rt_date`rt_time`rt_last`rt_vol;();::]			//strike
/q) qid:.wind.TDQ[`000001.SZ`000002.SZ`600000.SH;`rt_date`rt_time`rt_last`rt_vol;();`callback]
/ @see .wind.makeCallback
WSQ:{[F;c;i;p;f]
    $[not(::)~f;(::);
        {delete code from update sym:`$code from impl.quantData2Table x}]
        F[(),c;(),i;impl.dict2Strings$[p~();()!();p],(1#`REALTIME)!1#not(::)~f;f]
  }DLL 2:(`Wind_wsq;4);

/ @see .wind.makeCallback
/ @deprecated {@literal WindDataQuant.dll} does not yet support this interface.
TDQ:{[F;c;i;p;f]
    F[(),c;(),i;impl.dict2Strings$[p~();()!();p],(1#`REALTIME)!1#1b;f]
  }DLL 2:(`Wind_tdq;4);

/ @see .wind.WSQ
/ @see .wind.TDQ
makeCallback:{[f]
    if[2<>count value[f]1;'"callback should take 2 arguments"];
    {[f;q;d]
        f[q;]delete code from
            update sym:`$code from impl.quantData2Table d
      }[f;;]
  };

/q) .wind.unsub 3
unsub:DLL 2:(`Wind_cr;1);

/q) .wind.unsubAll[]
unsubAll:DLL 2:(`Wind_car;1);

/q) .wind.WSES[`1000011262000000;`sec_close_avg;.z.D-100;.z.D;(1#`DynamicTime)!1#0]
/q) .wind.WSES[`1000011262000000`1000011263000000`1000011264000000;`sec_close_avg;.z.D-100;.z.D;(1#`DynamicTime)!1#1]
WSES:{[F;c;i;b;e;p]
    impl.windHack update`date$ts,`$code from
        impl.quantData2Table F[(),c;i;b;e;impl.dict2Strings p]
  }DLL 2:(`Wind_wses;5);

/q) .wind.WSEE[`1000011262000000`1000011263000000`1000011264000000;`sec_close_avg`sec_turn_avg;`tradeDate`endDate`unit`DynamicTime!(-1 0+.z.D),1 0]
WSEE:{[F;c;i;p]
    impl.windHack update`$code from
        impl.quantData2Table F[(),c;(),i;impl.dict2Strings p]
  }DLL 2:(`Wind_wsee;3);

/q) .wind.h2code[`000001                ;`stocka;()]
/q) .wind.h2code[`000001`600000         ;`stocka;()]
/q) .wind.h2code[`IC00`A1705            ;`future;()]
/q) .wind.h2code[("10000875";"10000877");`option;()]
/q) .wind.h2code[`000001`000016         ;`index ;()]
/q) .wind.h2code[`000001                ;`fund  ;()]
/q) .wind.h2code[`000001`000002         ;`bond  ;()]
/q) .wind.h2code[`000001`000002         ;`      ;()]	/ enumerate all possible WindCodes
h2code:{[F;c;t;p]
    ![res;();0b;]a!(`$),/:a:exec c from meta
        res:delete ts,code from
            impl.quantData2Table F[(),c;$[`~t;`all;""~t;`all;t];impl.dict2Strings p]
  }DLL 2:(`Wind_htocode;3)

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
/q) .wind.EDB[`M0009808`M0009809;2011.01.01;2015.06.29;()]
EDB:{[F;c;b;e;p]
    delete ts,code from update date:"d"$ts,sym:`$code from
        impl.quantData2Table F[(),c;b;e;impl.dict2Strings p]
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

//=============================================================================

/ Convert a query resultset into tabular format.
impl.quantData2Table:{
    x:`ts`code`field`data!x;
    :impl.fixNA flip(`ts`code!flip raze x[`ts],\:/:enlist'[x`code]),
                (x[`field]!raze each flip flip each flip x`data)
  };
    
/ Convert a query parameter dict to a dict of strings.
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

/ <strong>Hack</strong> to try our best to fill in missing values from Wind's data set.
impl.fixNA:{[T]
    Tmeta:(min,max,{any 101h=x})each\:flip?[T;();0b;a!{type each x},/:a:cols T];
    Ttype:{?[101h=x[;0];x[;1];x[;0]]}a!Tmeta a:where flip[Tmeta][;2];
    :![T;();0b;({$[x~(::);y;x]}\:),/:{(x;y)}'[key Ttype;{$[x=101h;::;x$""]}each Ttype]];
  };

/ <strong>Hack</strong> to deal with quirks in the data set returned from WindQuantAPI.
impl.windHack:{[T]
    if[`MAXUPORDOWN in cols T;
        T:update{$[()~x;0Ni;-6h=type x;x;`int$x]}each MAXUPORDOWN
            from T];
    :T };

//==============================================================================
\
__EOD__