#include <atomic>
#include <event2/event_struct.h>
#include <bbt/base/assert/Assert.hpp>
#include <bbt/pollevent/Event.hpp>
#include <bbt/base/clock/Clock.hpp>

namespace bbt::pollevent
{

void COnEventWapper(evutil_socket_t fd, short events, void* arg) 
{
    auto pthis = reinterpret_cast<Event*>(arg);
    Assert(pthis != nullptr);
    pthis->m_c_func_wapper_param.m_cpp_handler(pthis->shared_from_this(), events);
}

Event::Event(EventBase* base, evutil_socket_t fd, short listen_events, const OnEventCallback& onevent_cb)
    :m_id(GenerateId())
{
    Assert(base != nullptr);
    m_c_func_wapper_param.m_cpp_handler = onevent_cb;
    m_raw_event = event_new(base->GetRawBase(), fd, listen_events, COnEventWapper, this);
    Assert(m_raw_event != nullptr);
}

Event::~Event()
{
    auto ret = CancelListen();
    DebugAssertWithInfo(ret == 0, "it`s a wrong!");
    event_free(m_raw_event);

    m_raw_event = nullptr;
}

int Event::StartListen(uint64_t timeout)
{
    timeval     tm;
    timeval*    tmptr = nullptr;
    timespec    ts;
    int         err;
    int         cache_time;
    int         system_time;

    auto base = event_get_base(m_raw_event);
    if (base == nullptr)
        return -1;

    evutil_timerclear(&tm);
    err = event_base_gettimeofday_cached(base, &tm);    
    if (err != 0)
        return -1;

    cache_time = tm.tv_sec * 1000 + tm.tv_usec / 1000;

    evutil_timerclear(&tm);
    err = gettimeofday(&tm, NULL);
    if (err != 0)
        return -1;

    system_time = tm.tv_sec * 1000 + tm.tv_usec / 1000;
    int diff = system_time - cache_time;
    Assert(diff >= 0);

    if (timeout > 0) {
        timeout += diff;
        m_timeout = system_time + timeout;
        evutil_timerclear(&tm);
        tm.tv_sec  = timeout / 1000;
        tm.tv_usec = (timeout % 1000) * 1000;
        tmptr = &tm;
    } else {
        tmptr = NULL;
    }

    err = event_add(m_raw_event, tmptr);
    if (err != 0) {
        return -1;
    }

    return 0;
}

int Event::CancelListen(bool need_close_fd)
{
    int         err;

    err = event_del(m_raw_event);
    evutil_socket_t socket = GetSocket();

    if (need_close_fd && socket >= 0)
        ::close(socket);

    if (err != 0) {
        return -1;
    }

    return 0;
}

int Event::GetSocket()
{
    return event_get_fd(m_raw_event);
}

short Event::GetEvents()
{
    return event_get_events(m_raw_event);
}

EventId Event::GetEventId()
{
    return m_id;
}

EventId Event::GenerateId()
{
    static std::atomic_uint64_t _id{0};
    return (++_id);
}

int64_t Event::GetTimeoutMs()
{
    auto sec = m_raw_event->ev_timeout.tv_sec;
    auto usec = m_raw_event->ev_timeout.tv_usec;
    return (sec * 1000 + usec / 1000);
}


} // namespace bbt::pollevent
