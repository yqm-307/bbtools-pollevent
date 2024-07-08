#pragma once
#include <memory>
#include <functional>
#include <event2/event.h>
#include <event2/thread.h>
#include <bbt/base/assert/Assert.hpp>


namespace bbt::pollevent
{

class Event;
class EventBase;
class EventLoop;

enum EventOpt : short
{
    TIMEOUT                 = EV_TIMEOUT,       // 事件超时时触发
    READABLE                = EV_READ,          // 套接字可读时触发
    WRITEABLE               = EV_WRITE,         // 套接字可写时触发
    CLOSE                   = EV_CLOSED,        // 套接字关闭时触发
    FINALIZE                = EV_FINALIZE,      // 对端关闭事件，相比Close支持更广，监听对端关闭请使用此事件
    PERSIST                 = EV_PERSIST,       // 设置事件监听事件为持续监听的，否则触发一次事件就结束
    SIGNAL                  = EV_SIGNAL,        // 系统事件
};

/**
 * @brief 事件循环的选项
 * 
 */
enum EventLoopOpt
{
    LOOP_NORMAL             = 0,                /* 默认循环，没有事件退出 */
    LOOP_ONCE               = EVLOOP_ONCE,      /* 触发一次 */
    LOOP_NONBLOCK           = EVLOOP_NONBLOCK,  /* ??? */
    LOOP_NO_EXIT_ON_EMPTY   = EVLOOP_NO_EXIT_ON_EMPTY, /* 即使没有任何监听事件也不退出循环 */
};


typedef std::function<void(std::shared_ptr<Event>, short)> OnEventCallback;
typedef uint64_t EventId;


struct __PollEvent_Init
{
    __PollEvent_Init() {
        AssertWithInfo(evthread_use_pthreads() == 0, "init libevent thread-safe failed!");
    }
};

static __PollEvent_Init __detail_init_helper; 
}