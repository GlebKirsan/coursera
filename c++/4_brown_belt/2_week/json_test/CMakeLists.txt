add_executable(json_test
        spendings_json.cpp
        json.cpp)
target_link_libraries(json_test PRIVATE tests)