add_executable(restructurization
        main.cpp
        stats_aggregator.cpp
        stats_aggregator_test.cpp)
target_link_libraries(restructurization PRIVATE tests)