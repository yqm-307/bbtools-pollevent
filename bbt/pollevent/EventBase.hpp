#pragma once
#include <bbt/pollevent/Define.hpp>

namespace bbt::pollevent
{

enum EventBaseConfigFlag: int32_t
{
    NOLOCK                  = event_base_config_flag::EVENT_BASE_FLAG_NOLOCK,
    IGNORE_ENV              = event_base_config_flag::EVENT_BASE_FLAG_IGNORE_ENV,
    STARTUP_IOCP            = event_base_config_flag::EVENT_BASE_FLAG_STARTUP_IOCP,
    NO_CACHE_TIME           = event_base_config_flag::EVENT_BASE_FLAG_NO_CACHE_TIME,
    EPOLL_USE_CHANGELIST    = event_base_config_flag::EVENT_BASE_FLAG_EPOLL_USE_CHANGELIST,
    PRECISE_TIMER           = event_base_config_flag::EVENT_BASE_FLAG_PRECISE_TIMER
};

class EventBase
{
public:
    explicit EventBase(int32_t flag);
    EventBase();
    ~EventBase();

    int                     GetEventNum();
    event_base*             GetRawBase();
    int                     GetTimeOfDayCache(struct timeval* tv);
private:
    event_base*             m_ev_base{nullptr};
    event_config*           m_ev_config{nullptr};
};

} // namespace bbt::pollevent
