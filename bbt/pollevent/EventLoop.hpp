#pragma once
#include <bbt/pollevent/EventBase.hpp>

namespace bbt::pollevent
{

class EventLoop
{
public:
    /* 如果设置为true，EventLoop释放时，自动释放base */
    explicit EventLoop(EventBase* base, bool need_auto_free_base = true);
    EventLoop();
    ~EventLoop();

    int                     StartLoop(int opt);
    int                     BreakLoop();

    int                     GetEventNum();

    std::shared_ptr<Event>  CreateEvent(evutil_socket_t fd, short events, const OnEventCallback& onevent_cb);
    EventBase*              GetEventBase() { return m_ev_base; }
    int64_t                 GetEvMonotonic();
    int                     GetTimeOfDayCached(struct timeval *tv);
    int                     GetTime();
private:
    EventBase*              m_ev_base{nullptr};
    bool                    m_auto_free_base{false};
};

} // namespace bbt::pollevent
