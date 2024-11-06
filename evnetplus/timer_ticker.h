#ifndef _TIMER_TICKER_H_
#define _TIMER_TICKER_H_

#include <stdint.h>
#include <memory>
#include "timestamp.h"

class Ticker {
public:
    using Ptr = std::shared_ptr<Ticker>;

    /**
     * 此对象可以用于代码执行时间统计，以可以用于一般计时
     * @param min_ms 开启码执行时间统计时，如果代码执行耗时超过该参数，则打印警告日
     */
    Ticker(uint64_t min_ms = 0) {
        _created = _begin = evnetplus::Timestamp::Now().UnixMicro()/1000;
        _min_ms = min_ms;
    }

    ~Ticker() {}

    /**
     * 获取上次resetTime后至今的时间，单位毫秒
     */
    uint64_t elapsedTime() const {return evnetplus::Timestamp::Now().UnixMicro()/1000 - _begin;}

    /**
     * 获取从创建至今的时间，单位毫秒
     */
    uint64_t createdTime() const {return evnetplus::Timestamp::Now().UnixMicro()/1000 - _created;}

    /**
     * 重置计时器
     */
    void resetTime() {_begin = evnetplus::Timestamp::Now().UnixMicro()/1000;}

private:
    uint64_t _min_ms;
    uint64_t _begin;
    uint64_t _created;
};

#endif // _TIMER_TICKER_H_
