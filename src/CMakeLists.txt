aux_source_directory(. SRC_LIST)
aux_source_directory(./frame FRAME_LIST)
# set(SRC_LIST main.cpp graph.cpp node.cpp)

add_executable(programer ${SRC_LIST} ${FRAME_LIST})
