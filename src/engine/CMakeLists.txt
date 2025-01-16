add_library(engine
  piece_moves.cxx
)

target_include_directories(engine PRIVATE 
    ${CMAKE_SOURCE_DIR}/includes
)
