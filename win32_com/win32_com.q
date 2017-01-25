/==============================================================================
/ This is the q loader for win32_com.dll wrapper.
/==============================================================================

/q) \l win32_com.q
\d .com

DLL:hsym`$("win32_com");

// DLL version
version:DLL 2:(`version;1);

//@ref .com.create
CLSCTX_INPROC_SERVER :0x01;
CLSCTX_INPROC_HANDLER:0x02;
CLSCTX_LOCAL_SERVER  :0x04;
CLSCTX_REMOTE_SERVER :0x10;

//@ref .com.invoke
DISPATCH_PROPERTYGET   :0x02;
DISPATCH_PROPERTYPUT   :0x04;
/DISPATCH_PROPERTYPUTREF:0x08;

/q) app:.com.create[`Excel.Application;.com.CLSCTX_LOCAL_SERVER];
create:DLL 2:(`createCOM;2);

/q) .com.put[app;`Visible;1#1b]
/q) workbooks:.com.get[app;`Workbooks;()]
/q) workbook:.com.get[workbooks;`Add;()]
invoke:{[code;h;getOrPut;propName;args]
	code[h][getOrPut;propName;(),args]
	}DLL 2:(`invokeCOM;4);
.com.get:invoke[;DISPATCH_PROPERTYGET;;];
.com.put:invoke[;DISPATCH_PROPERTYPUT;;];

/q) .com.release workbook
/q) .com.release workbooks
/q) .com.release app
release:DLL 2:(`releaseCOM;1);

\d .

\
__EOD__
===============================================================================

.com.version`
h:.com.createCOM`Excel.Application
.com.releaseCOM h