cmake_minimum_required(VERSION 3.2)

if($ENV{TRAVIS_BUILD_NUMBER})
 project(13_homework VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
 project(13_homework VERSION 0.0.3)
endif()

#SET(CMAKE_CXX_COMPILER /usr/local/Cellar/gcc/HEAD-f2787da/bin/c++-HEAD)

find_package(Threads)
find_package(Boost COMPONENTS system regex REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

add_executable(join_server main.cpp parser.cpp database.cpp server.cpp session.cpp sqlite3.c utils.cpp)

set_target_properties(join_server PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON)

target_include_directories(join_server PRIVATE "${CMAKE_BINARY_DIR}")
target_link_libraries(join_server PRIVATE ${CMAKE_THREAD_LIBS_INIT}  ${Boost_LIBRARIES})

#target_compile_options(join_server PRIVATE -Wall -Wextra -pedantic -Werror )

install(TARGETS join_server RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT ddzuba@yandex.ru)
include(CPack)

