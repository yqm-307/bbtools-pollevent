#include <bbt/pollevent/Event.hpp>
#include <bbt/pollevent/EventLoop.hpp>

namespace bbt::pollevent
{

EventLoop::EventLoop(EventBase* base, bool need_auto_free_base):
    m_ev_base(base),
    m_auto_free_base(need_auto_free_base)
{
}


EventLoop::EventLoop():
    m_ev_base(new EventBase),
    m_auto_free_base(true)
{
}

EventLoop::~EventLoop()
{
    if (m_auto_free_base) {
        delete m_ev_base;
        m_ev_base = nullptr;
    }
}

int EventLoop::StartLoop(int opt)
{
    int err = event_base_loop(m_ev_base->GetRawBase(), opt);

    return err;
}

int EventLoop::BreakLoop()
{
    int err = event_base_loopbreak(m_ev_base->GetRawBase());

    if (err < 0)
        return -1;

    return 0;
}

std::shared_ptr<Event> EventLoop::CreateEvent(evutil_socket_t fd, short events, const OnEventCallback& onevent_cb)
{
    auto event_sptr = std::make_shared<Event>(m_ev_base, fd, events, onevent_cb);
    return event_sptr;
}

int EventLoop::GetEventNum()
{
    return m_ev_base->GetEventNum();
}

int64_t EventLoop::GetEvMonotonic() const
{
    timeval tv;
    int64_t timenow_ms = 0;

    event_gettime_monotonic(m_ev_base->GetRawBase(), &tv);
    timenow_ms += tv.tv_sec * 1000;
    timenow_ms += (tv.tv_usec / 1000);
    return timenow_ms;
}

int EventLoop::GetTimeOfDayCached(struct timeval *tv) const
{
    return m_ev_base->GetTimeOfDayCache(tv);
}

int64_t EventLoop::GetTime() const
{
    timeval tv;
    int ret = GetTimeOfDayCached(&tv);
    if (ret != 0)
        return ret;

    return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}


} // namespace bbt::pollevent
