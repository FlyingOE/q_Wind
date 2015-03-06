# q_Wind

## kdb+/q interface for Wind API (non-COM version)

## Wind 万得 kdb+/q 数据接口（non-COM version）

这是一个[Wind Quant API](http://www.dajiangzhang.com/download)的kdb+接口。

本项目是使用Visual Studio 2013编译的工程。与[zwz](http://www.dajiangzhang.com/u?4bf215b5-2c07-4b70-91ec-09d8269e48e2)的《[WindKdb+数据及交易接口V1.1](http://www.dajiangzhang.com/q?fc42e518-3ced-4b97-833e-5f6673a7127b)》不同，本工程直接使用`WindQuantData.dll`的API而不须使用基于COM的`WAPIWrapperCpp.dll`。虽然没有使用`WAPIWrapperCpp.dll`，不过本接口已经把所有接数的函数封装为同步函数（`.wind.wsq`除外），以方便在q程序内使用。

### License

[Apache License v2.0](http://www.apache.org/licenses/LICENSE-2.0)

### Build

如果您使用正版的Visual Studio 2013，则本工程只需要[Kx](http://www.kx.com)的[`k.h`](http://code.kx.com/wsvn/code/kx/kdb%2B/c/c/k.h)和[`q.lib`](http://code.kx.com/wsvn/code/kx/kdb%2B/w32/q.lib)即可编译通过。

如果您使用的是免费版的Visual Studio Express 2013，则本工程还额外需要[Windows Driver Kit (WDK) v7.1.0](http://www.microsoft.com/en-us/download/details.aspx?id=11800)中的头文件和库方可编译。为方便直接使用，[压缩包](./packaged/)里还包括了工程编译后生成的DLL（在Release目录中）。

### Change Log

* [2015.02.27](./packaged/CE.kdb+-2015.02.27.zip)
  * Initial version to support most APIs in `WindQuantData.dll`
* [2015.03.06](./packaged/q_Wind-2015.03.06.zip)
  * Initial merge into Github
  * Added missing support for WST function
  * Fixed problem with WSQ subscribers that cannot modify global data in q

### References

#### 关于Wind万得

* 公司：http://www.wind.com.cn
* 大奖章：http://www.dajiangzhang.com

#### 关于kdb+/q

* 公司：http://www.kx.com
* 开源：http://code.kx.com
* 中文介绍：http://itfin.f3322.org/opt/cgi/wiki.pl/KdbPlus
* 维基主题：http://en.wikipedia.org/wiki/Q_(programming_language_from_Kx_Systems)
