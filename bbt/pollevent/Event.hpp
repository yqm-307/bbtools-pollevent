#pragma once
#include<bbt/pollevent/EventBase.hpp>

namespace bbt::pollevent
{



class Event:
    public std::enable_shared_from_this<Event>
{
    friend void COnEventWapper(evutil_socket_t, short, void*);
public:
    typedef struct {OnEventCallback m_cpp_handler{nullptr};} COnEventWapperParam;

    Event(EventBase* base, evutil_socket_t fd, short listen_events, const OnEventCallback& onevent_cb);
    ~Event();

    int                         StartListen(uint64_t timeout);
    int                         CancelListen(bool need_close_fd = false);
    int                         GetSocket();
    short                       GetEvents();
    EventId                     GetEventId();
    /* 获取超时时间戳，否则返回-1 */
    int64_t                     GetTimeoutMs();

private:
    EventId                     GenerateId();
private:
    EventId                     m_id{0};
    event*                      m_raw_event;
    COnEventWapperParam         m_c_func_wapper_param;
    int64_t                     m_timeout{-1};
};

} // namespace bbt::pollevent
