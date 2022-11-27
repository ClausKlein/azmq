add_subdirectory(actor)

add_executable(PubSub PubSub.cpp)
target_link_libraries(PubSub Azmq::azmq)

add_executable(wu_server wu_server.cpp)
target_link_libraries(wu_server Azmq::azmq)

add_executable(wu_client wu_client.cpp)
target_link_libraries(wu_client Azmq::azmq)
