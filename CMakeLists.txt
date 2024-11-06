cmake_minimum_required(VERSION 3.29)
project(SmartObject)

set(CMAKE_CXX_STANDARD 20)

find_package(PalSigslot CONFIG REQUIRED)

add_executable(SmartObject main.cpp
        SmartObject/SmartObject.cpp
        SmartObject/SmartObject.h
        SmartObject/SmartPointer.h
        SmartObject/SmartDefine.h
        TypeWrapper/TypeWrapper.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE Pal::Sigslot)
