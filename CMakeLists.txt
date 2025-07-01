cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME building-vocabulary)
project(${PROJECT_NAME} CXX)

if (NOT CMAKE_BUILD_TYPE) 
  set(CMAKE_BUILD_TYPE Release) 
endif() 

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wno-sign-compare)

include_directories(include)

add_executable(building-vocabulary "${PROJECT_SOURCE_DIR}/src/tokenizer_example.cpp")
target_compile_options(building-vocabulary PRIVATE -Wall -Wextra -pedantic)

add_executable(playground "${PROJECT_SOURCE_DIR}/src/playground.cpp")
target_compile_options(playground PRIVATE -Wall -Wextra -pedantic)

add_executable(huge_data "${PROJECT_SOURCE_DIR}/src/huge_data.cpp")
target_compile_options(huge_data PRIVATE -Wall -Wextra -pedantic)

