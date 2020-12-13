cmake_minimum_required(VERSION 3.5)

project(habit_treker LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(Qt5_DIR "D:/Qt/5.14.2/msvc2017_64/lib/cmake/Qt5" CACHE PATH "Initial cahe" FORCE)

SET(CMAKE_INSTALL_PREFIX $<TARGET_FILE_DIR:${PROJECT_NAME}> CACHE PATH "Initial cahe" FORCE)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets Gui Core)

set(project_headers
mainwindow.h)

set (project_sources
main.cpp
mainwindow.cpp
mainwindow.ui)

qt5_wrap_cpp(project_sources_moc ${project_headers})

add_executable(${PROJECT_NAME} ${project_headers} ${project_sources} ${project_sources_moc})
target_link_libraries(${PROJECT_NAME} PUBLIC Qt5::Widgets)

install(FILES $<TARGET_FILE:Qt5::Widgets> DESTINATION $<TARGET_FILE_DIR:${PROJECT_NAME}>)
install(FILES $<TARGET_FILE:Qt5::Gui> DESTINATION $<TARGET_FILE_DIR:${PROJECT_NAME}>)
install(FILES $<TARGET_FILE:Qt5::Core> DESTINATION $<TARGET_FILE_DIR:${PROJECT_NAME}>)

install(DIRECTORY "D:/Qt/5.14.2/msvc2017_64/plugins" DESTINATION $<TARGET_FILE_DIR:${PROJECT_NAME}>)
