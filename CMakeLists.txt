cmake_minimum_required(VERSION 3.10)
project(SimpleBrowser)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set the SFML directory explicitly
set(SFML_DIR "/opt/homebrew/Cellar/sfml/2.6.1/lib/cmake/SFML")

# Find SFML
find_package(SFML 2.6.1 COMPONENTS graphics window system REQUIRED)

# Find CURL
find_package(CURL REQUIRED)

# Find Gumbo
find_path(GUMBO_INCLUDE_DIR gumbo.h)
find_library(GUMBO_LIBRARY gumbo)

# Add your source files
add_executable(SimpleBrowser
    main.cpp
    Browser.cpp
    NetworkManager.cpp
    HTMLParser.cpp
    Renderer.cpp
    CSSParser.cpp  # Add this line
)

# Link libraries
target_link_libraries(SimpleBrowser PRIVATE 
    sfml-graphics 
    sfml-window 
    sfml-system 
    CURL::libcurl
    ${GUMBO_LIBRARY}
)

# Include directories
target_include_directories(SimpleBrowser PRIVATE 
    ${SFML_INCLUDE_DIR}
    ${CURL_INCLUDE_DIRS}
    ${GUMBO_INCLUDE_DIR}
)

# Set architecture for Apple Silicon Macs
if(APPLE)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
endif()

# if(CMAKE_EXPORT_COMPILE_COMMANDS)
#   add_custom_target(
#     copy-compile-commands ALL
#     ${CMAKE_COMMAND} -E copy_if_different
#     ${CMAKE_BINARY_DIR}/compile_commands.json
#     ${CMAKE_SOURCE_DIR}/compile_commands.json
#   )
# endif()
