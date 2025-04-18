add_executable("${ProjectName}-demo01"
        #src files
        src/main.cpp
        
        #header files
)

target_include_directories("${ProjectName}-demo01" PUBLIC 
    "${CMAKE_SOURCE_DIR}/dependencies"
    "${PROJECT_SOURCE_DIR}/include"
)
target_link_libraries("${ProjectName}-demo01" PUBLIC "${ProjectName}")