add_executable("${ProjectName}_Test"
        #src files
        src/main.cpp
        
        #header files
)

target_include_directories("${ProjectName}_Test" PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries("${ProjectName}_Test" PUBLIC "${ProjectName}")