aux_source_directory(. SRC_LIST)
# 指定源目录文件名，如果新增文件需要在这里加入（新增文件但不重新cmake，会报链接错误）
set(SRC_LIST mprpcapplication.cc mprpcconfig.cc)
add_library(mprpc SHARED ${SRC_LIST})