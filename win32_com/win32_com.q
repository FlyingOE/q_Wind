//=============================================================================
// This is the q loader for win32_com.dll wrapper.
//=============================================================================

/q) \l win32_com.q
\d .com

/ Path to {@literal win32_com.dll}<p>
/ NOTE: When loaded from within TorQ, a {@literal $KDBLIB}-based path is constructed.
DLL:hsym`${$[x~"";"";x,y]}[getenv[`KDBLIB];"/",string[.z.o],"/"],"win32_com"

/ Version of {@literal win32_com.dll}
version:DLL 2:(`version;1);

/ @see .com.create
CLSCTX_INPROC_SERVER :0x01;
/ @see .com.create
CLSCTX_INPROC_HANDLER:0x02;
/ @see .com.create
CLSCTX_LOCAL_SERVER  :0x04;
/ @see .com.create
CLSCTX_REMOTE_SERVER :0x10;

/ @see .com.invoke
DISPATCH_PROPERTYGET   :0x02;
/ @see .com.invoke
DISPATCH_PROPERTYPUT   :0x04;
//DISPATCH_PROPERTYPUTREF:0x08;

/q) app:.com.create[`Excel.Application;.com.CLSCTX_LOCAL_SERVER];
create:DLL 2:(`createCOM;2);

invoke:{[code;h;getOrPut;propName;args]
	code[h][getOrPut;propName;(),args]
	}DLL 2:(`invokeCOM;4);

/q) .com.put[app;`Visible;1#1b]
/ @see .com.invoke
.com.put:invoke[;DISPATCH_PROPERTYPUT;;];

/q) workbooks:.com.get[app;`Workbooks;()]
/q) workbook:.com.get[workbooks;`Add;()]
/ @see .com.invoke
.com.get:invoke[;DISPATCH_PROPERTYGET;;];

/q) .com.release workbook
/q) .com.release workbooks
/q) .com.release app
release:DLL 2:(`releaseCOM;1);

//=============================================================================
\
__EOD__

.com.version`
h:.com.createCOM`Excel.Application
.com.releaseCOM h