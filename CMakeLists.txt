cmake_minimum_required(VERSION 3.25)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})
project(fashion_mnist VERSION ${PROJECT_VESRION})

include(FetchContent)
if (WIN32)
  FetchContent_Declare(tensorflow URL "https://storage.googleapis.com/tensorflow/libtensorflow/libtensorflow-cpu-windows-x86_64-2.7.0.zip")
else ()
  FetchContent_Declare(tensorflow URL "https://storage.googleapis.com/tensorflow/libtensorflow/libtensorflow-cpu-linux-x86_64-2.7.0.tar.gz")
endif ()
FetchContent_MakeAvailable(tensorflow)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.13.0
)

FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
  FetchContent_Populate(googletest)
  add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()

if (WIN32)
  include_directories(${tensorflow_SOURCE_DIR}/include/)
  link_directories(${tensorflow_SOURCE_DIR}/lib/)
else ()
  include_directories(${tensorflow_SOURCE_DIR}/./include/)
  link_directories(${tensorflow_SOURCE_DIR}/./lib/)
endif ()

find_package(Threads REQUIRED)

add_executable(
  fashion_mnist
  fashion_mnist.cpp
  TFClassifier.cpp)

add_executable(
  test_fashion_mnist_gtest
  test_fashion_mnist_gtest.cpp
  TFClassifier.cpp)

set_target_properties(
  fashion_mnist
  test_fashion_mnist_gtest
  PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
)

target_link_libraries(
  fashion_mnist
  tensorflow)

target_link_libraries(
  test_fashion_mnist_gtest
  GTest::gtest_main
  Threads::Threads
  tensorflow)

if (WIN32)
  add_custom_command(
    TARGET fashion_mnist POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${tensorflow_SOURCE_DIR}/lib/tensorflow.dll              
    $<TARGET_FILE_DIR:fashion_mnist>)

  add_custom_command(
    TARGET test_fashion_mnist_gtest POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${tensorflow_SOURCE_DIR}/lib/tensorflow.dll              
    $<TARGET_FILE_DIR:test_fashion_mnist_gtest>)
else()
  add_custom_command(
    TARGET fashion_mnist POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${tensorflow_SOURCE_DIR}/./lib/libtensorflow.so.2.7.0             
    $<TARGET_FILE_DIR:fashion_mnist>)
  
  add_custom_command(
    TARGET fashion_mnist POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${tensorflow_SOURCE_DIR}/./lib/libtensorflow_framework.so.2.7.0           
    $<TARGET_FILE_DIR:fashion_mnist>)

  add_custom_command(
    TARGET test_fashion_mnist_gtest POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${tensorflow_SOURCE_DIR}/./lib/libtensorflow.so.2.7.0             
    $<TARGET_FILE_DIR:test_fashion_mnist_gtest>)

  add_custom_command(
    TARGET test_fashion_mnist_gtest POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${tensorflow_SOURCE_DIR}/./lib/libtensorflow_framework.so.2.7.0              
    $<TARGET_FILE_DIR:test_fashion_mnist_gtest>)
endif()

add_custom_command(
  TARGET test_fashion_mnist_gtest POST_BUILD 
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_SOURCE_DIR}/tests           
  $<TARGET_FILE_DIR:test_fashion_mnist_gtest>/tests)

install (
  TARGETS 
  fashion_mnist 
  RUNTIME 
  DESTINATION 
  bin
)

# CPack area
if (WIN32)
    set(CPACK_GENERATOR "ZIP")
else ()
    set(CPACK_GENERATOR "DEB")
endif()
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT https://github.com/4l3x777/homework13)
include(CPack)

# Tests area
enable_testing()

add_test(
    NAME 
    unit_tests_test_fashion_mnist_gtest 
    COMMAND 
    test_fashion_mnist_gtest
)

