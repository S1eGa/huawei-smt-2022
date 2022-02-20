cmake_minimum_required(VERSION 3.10)

project(FinOrdSolver)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

add_compile_options(-Wall -Wextra -Werror -O2 -fsanitize=address)
add_link_options(-fsanitize=address)

include_directories(include)
add_executable(solver src/FinOrdSolver.cpp src/main.cpp)
target_link_libraries(solver cvc5)