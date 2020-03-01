add_executable(refactoring
        refactoring.cpp
        xml.cpp
        json.cpp)

target_link_libraries(refactoring PRIVATE tests)