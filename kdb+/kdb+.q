\d .CE

/ Path to {@literal kdb+.dll}<p>
/ NOTE: When loaded from within TorQ, a {@literal $KDBLIB}-based path is constructed. 
DLL:hsym`${$[x~"";"";x,y]}[getenv[`KDBLIB];"/",string[.z.o],"/"],"kdb+"

/ Version of {@literal kdb+.dll}
version:DLL 2:(`version;1);

\d .text

/ Apply a function to either a single line or a few lines of text.
lineOrLines:{$[(t<>11h)and t:type y;x@y;.z.s/:[x;y]]};

/ Encoding conversion: GB18030/GBK/GB2312 to UTF-8
gb18030_utf8:lineOrLines .CE.DLL 2:(`gb18030_utf8;1);

/ Encoding conversion: UTF-8 to GB18030
utf8_gb18030:lineOrLines .CE.DLL 2:(`utf8_gb18030;1);

\d .os

/ Current Working Directory
cwd:$[value;`.os.cwd;
	$[.z.o like"w*";.CE.DLL 2:(`cwd;1);{first system"pwd"}]	];

//==============================================================================
\
__EOD__