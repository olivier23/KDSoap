project(KDSoap CXX)
cmake_minimum_required(VERSION 2.8.7)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/ ${CMAKE_MODULE_PATH}")

option(${PROJECT_NAME}_STATIC "Build statically" OFF)
option(${PROJECT_NAME}_TESTS "Build the tests" OFF)

set(${PROJECT_NAME}_VERSION_MAJOR 1)
set(${PROJECT_NAME}_VERSION_MINOR 0)
set(${PROJECT_NAME}_VERSION_PATCH 0)
set(${PROJECT_NAME}_VERSION ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_PATCH})
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# setup default install locations
include(InstallLocation)

find_package(Qt5Widgets QUIET)
if (Qt5Widgets_FOUND)
  set(QT_LIBRARIES Qt5::Widgets)
  set(QT_USE_FILE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Qt5Portability.cmake")

  if(Qt5_POSITION_INDEPENDENT_CODE)
    if(CMAKE_VERSION VERSION_LESS 2.8.9)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
    elseif(CMAKE_VERSION VERSION_LESS 2.8.11)
      set(CMAKE_POSITION_INDEPENDENT_CODE ON)
    endif()
  endif()

else()
  find_package(Qt4 4.7 QUIET REQUIRED QtCore QtMain QtGui)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR TRUE)
set(CMAKE_AUTOMOC TRUE)

add_definitions(-DQT_NO_CAST_TO_ASCII -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_FROM_BYTEARRAY)
add_definitions(-DUSE_EXCEPTIONS -DQT_FATAL_ASSERT)

if(${PROJECT_NAME}_STATIC)
  set(${PROJECT_NAME}_LIBRARY_MODE "STATIC")
else()
  set(${PROJECT_NAME}_LIBRARY_MODE "SHARED")
endif()

if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
  set(${PROJECT_NAME}_IS_ROOT_PROJECT TRUE)

  if(CMAKE_BUILD_TYPE MATCHES "Release")
    add_definitions(-DNDEBUG)
  endif()

  if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "" FORCE)
  endif()

  if(UNIX AND CMAKE_INSTALL_PREFIX MATCHES "/usr/local")
    set(CMAKE_INSTALL_PREFIX "/usr/local/KDAB/${PROJECT_NAME}-${${PROJECT_NAME}_VERSION}")
  elseif(WIN32)
    set(CMAKE_INSTALL_PREFIX "C:\\KDAB\\${PROJECT_NAME}-$$VERSION")
  endif()

  message(STATUS "Building ${PROJECT_NAME} ${${PROJECT_NAME}_VERSION} in ${CMAKE_BUILD_TYPE} mode")

  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib")
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib")

  install(FILES LICENSE.GPL.txt LICENSE.US.txt LICENSE.txt README.txt kdsoap.pri kdwsdl2cpp.pri DESTINATION .)

  if (CMAKE_VERSION VERSION_LESS 2.8.8)
    include("${CMAKE_SOURCE_DIR}/cmake/CMakePackageConfigHelpers.cmake")
  else()
    include(CMakePackageConfigHelpers)
  endif()

  write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/KDSoap/KDSoapConfigVersion.cmake"
    VERSION ${${PROJECT_NAME}_VERSION}
    COMPATIBILITY AnyNewerVersion
  )

  configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KDSoapConfig-buildtree.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/KDSoap/KDSoapConfig.cmake"
    @ONLY
  )
  file(COPY
    "${CMAKE_CURRENT_SOURCE_DIR}/KDSoapMacros.cmake"
    DESTINATION
    "${CMAKE_CURRENT_BINARY_DIR}/KDSoap/"
  )
  configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KDSoapConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/install/KDSoapConfig.cmake"
    INSTALL_DESTINATION  "${INSTALL_LIBRARY_DIR}/cmake/KDSoap"
    PATH_VARS  INSTALL_INCLUDE_DIR
  )

  install(FILES
      "${CMAKE_CURRENT_BINARY_DIR}/install/KDSoapConfig.cmake"
      "${CMAKE_CURRENT_BINARY_DIR}/KDSoap/KDSoapConfigVersion.cmake"
      KDSoapMacros.cmake
    DESTINATION "${INSTALL_LIBRARY_DIR}/cmake/KDSoap"
  )
  install(EXPORT KDSoapTargets NAMESPACE KDSoap::
    DESTINATION "${INSTALL_LIBRARY_DIR}/cmake/KDSoap"
  )
endif()

add_subdirectory(src)
add_subdirectory(kdwsdl2cpp)
if(${PROJECT_NAME}_IS_ROOT_PROJECT)

  export(TARGETS kdsoap kdsoap-server kdwsdl2cpp NAMESPACE KDSoap::
    FILE "${CMAKE_CURRENT_BINARY_DIR}/KDSoap/KDSoapTargets.cmake"
  )

  add_subdirectory(features)
  if(${PROJECT_NAME}_TESTS)
    enable_testing()
    add_subdirectory(testtools)
    add_subdirectory(unittests)
  endif()
  add_subdirectory(examples)
endif()
