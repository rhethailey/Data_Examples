# Set the minimum version of CMake required
cmake_minimum_required(VERSION 3.10)

# Set the project name
project(TCP_HANDSHAKE)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# include_directories(${CMAKE_SOURCE_DIR}/udpManager src) 

# add_library(udpManager STATIC
#     udpManager/udpManager.cpp
#     # udpManager/udpManager.h
# )

# Collect all .cpp files in directories
# file(GLOB SOURCES_GET "get/*.cpp")
file(GLOB SOURCES_SEND "handshake/*.cpp")

# Set the output directory for executables
# set(EXECUTABLE_OUTPUT_PATH_GET ${CMAKE_BINARY_DIR}/get_exe)
set(EXECUTABLE_OUTPUT_PATH_SEND ${CMAKE_BINARY_DIR}/send_exe)

# Loop through each source file in folder A and create an executable for each
# foreach(source_file ${SOURCES_GET})
#     # Extract the base filename (without path and extension)
#     get_filename_component(executable_name ${source_file} NAME_WE)

#     # Create an executable from the source file
#     add_executable(${executable_name} ${source_file})

#     # Set the output directory for this executable (folder A)
#     set_target_properties(${executable_name} PROPERTIES
#         RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH_GET}
#     )

#     # Link any necessary libraries (if needed)
#     target_link_libraries(${executable_name} udpManager pthread)  # Add dependencies if necessary
# endforeach()

# Loop through each source file in folder B and create an executable for each
foreach(source_file ${SOURCES_SEND})
    # Extract the base filename (without path and extension)
    get_filename_component(executable_name ${source_file} NAME_WE)

    # Create an executable from the source file
    add_executable(${executable_name} ${source_file})

    # Set the output directory for this executable (folder B)
    set_target_properties(${executable_name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH_SEND}
    )

    # Link any necessary libraries (if needed)
    # target_link_libraries(${executable_name} udpManager pthread)  # Add dependencies if necessary
endforeach()



# # Set the output directory for executables
# set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/${PARENT_DIR_NAME})

# # Loop through each source file and create an executable for each
# foreach(source_file ${SOURCES})
#     # Extract the base filename (without path and extension)
#     get_filename_component(executable_name ${source_file} NAME_WE)

#     # Create an executable from the source file
#     add_executable(${executable_name} ${source_file})

#     # Link any necessary libraries (if needed)
#     target_link_libraries(${executable_name} PRIVATE udpManager pthread)  # Add dependencies if necessary
# endforeach()