cmake_minimum_required(VERSION 3.9)
project(es-client-api-freertos-docs)

set(SPHINX_EXECUTABLE sphinx-build)
set(SPHINX_SOURCE ${CMAKE_CURRENT_SOURCE_DIR})
set(SPHINX_BUILD ${CMAKE_CURRENT_BINARY_DIR}/html)
set(SPHINX_INDEX_FILE ${SPHINX_BUILD}/index.html)

file(GLOB DOCS_FILES
    "${SPHINX_SOURCE}/*.md"
    "${SPHINX_SOURCE}/api/*.rst"
    "${SPHINX_SOURCE}/api/commands/*.md"
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/_static DESTINATION ${SPHINX_SOURCE}/)

add_custom_target(docs ALL DEPENDS ${SPHINX_INDEX_FILE})
add_custom_command(OUTPUT ${SPHINX_INDEX_FILE}
    COMMAND
        ${SPHINX_EXECUTABLE} -E -b html ${SPHINX_SOURCE} ${SPHINX_BUILD} ${DOCS_FILES}
    MAIN_DEPENDENCY conf.py
    COMMENT "Building DOCS for lib ES Client-API FreeRTOS")
