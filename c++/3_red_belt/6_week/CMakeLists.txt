add_executable(searcher
        main.cpp
        parse.cpp
        search_server.cpp)
target_link_libraries(searcher PRIVATE tests)