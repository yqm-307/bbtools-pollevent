#include <bbt/pollevent/Event.hpp>
#include <bbt/pollevent/EventLoop.hpp>

namespace bbt::pollevent
{

EventLoop::EventLoop(EventBase* base, bool need_auto_free_base):
    m_ev_base(base),
    m_auto_free_base(need_auto_free_base)
{
}


EventLoop::EventLoop()
    :m_ev_base(new EventBase)
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

    if (err != 0)
        return -1;

    return 0;
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

} // namespace bbt::pollevent
