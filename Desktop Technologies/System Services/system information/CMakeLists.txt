cmake_minimum_required(VERSION 3.20)
project(system-information)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/MP) # 并行编译

    add_compile_options(/utf-8)
    add_compile_options(/Zc:__cplusplus)
    add_compile_options(/Zi) # pdb 格式
    # 额外优化
    add_compile_options("$<$<NOT:$<CONFIG:Debug>>:/Oi>") # Intrinsic Function
    add_compile_options("$<$<NOT:$<CONFIG:Debug>>:/Gy>") # function-level linkage
    # 更符合标准
    add_compile_options("/permissive-")

    add_compile_definitions(UNICODE _UNICODE) # Unicode
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
    add_compile_definitions(NOGDI)
    add_compile_definitions(_HAS_EXCEPTIONS=0)

    # 为非 Debug 也生成 pdb
    add_link_options("$<$<NOT:$<CONFIG:Debug>>:/DEBUG>")
    add_link_options("$<$<NOT:$<CONFIG:Debug>>:/OPT:REF>")
    add_link_options("$<$<NOT:$<CONFIG:Debug>>:/OPT:ICF>")
endif()


add_subdirectory("hardware information")
