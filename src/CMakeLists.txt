# Project info
cmake_minimum_required(VERSION 3.16)
project(RemoteRender)

# Add subprojects
add_subdirectory(client)
add_subdirectory(daemon)

# Target runs daemon
add_custom_target(run_daemon
    COMMAND $<TARGET_FILE:appdaemon>
    DEPENDS appdaemon
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

# Target runs client
add_custom_target(run_client
    COMMAND $<TARGET_FILE:appclient>
    DEPENDS appclient
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

# Target runs both
add_custom_target(run_all
    DEPENDS run_daemon run_client
)
