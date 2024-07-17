#pragma once
#include <bbt/pollevent/Define.hpp>

namespace bbt::pollevent
{

class EventBase
{
public:
    EventBase();
    ~EventBase();

    int                     GetEventNum();
    event_base*             GetRawBase();
    int                     GetTimeOfDayCache(struct timeval* tv);
private:
    event_base* m_ev_base{nullptr};
};

} // namespace bbt::pollevent
