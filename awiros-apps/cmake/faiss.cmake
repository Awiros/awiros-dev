include_directories(${PROJECT_NAME} SYSTEM PUBLIC "/usr/local/include/faiss-new")

#add_library(faiss_lib SHARED IMPORTED)
#set_target_properties(faiss_lib PROPERTIES IMPORTED_LOCATION /usr/local/include/faiss/libs/libfaiss.so)

target_compile_definitions(${PROJECT_NAME} PRIVATE -DENABLE_FAISS)
#target_link_libraries(${PROJECT_NAME} faiss_lib)
