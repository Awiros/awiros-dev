include_directories(${PROJECT_NAME} SYSTEM PUBLIC "/usr/local/include/tensorflow" )
include_directories(${PROJECT_NAME} SYSTEM PUBLIC "/usr/local/include/google" )
include_directories(${PROJECT_NAME} SYSTEM PUBLIC "/usr/local/include/tensorflow/bazel-genfiles" )
include_directories(${PROJECT_NAME} SYSTEM PUBLIC "/usr/local/include/tensorflow/tensorflow" )
include_directories(${PROJECT_NAME} SYSTEM PUBLIC "/usr/local/include/tensorflow/third_party" )
include_directories(${PROJECT_NAME} SYSTEM PUBLIC "/usr/local/include/tensorflow/tensorflow/contrib/makefile/downloads/absl" )
include_directories(${PROJECT_NAME} SYSTEM PUBLIC "/usr/local/include/tensorflow/tensorflow/contrib/makefile/downloads/eigen" )
include_directories(${PROJECT_NAME} SYSTEM PUBLIC "/usr/local/include/tensorflow/tensorflow/contrib/makefile/downloads/protobuf" )


#add_library(tf_cc SHARED IMPORTED )
#set_target_properties(tf_cc PROPERTIES 
#    IMPORTED_LOCATION /usr/local/lib/libtensorflow_cc.so)

#add_library(tf_framework SHARED IMPORTED )
#set_target_properties(tf_framework PROPERTIES 
#    IMPORTED_LOCATION /usr/local/lib/libtensorflow_framework.so) 

target_compile_definitions(${PROJECT_NAME} PRIVATE -DENABLE_TF)
#target_link_libraries(${PROJECT_NAME} tf_cc tf_framework)
