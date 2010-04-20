set(SOURCE_DIR "*.cpp")
set(INLINE_DIR "../../include/fx/*.inl")
set(HEADER_DIR "../../include/fx/*.hpp")

file(GLOB HEADER_FILE ${HEADER_DIR})
file(GLOB INLINE_FILE ${INLINE_DIR})
file(GLOB SOURCE_FILE ${SOURCE_DIR})

source_group("Inline Files" FILES ${INLINE_FILE})

add_library(${KUEKEN_FX_NAME} STATIC ${SOURCE_FILE} ${HEADER_FILE} ${INLINE_FILE})

add_parallel_build_GTC(4)
