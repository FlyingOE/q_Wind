\d .CPPlib

DLL:hsym`$"kdb+";

/==============================================================================
\d .text

impl.lineOrList:{$[type x;y@x;.z.s\:[x;y]]};

gb18030_utf8:impl.lineOrList[;.CPPlib.DLL 2:(`gb18030_utf8;1)];
utf8_gb18030:impl.lineOrList[;.CPPlib.DLL 2:(`utf8_gb18030;1)];

/==============================================================================
\d .opt

parse{[cfg;z_x]
	args:(!).@["S=\001"0:"\001"sv"="sv/:0N 2#z_x;0;{`$1_/:string x}];
	if[0<count unknown:k where not(k:key args)in key cfg;
		'","sv"-",/:string unknown];
	:(cfg key args)$'args
	};

\d .
/==============================================================================
\
__EOD__
===============================================================================
