cmake_minimum_required(VERSION 3.28)
project(GCSDllCollector_src)

set(PEParser_LIB PEParser.cpp)
set(DllWalker_LIB DllWalker.cpp)
set(GCSDllCollector_APP main.cpp)

add_library(GCS.PEParser SHARED ${PEParser_LIB})
add_library(GCS.DllWalker SHARED ${DllWalker_LIB})
add_executable(GCSDllCollector ${GCSDllCollector_APP})

target_compile_options(GCS.PEParser PRIVATE -Wall)
target_compile_options(GCS.DllWalker PRIVATE -Wall)
target_compile_options(GCSDllCollector PRIVATE -Wall)

target_link_libraries(GCS.DllWalker GCS.PEParser)
target_link_libraries(GCSDllCollector GCS.DllWalker)