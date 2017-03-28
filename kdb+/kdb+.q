\d .CPPlib

DLL:hsym`$"kdb+";

// DLL version
version:DLL 2:(`version;1);

/==============================================================================
\d .text

lineOrList:{$[(t<>11h)and t:type x;y@x;.z.s\:[x;y]]};

gb18030_utf8:lineOrList[;.CPPlib.DLL 2:(`gb18030_utf8;1)];
utf8_gb18030:lineOrList[;.CPPlib.DLL 2:(`utf8_gb18030;1)];

/==============================================================================
\d .os

// Current Working Directory
cwd:.CPPlib.DLL 2:(`cwd;1);

// "Sleep"
sleep:{system"ping -t ",string[`long$`second$x]," localhost > ",$[.z.o like"w*";"NUL";"/dev/null"]};

\d .
\
__EOD__
===============================================================================