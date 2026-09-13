find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab6
    src/main.cpp
    src/CameraProvider.cpp
    src/KeyProcessor.cpp
    src/FrameProcessor.cpp
    src/Display.cpp
)

target_link_libraries(lab6 ${OpenCV_LIBS})