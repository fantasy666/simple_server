file(GLOB_RECURSE zbase_sources ${PROJECT_SOURCE_DIR}/src/base/*.cpp)
add_library(zbase_shared SHARED ${zbase_sources})