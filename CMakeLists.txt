cmake_minimum_required(VERSION 3.10)
project(restaurant)

set(CMAKE_CXX_STANDARD 11)

# 添加 include 目录
include_directories(/opt/homebrew/include)

# 添加库目录
link_directories(/opt/homebrew/lib)

# 添加源文件
add_executable(costomer costomer.cpp)
add_executable(waiter waiter.cpp)
add_executable(administrator administrator.cpp)
add_executable(cooker cooker.cpp)

# 链接 MySQL 库
target_link_libraries(costomer mysqlclient)
target_link_libraries(waiter mysqlclient)
target_link_libraries(administrator mysqlclient)
target_link_libraries(cooker mysqlclient)
