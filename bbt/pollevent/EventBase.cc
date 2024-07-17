#include <bbt/pollevent/EventBase.hpp>
#include <bbt/base/assert/Assert.hpp>

namespace bbt::pollevent
{

EventBase::EventBase()
    :m_ev_base(event_base_new())
{
    Assert(m_ev_base != nullptr);
}

EventBase::~EventBase()
{
    event_base_free(m_ev_base);
}

int EventBase::GetEventNum()
{
    return event_base_get_num_events(m_ev_base, EVENT_BASE_COUNT_ADDED);
}

event_base* EventBase::GetRawBase()
{
    return m_ev_base;
}

int EventBase::GetTimeOfDayCache(struct timeval* tv)
{
    return event_base_gettimeofday_cached(m_ev_base, tv);
}


} // namespace bbt:pollevent
