# SPDX-License-Identifier: Unlicense
file(GLOB_RECURSE SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

target_sources(${PROJECT_NAME}
    PRIVATE
        ${SOURCES}
)

