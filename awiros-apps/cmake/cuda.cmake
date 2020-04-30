target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC "/usr/local/cuda-10.0/include")

target_compile_definitions(${PROJECT_NAME} PRIVATE -DENABLE_CUDA)
target_link_libraries(${PROJECT_NAME} -L/usr/local/cuda-10.0/lib64)
