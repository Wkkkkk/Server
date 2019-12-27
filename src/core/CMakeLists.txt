project(core)

file(GLOB SOURCES_CXX *.cpp *.h)

IF (UNIX)
    set(PTHREAD_LIB pthread)
ENDIF ()

add_library(core ${SOURCES_CXX})
target_link_libraries(core ${PTHREAD_LIB})