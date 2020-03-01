add_executable(xml_test
        xml.cpp
        spendings_xml.cpp)

target_link_libraries(xml_test PRIVATE tests)