#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>
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

BOOST_AUTO_TEST_SUITE_END()