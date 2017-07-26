set(GSL_ROOT ${MAINFOLDER}/thirdparty/GSL)
set(GSL_INCLUDE_DIR ${GSL_ROOT}/include)
set(GSL_LIBRARIES GSL)
include_directories(${GSL_INCLUDE_DIR})
include_directories(${MAINFOLDER})

aux_source_directory(. DIR_LIB_SRCS)
aux_source_directory(ioutil DIR_LIB_SRCS)
aux_source_directory(os DIR_LIB_SRCS)
aux_source_directory(path DIR_LIB_SRCS)
aux_source_directory(path/filepath DIR_LIB_SRCS)

add_library (molly ${DIR_LIB_SRCS})