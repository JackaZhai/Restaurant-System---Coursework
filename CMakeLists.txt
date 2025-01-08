cmake_minimum_required(VERSION 3.10)
project(restaurant)

set(CMAKE_CXX_STANDARD 17) # 设置 C++ 标准

# 添加 include_MySQL 目录
include_directories(C:/Users/JackZhai/CLionProjects/Restaurant-System-Coursework/include/include_MySQL)
include_directories(C:/Users/JackZhai/CLionProjects/Restaurant-System-Coursework/include)

# 添加库目录
link_directories(C:/Users/JackZhai/CLionProjects/Restaurant-System-Coursework/lib)

# 添加源文件
add_executable(main src/main.cpp src/customer.cpp src/waiter.cpp src/administrator.cpp src/cooker.cpp src/cashier.cpp src/boss.cpp)

# 链接 MySQL 库
target_link_libraries(main libmysql)