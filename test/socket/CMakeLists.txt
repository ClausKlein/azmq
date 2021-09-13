project(test_socket)

add_executable(${PROJECT_NAME} main.cpp)

add_catch_test(${PROJECT_NAME})
