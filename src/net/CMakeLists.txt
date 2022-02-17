file(GLOB_RECURSE znet_sources ${PROJECT_SOURCE_DIR}/src/net/*.cpp)
add_library(znet_shared SHARED ${znet_sources})
target_link_libraries(zbase_shared)
