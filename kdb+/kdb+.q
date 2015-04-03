\d .CPPlib

DLL:hsym`$"kdb+";

/==============================================================================
\d .text

impl.lineOrList:{$[type x;y@x;.z.s\:[x;y]]};

gb18030_utf8:impl.lineOrList[;.CPPlib.DLL 2:(`gb18030_utf8;1)];
utf8_gb18030:impl.lineOrList[;.CPPlib.DLL 2:(`utf8_gb18030;1)];

/==============================================================================
\d .opt

.opt.parse:{[cfg;z_x]
	args:(!).@["S\037\036"0:"\036"sv"\037"sv/:0N 2#z_x;0;{`$1_/:string x}];
	if[0<count unknown:k where not(k:key args)in key cfg;
		'","sv"-",/:string unknown];
	:(cfg key args)$'args
	};

/==============================================================================
\d .math

// Exponential average
/@ref http://www.timestored.com/b/exponential-moving-average-ema-kdb/
ema:$[(.z.K>=3.1)and(.z.k>=2013.07.07);
	{first[y](1-x)\x*y};	/ Weird specialization of `\', but much more efficient!
	{{z+x*y}\[first y;1-x;x*y]}];

\d .
/==============================================================================
\
__EOD__
===============================================================================
