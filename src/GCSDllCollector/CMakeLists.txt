cmake_minimum_required(VERSION 3.28)
project(GCSDllCollector_src)

set(PEParser_LIB PEParser.cpp)
set(DllWalker_LIB DllWalker.cpp)
set(GCSDllCollector_APP main.cpp)

add_library(GCS.PEParser SHARED ${PEParser})
add_library(GCS.DllWalker SHARED ${DllWalker})
add_executable(GCSDllCollector ${GCSDllCollector_APP})

target_link_libraries(GCS.DllWalker GCS.PEParser)
target_link_libraries(GCSDllCollector GCS.DllWalker GCS.PEParser)