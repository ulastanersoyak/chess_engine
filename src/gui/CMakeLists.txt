find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

add_library(gui
    click_event.cxx
    draw.cxx
)

target_include_directories(gui PRIVATE 
    ${CMAKE_SOURCE_DIR}/includes
    ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(gui PRIVATE 
    SDL2::SDL2
    SDL2_image::SDL2_image
)
