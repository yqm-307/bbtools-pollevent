set(Wall_Flag "-Wall -Wno-sign-compare -Wno-format -Wno-reorder -Wno-unknown-pragmas")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -std=c++17 ${Wall_Flag} -g")

enable_testing()
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/unit_test)

set(MY_LIBS
    boost_unit_test_framework
    boost_test_exec_monitor
    bbt_pollevent
    ybbt
)

add_executable(Test_eventloop Test_eventloop.cc)
target_link_libraries(Test_eventloop ${MY_LIBS})
add_test(NAME Test_eventloop COMMAND Test_eventloop)