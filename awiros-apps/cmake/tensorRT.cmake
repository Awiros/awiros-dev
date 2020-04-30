target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC "/usr/local/include/TensorRT-7.0.0.11/include")


target_compile_definitions(${PROJECT_NAME} PRIVATE -DENABLE_TENSORRT)
target_link_libraries(${PROJECT_NAME} -L/usr/local/include/TensorRT-7.0.0.11/lib)
