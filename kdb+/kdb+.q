\d .CPPlib

DLL:hsym`$"kdb+";

/==============================================================================
\d .text

lineOrList:{$[(t<>11h)and t:type x;y@x;.z.s\:[x;y]]};

gb18030_utf8:lineOrList[;.CPPlib.DLL 2:(`gb18030_utf8;1)];
utf8_gb18030:lineOrList[;.CPPlib.DLL 2:(`utf8_gb18030;1)];

stringize:{x _.Q.s y}$[.z.o like"w*";2;1]

/==============================================================================
\d .opt

.opt.parse:{[cfg;z_x]
    args:(key[cfg]!count[cfg]#enlist""),.Q.opt z_x;
    if[0<count unknown:key[args]except key cfg;
        '","sv"-",/:string unknown];
	:(first')type'[cfg key args]$'args
	};

/==============================================================================
\d .math

round:{(1%k)*floor .5+(k:10 xexp x)*y};
scale:{round[y;]z%x};

// Exponential average
/@ref http://www.timestored.com/b/exponential-moving-average-ema-kdb/
ema:$[(.z.K>=3.1)and(.z.k>=2013.07.07);
	{first[y](1-x)\x*y};	/ Weird specialization of `\', but much more efficient!
	{{z+x*y}\[first y;1-x;x*y]}];

// Convert cumulative rate to incremetal rate
cum2inc:{1_-1+(%':)reverse fills reverse fills(1#0#x),x};

/==============================================================================
\d .util

// Copy an object from another namespace to the current namespace
copyObject:{eval(:;` sv(system"d"),y;x y)};

// "Sleep"
sleep:{system"ping -t ",string[`long$`second$x]," localhost > NUL"};

\d .
/==============================================================================
\
__EOD__
===============================================================================
