# q_Wind

## kdb+/q interface for Wind APIs (non-COM version)

This project provides kdb+/q support for 4 distinct sets of APIs provided by [Wind Information Co., Ltd][Wind Info], a Chinese financial information and solutions provider:
* [Wind Quant API][], a Quants API that also provides some market data feeds;
* [Wind TDF API][], a read-time data feed API for Chinese financial market data;
  * *Development suspended (prefer utilizing TDF API for any supported languages to construct feedhandler executables directly, which should be much easier than building TDF into a DLL).*
* [Wind TDB API][], a historical database API for what is provided by TDF API at real-time;
* [Wind RD Database][], a Quants research database for global markets.

## Wind 万得 kdb+/q 数据接口（non-COM version）

本项目提供了四个kdb+/q的接口，用于连接[万得Wind][Wind Info]几个不同的编程接口：
* [Wind Quant API][]——量化编程接口（机构版）；
* [Wind TDF API][]——实时市场数据流接口；
  * *暂停开发（推荐使用各种语言的TDF API直接写feedhandler程序，这样做远比把TDF封包成DLL要容易得多）*
* [Wind TDB API][]——对应TDF API的历史市场数据接口；
* [Wind RD Database][]——量化研究数据库。

本项目是使用Visual Studio <del>2013</del>2015编译的工程。与[zwz][]的《[WindKdb+数据及交易接口V1.1][WindKdb+]》不同，本工程对Wind Quant API的支持直接使用`WindQuantData.dll`的API而不须使用基于COM的`WAPIWrapperCpp.dll`。虽然没有使用`WAPIWrapperCpp.dll`，不过本接口已经把所有接数的函数封装为同步函数（`WSQ`/`TDQ`数据订阅函数除外），以方便在q程序内使用。

### License

[Apache License v2.0](http://www.apache.org/licenses/LICENSE-2.0)

### Build

本工程需要使用<del>正版的Visual Studio 2013</del>Visual Studio 2015（包括社区版）进行编译。

<del>如果您使用的是免费版的Visual Studio Express 2013，则本工程还额外需要[Windows Driver Kit (WDK) v7.1.0][WDK]中的头文件和库方可编译。为方便直接使用，[压缩包](./packaged/)里还包括了工程编译后生成的DLL（在Release目录中）。</del>

### Note

万得金融终端安装时，缺省只会把安装目录下的`bin`子目录加入到`PATH`环境变量中。这种情况下只能正常使用32位版的接口。

如果需要使用64位版的接口，请务必在kdb+启动前，先把万得金融终端安装目录下的`x64`子目录加入到`PATH`环境变量最前面，以便让`q.exe`进程能在32位版本之前先找到64位版本的库。

### Change Log

* 2019.09
  * WindQuantData library change caused by Wind WFT upgrade to 'Everest'
* 2019.05
  - WindQuantData API data type change caused by Wind WFT upgrade to '泰山' (Mt. Taishan)
* 2018.04
  * Include dependent headers and libraries into the project
  * Add support for x64 build
  * Add support for the new `WSES` and `WSEE` functions
* 2018.01
  * Upgraded from MSVC 2013 to 2015
  * Various bug fixes for `Wind_RD.q` module
* 2017.05
  * Add support for the new `h2code` function
* 2017.03
  * Move code documentation to [q-doc](https://github.com/FlyingOE/q-doc) format
* 2017.01
  * Upgrade TDB to v3.20161219 API (server-side upgrade)
  * WindQuantAPI queries: missing (N/A) data items return as identities (::) instead of empty lists ()
  * Add preliminary support for COM automation
  * Various bugfixes
* 2016.01
  * Upgrade TDB to v3 API (on-the-wire data compression, faster access)
  * Merged multiple tick data extraction functions (TDB v3 change)
  * Added support for OHLC (a.k.a. K-line) data extraction
* 2015.12
  * Added simple ODBC-based wrapper for RD database
* 2015.08
  * Suspended development for TDF API due to the lack of need
  * More complete API support for TDB
* 2015.07
  * Added support for Wind TDF/TDB APIs -- Only part of the APIs have been implemented, still work in progress...
* 2015.03&ndash;04
  * Various bug fixes
* [2015.03.06](./packaged/q_Wind-2015.03.06.zip)
  * Initial merge into GitHub
  * Added missing support for `WST` function
  * Fixed problem with `WSQ` subscribers being not able to modify global data in q
* [2015.02.27](./packaged/CE.kdb+-2015.02.27.zip)
  * Initial version to support most APIs in `WindQuantData.dll`

### References

[Kx]:    http://www.kx.com
[github_c]:   https://github.com/KxSystems/kdb/tree/master/c/c
[github_w]:   https://github.com/KxSystems/kdb
[WDK]:   http://www.microsoft.com/en-us/download/details.aspx?id=11800
[Wind Info]:      http://www.wind.com.cn/En/
[Wind Quant API]: http://www.dajiangzhang.com/download
[Wind TDF API]: -
[Wind TDB API]: -
[Wind RD Database]: -
[zwz]:            http://www.dajiangzhang.com/u?4bf215b5-2c07-4b70-91ec-09d8269e48e2
[WindKdb+]:       http://www.dajiangzhang.com/q?fc42e518-3ced-4b97-833e-5f6673a7127b

#### About Wind万得

* 公司：<http://www.wind.com.cn>
* 大奖章：<http://www.dajiangzhang.com>
  * <http://www.dajiangzhang.com/q?32571fd0-4861-4f5b-b8e4-5d4e26b06036>

#### About kdb+/q

* 公司：<http://www.kx.com>
  * Wikipedia: <http://en.wikipedia.org/wiki/Q_(programming_language_from_Kx_Systems)>
  * 中文介绍：<http://itfin.f3322.org/opt/cgi/wiki.pl/KdbPlus>
* 开源：<http://code.kx.com>
* GitHub: <https://kxsystems.github.io>
