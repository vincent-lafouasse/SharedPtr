project(tests)

add_executable(tests)
add_test(NAME tests
         COMMAND tests
)

target_sources(tests
PRIVATE
    ./main.cpp
    ${CMAKE_SOURCE_DIR}/SharedPtr.hpp
)

target_include_directories(tests
PUBLIC
    ${CMAKE_SOURCE_DIR}
)
