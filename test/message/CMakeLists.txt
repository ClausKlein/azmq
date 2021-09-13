project(test_message)

add_executable(${PROJECT_NAME} main.cpp)

add_catch_test(${PROJECT_NAME})
