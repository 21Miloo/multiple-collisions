cmake_minimum_required(VERSION 3.16)

project(multiple-collisions LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(multiple-collisions main.cpp
    box.h box.cpp
    particula.h particula.cpp
    obstaculo.h obstaculo.cpp
    simulador.h simulador.cpp)

include(GNUInstallDirs)
install(TARGETS multiple-collisions
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
