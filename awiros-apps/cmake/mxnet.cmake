include_directories(${PROJECT_NAME} SYSTEM PUBLIC "/usr/local/include/mxnet/Include")

#add_library(mxnet_cc SHARED IMPORTED)
#set_target_properties(mxnet_cc PROPERTIES IMPORTED_LOCATION /usr/local/include/mxnet/libs/libmxnet.so)

target_compile_definitions(${PROJECT_NAME} PRIVATE -DENABLE_MXNET)
#target_link_libraries(${PROJECT_NAME} mxnet_cc)
