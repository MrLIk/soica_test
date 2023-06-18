cmake_minimum_required(VERSION 3.15)

# Add *.h to be visible in QtCreator project tree
file(GLOB SRC "src/*.cpp" "include/*.h")

find_package(OpenCV COMPONENTS core dnn highgui imgproc REQUIRED)

add_library(backend STATIC ${SRC})

target_link_libraries(backend PUBLIC ${OpenCV_LIBS})
target_include_directories(backend PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${OpenCV_INCLUDE_DIRS})
