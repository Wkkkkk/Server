project(driver)

file(GLOB SOURCES_CXX *.cpp *.ui *.h)

add_custom_target(copy_run
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/scripts/run.sh ${COPY_OUTPUT_PATH}/run.sh
        )

add_custom_target(copy_param
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/scripts/param.cmd ${COPY_OUTPUT_PATH}/param.cmd
        )

add_executable(${PROJECT_NAME} ${SOURCES_CXX})
add_dependencies(${PROJECT_NAME} copy_run copy_param)

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} gflags glog::glog core message)