find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

add_subdirectory(gui)

add_executable(main main.cxx)

target_include_directories(main PRIVATE 
    ${CMAKE_SOURCE_DIR}/includes 
    ${CMAKE_SOURCE_DIR}/test 
    ${SDL2_INCLUDE_DIRS}  
)

target_link_libraries(main PRIVATE 
    SDL2::SDL2 
    SDL2::SDL2main 
    SDL2_image::SDL2_image
    gui
)
