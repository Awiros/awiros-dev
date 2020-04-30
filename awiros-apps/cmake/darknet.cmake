target_include_directories(${child} SYSTEM PRIVATE PUBLIC "/home/awiros-docker/engyn-build/include/ext/darknet/include/")
target_compile_definitions(${child} PRIVATE -DENABLE_DARKNET)
