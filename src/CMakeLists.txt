aux_source_directory(. SRC_LIST)
# 指定源目录文件名，如果新增文件需要在这里加入（新增文件但不重新cmake，会报链接错误）
set(SRC_LIST mprpcapplication.cc mprpcconfig.cc rpcheader.pb.cc rpcprovider.cc )
add_library(mprpc ${SRC_LIST})
target_link_libraries(mprpc muduo_net muduo_base pthread)