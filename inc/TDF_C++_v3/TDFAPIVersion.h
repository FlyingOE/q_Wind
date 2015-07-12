#ifndef __WIND_TDF_API_VERSION_H__
#define __WIND_TDF_API_VERSION_H__

#ifdef __cplusplus
extern "C" {
#endif

static const char* TDF_Version()
{
    //TDF API Version: 2383-20140115.1 Comment: 添加期权支持
    //2408-20140116.1: 支持获取期权代码表接口。
    //2448-20140117.1: 添加connection id。
    //2501-20140121.1: 将期货连线参数化，在DATA_TYPE_FLAG中添加DATA_TYPE_FUTURE_CX枚举支持。
    //2504-20140121.1: 将TDF_OPEN_SETTING 中的nConnectionID放到结构体的后面去。
    //2565-20140124.1:（1）将vld移除；（2）修复闭市消息nDataLen为0；
    //2654-20140211.1:（1）更新期权demo
    return "2654-20140211.1";
}

#ifdef __cplusplus
}
#endif

#endif