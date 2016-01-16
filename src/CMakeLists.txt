set(EXECUTABLE_NAME Super-Bubba-Awesome-Space)


#########################################################
# ADD SOURCE FILES TO EXECUTABLE
#########################################################

add_executable(${EXECUTABLE_NAME} main.cpp ${files_SOURCE})

target_link_libraries(${EXECUTABLE_NAME} LINK_PUBLIC Bubba3D)

