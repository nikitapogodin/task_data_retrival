
set(sources
        server_main.cpp
        data_source.cpp
        data_cache.cpp
        data_server.cpp
)

# Add executable
add_executable(server ${sources})

# Set any additional compilation options if needed
target_compile_options(server PRIVATE -Wall -Wextra -Werror)
