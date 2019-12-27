project(test)

file(GLOB SOURCES_CXX *.cpp *.h)

add_executable(test ${SOURCES_CXX})
target_link_libraries(test message core ${GTEST_LIBRARIES})