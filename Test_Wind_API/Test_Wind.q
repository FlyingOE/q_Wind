/BASEDIR:hsym`$system"cd";
/LIB_DIR:.Q.dd[BASEDIR]$[`w32~.z.o;`..`Release;`..`x64`Release];
\c 25 2000

// Load Wind_API
NDEBUG:0b
/system" "sv("cd";1_string LIB_DIR)
\l kdb+.q
\l Wind.q
/system" "sv("cd";1_string BASEDIR)

///////////////////////////////////////////////////////////////////////////////

-1"WindAPI Version: ",.wind.version[];

-1"=== Variables =================";
show
    .wind.QueryGap;
-1"=== Timeout =================";
show
    .wind.getTimeout[];
show
    .wind.setTimeout 00:01;
-1"=== Login =================";
//NOTE: no longer require username/password, as account is tied to Wind Terminal
    .wind.login[`;`];
/    .wind.logout[];
-1"=== WSD =================";
show
    .wind.WSD[`000001.SZ;`open`high`low`close`volume;"ED-1M";.z.D;()];
show
    .wind.WSD[`000001.SZ`600000.SH;`volume;"ED-21TD";.z.D;()]
-1"=== WSS =================";
show
    .wind.WSS[`000001.SZ`000002.SZ`600000.SH;`open`high`low`close`volume;`tradeDate`cycle!(.z.D;`W)];
-1"=== WSI =================";
show
    .wind.WSI[`000001.SZ`000002.SZ`600000.SH;`open`high`low`close`volume;.z.D;`datetime$1D+.z.D;(1#`BarSize)!1#1];
-1"=== WST =================";
show
    .wind.WST[`000001.SZ;`last`bid`ask`bid1`bsize1`ask1`asize1;.z.D;`datetime$1D+.z.D;()];
-1"=== WSET =================";
show
    .wind.WSET[`SectorConstituent;`date`sectorid!(.z.D;`a001010100000000)];
show
    .wind.WSET["OptionChain";`date`us_code`position_var`month`call_put!(.z.D;`0001.HK;`;"全部";"全部")];
-1"=== WSQ =================";
show
    .wind.WSQ[`000001.SZ`000002.SZ`600000.SH;`rt_date`rt_time`rt_last`rt_vol;();::];
//TODO: How to test WSQ subscription mode?!
/show
/    qid:.wind.WSQ[`000001.SZ`000002.SZ`600000.SH;`rt_date`rt_time`rt_last`rt_vol;();]`callback set .wind.makeCallback{show(.z.P;x);show y};
/    .wind.unsub qid;
/    .wind.unsubAll[];
-1"=== WSES =================";
show
    .wind.WSES[`1000011262000000;`sec_close_avg;.z.D-100;.z.D;(1#`DynamicTime)!1#0];
show
    .wind.WSES[`1000011262000000`1000011263000000`1000011264000000;`sec_close_avg;.z.D-100;.z.D;(1#`DynamicTime)!1#1];
-1"=== WSEE =================";
show
    .wind.WSEE[`1000011262000000`1000011263000000`1000011264000000;`sec_close_avg`sec_turn_avg;`tradeDate`endDate`unit`DynamicTime!(-1 0+.z.D),1 0];
-1"=== h2code =================";
show
    .wind.h2code[("10000875";"10000877");`option;()];
-1"=== tDays =================";
show
    .wind.tDays[2010.01.01;.z.D;`Days`Period!`Weekdays`Q];
-1"=== tDaysOffset =================";
show
    .wind.tDaysOffset[.z.D;-2;`Days`Period!`Weekdays`W];
-1"=== tDaysCount =================";
show
    .wind.tDaysCount[2010.01.01;.z.D;`Days`Period!`Weekdays`Q];
-1"=== EDB =================";
show
    .wind.EDB[`M0009808;.z.D-30;.z.D;()];
show
    .wind.EDB[`M0009808`M0009809;.z.D-30;.z.D;()]
//TODO: How to test PMS functionality?
/-1"=== WPF =================";
/    .wind.WPF
/-1"=== WUPF =================";
/    .wind.WUPF
-2@/:(
    "==========================================================================================";
    " Test_Wind_API >>> If you can read this line, all tests have been completed successfully! ";
    "==========================================================================================");
exit 0