cmake_minimum_required(VERSION 3.10)

project(FireSafetyApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Добавляем все исходники
set(SOURCES
    main.cpp
    business/Entities.cpp
    business/FireSafetyService.cpp
    data/Repository.cpp
    presentation/ConsoleUI.cpp
)

add_executable(fire_safety_app ${SOURCES})

# Включаем каталоги с заголовками
target_include_directories(fire_safety_app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/business
    ${CMAKE_CURRENT_SOURCE_DIR}/data
    ${CMAKE_CURRENT_SOURCE_DIR}/presentation
)
