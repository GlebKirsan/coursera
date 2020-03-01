add_executable(web_server server_stats.cpp
                          stats.cpp)

target_link_libraries(web_server PRIVATE tests)