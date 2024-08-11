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
    :m_id(GenerateId()),
    m_ref_base(base),
    m_mono_timer(evutil_monotonic_timer_new())
{
    Assert(base != nullptr);
    Assert(m_mono_timer != nullptr);
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

    evutil_monotonic_timer_free(m_mono_timer);
    m_mono_timer = nullptr;
}

int Event::StartListen(uint64_t timeout)
{
    timeval     tv;
    timeval*    tmptr = nullptr;
    int         err;

    if (timeout > 0) {
        m_timeout = bbt::clock::nowAfter(bbt::clock::milliseconds(timeout + 1)).time_since_epoch().count();
        evutil_timerclear(&tv);
        tv.tv_sec  = timeout / 1000;
        tv.tv_usec = (timeout % 1000) * 1000 + 1;
        tmptr = &tv;
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

int Event::GetSocket() const
{
    return event_get_fd(m_raw_event);
}

short Event::GetEvents() const
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

int64_t Event::GetTimeoutMs() const
{
    return m_timeout;
}


} // namespace bbt::pollevent
