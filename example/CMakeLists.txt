cmake_minimum_required(VERSION 4.0)

project(singleton-cpp-exmaple C CXX)
enable_language(C CXX)

set(my_headers
    ../include/singleton-cpp/singleton.h
    ./my_dll.h
    ./my_dll_api.h
)

set(my_includes
    ../include
)

add_library(my_dll SHARED my_dll.cpp ${my_headers})
target_include_directories(my_dll PRIVATE ${my_includes})
target_link_libraries(my_dll PRIVATE  EnTT::EnTT singleton)
target_compile_definitions(my_dll PRIVATE ENTT_ID_TYPE=std::uint64_t ENTT_USE_ATOMIC)


add_executable(my_exe my_exe.cpp ${my_headers})
target_include_directories(my_exe PRIVATE ${my_includes})
target_link_libraries(my_exe PRIVATE EnTT::EnTT singleton my_dll)
target_compile_definitions(my_exe PRIVATE ENTT_ID_TYPE=std::uint64_t ENTT_USE_ATOMIC)