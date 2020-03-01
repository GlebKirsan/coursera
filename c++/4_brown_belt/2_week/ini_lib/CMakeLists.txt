add_executable(ini_lib
        test_ini.cpp
        ini.cpp)

target_link_libraries(ini_lib PRIVATE tests)