set(SOURCE_DIR "*.cpp")
set(INLINE_DIR "../../include/core/*.inl")
set(HEADER_DIR "../../include/core/*.hpp")

file(GLOB HEADER_FILE ${HEADER_DIR})
file(GLOB INLINE_FILE ${INLINE_DIR})
file(GLOB SOURCE_FILE ${SOURCE_DIR})

source_group("Inline Files" FILES ${INLINE_FILE})

add_library(${KUEKEN_CORE_NAME} STATIC ${SOURCE_FILE} ${HEADER_FILE} ${INLINE_FILE})

#add_parallel_build_GTC(4)
