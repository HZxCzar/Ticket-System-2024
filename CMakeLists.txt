cmake_minimum_required(VERSION 3.10)

# 设置项目名称和版本
project(TicketSystem VERSION 1.0)

# 指定 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
enable_testing()
# 添加可执行文件
add_executable(code 
code.cpp 
structure/User.cpp 
structure/Train.cpp 
structure/Station.cpp 
structure/RemSeat.cpp 
structure/Queue.cpp 
structure/Order.cpp 
structure/Login.cpp 
structure/Box.cpp 
structure/Tools/TokenScanner.cpp 
structure/Stream/output.cpp)
