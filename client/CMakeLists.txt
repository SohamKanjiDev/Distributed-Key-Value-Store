add_executable(Client "client.cpp")
target_link_libraries(Client PUBLIC
    TCPServer
    ${Boost_LIBRARIES}
)