add_executable(main main.cxx)
target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/includes)
target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/test)
