add_executable(double_dispatching
        collide.cpp
        geo2d.cpp)

target_link_libraries(double_dispatching PRIVATE tests)