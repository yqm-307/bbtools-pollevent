#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>
#include <bbt/base/clock/Clock.hpp>
#include <bbt/pollevent/EventLoop.hpp>
#include <bbt/pollevent/Event.hpp>

using namespace bbt::pollevent;

BOOST_AUTO_TEST_SUITE(EventLoop_Test_Suit)

BOOST_AUTO_TEST_CASE(test_time_count)
{
    int count = 0;
    EventLoop* loop = new EventLoop();

    auto event1 = loop->CreateEvent(0, EventOpt::PERSIST,
    [&count](auto event, short events){
        if (count < 10)
            ++count;
        printf("%d\n", count);
    });

    event1->StartListen(10);

    auto event2 = loop->CreateEvent(0, EventOpt::TIMEOUT,
    [&event1, &count](auto event, short events){
        event1->CancelListen();
        printf("cancel\n");
    });

    event2->StartListen(200);
    loop->StartLoop(EventLoopOpt::LOOP_NORMAL);
    BOOST_CHECK(count == 10);

}

BOOST_AUTO_TEST_CASE(t_timer_interval)
{
    const int timeout_ms = 1000;
    EventLoop* loop = new EventLoop();
    // std::vector<std::shared_ptr<Event>> events;
    std::map<EventId, uint64_t> regist_time_map;
    std::map<EventId, uint64_t> timeout_map;


    for (int i = 0; i < 100000; ++i) {
        
        auto event = loop->CreateEvent(0, EventOpt::TIMEOUT,
        [&timeout_map](auto event, auto){
            timeout_map.insert(std::make_pair(event->GetEventId(), bbt::clock::gettime()));
        });

        event->StartListen(timeout_ms);
        // events.push_back(event);
        regist_time_map.insert(std::make_pair(event->GetEventId(), bbt::clock::gettime()));
    }

    auto max_end_ts = bbt::clock::nowAfter(bbt::clock::seconds(2));
    while (!bbt::clock::is_expired<bbt::clock::milliseconds>(max_end_ts))
    {
        loop->StartLoop(EventLoopOpt::LOOP_NONBLOCK);
        std::this_thread::sleep_for(bbt::clock::milliseconds(10));
    }

    for (auto it : regist_time_map) {
        auto diff = timeout_map[it.first] - it.second;
        BOOST_CHECK_MESSAGE(diff >= timeout_ms, "diff=" << diff);
    }
}

BOOST_AUTO_TEST_SUITE_END()