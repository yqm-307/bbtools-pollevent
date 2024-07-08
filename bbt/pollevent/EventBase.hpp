#pragma once
#include <bbt/pollevent/Define.hpp>

namespace bbt::pollevent
{

class EventBase
{
public:
    EventBase();
    ~EventBase();

    int GetEventNum();
    event_base* GetRawBase();

private:
    event_base* m_ev_base{nullptr};
};

} // namespace bbt::pollevent
