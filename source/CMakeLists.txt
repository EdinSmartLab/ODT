cmake_minimum_required(VERSION 3.10)

project(test LANGUAGES CXX C)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)      # Release or Debug
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../run)

add_executable(odt.x "")

#################### preprocessor directives

if(PARALLEL STREQUAL "yes")
    message("MPI is on")
    add_definitions(-DDOMPI)
endif()

if(NOT CHEMISTRY STREQUAL "CANTERARR" )
    add_definitions(-DPROBLEMSPECIFICRR)
endif()

if(SILENT STREQUAL "yes" )
    message("Suppressing some runtime output")
    add_definitions(-DSILENT)
endif()

add_definitions(-D_USESTDVECTOR_)

#################### Compile options

target_compile_features(odt.x PUBLIC cxx_std_11)

set(CMAKE_CXX_FLAGS_DEBUG   "-ggdb3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
#set(CMAKE_CXX_FLAGS        "-Wall -Wextra")

#################### Link: libraries and flags

target_include_directories(odt.x PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/yaml/include)
find_library(YAML yaml-cpp HINTS ${CMAKE_CURRENT_SOURCE_DIR}/yaml/lib)
target_link_libraries(odt.x ${YAML})
#target_link_libraries(odt.x ${CMAKE_CURRENT_SOURCE_DIR}/yaml/lib/libyaml-cpp.a)

target_include_directories(odt.x PRIVATE ${CANTERA_INCLUDE_DIR}) 
find_library(CANTERA cantera HINTS ${CANTERA_LIB_DIR})
target_link_libraries(odt.x ${CANTERA})

find_package(Boost REQUIRED COMPONENTS system)
target_include_directories(odt.x PRIVATE ${Boost_INCLUDE_DIRS})

find_library(LAPACK lapack)
target_link_libraries(odt.x ${LAPACK})

find_library(BLAS blas)
target_link_libraries(odt.x ${BLAS})

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    target_link_libraries(odt.x "-framework Accelerate")
endif()

target_link_libraries(odt.x "pthread")

#################### Local source files

target_include_directories(odt.x PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(odt.x
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/inputoutput.cc      ${CMAKE_CURRENT_SOURCE_DIR}/inputoutput.h
        ${CMAKE_CURRENT_SOURCE_DIR}/domain.cc           ${CMAKE_CURRENT_SOURCE_DIR}/domain.h
        ${CMAKE_CURRENT_SOURCE_DIR}/processor.cc        ${CMAKE_CURRENT_SOURCE_DIR}/processor.h
        ${CMAKE_CURRENT_SOURCE_DIR}/streams.cc          ${CMAKE_CURRENT_SOURCE_DIR}/streams.h
        ${CMAKE_CURRENT_SOURCE_DIR}/meshManager.cc      ${CMAKE_CURRENT_SOURCE_DIR}/meshManager.h
        ${CMAKE_CURRENT_SOURCE_DIR}/param.cc            ${CMAKE_CURRENT_SOURCE_DIR}/param.h
        ${CMAKE_CURRENT_SOURCE_DIR}/micromixer.cc       ${CMAKE_CURRENT_SOURCE_DIR}/micromixer.h
        ${CMAKE_CURRENT_SOURCE_DIR}/eddy.cc             ${CMAKE_CURRENT_SOURCE_DIR}/eddy.h
        ${CMAKE_CURRENT_SOURCE_DIR}/solver.cc           ${CMAKE_CURRENT_SOURCE_DIR}/solver.h
        ${CMAKE_CURRENT_SOURCE_DIR}/cvodeDriver.cc      ${CMAKE_CURRENT_SOURCE_DIR}/cvodeDriver.h
                                                        ${CMAKE_CURRENT_SOURCE_DIR}/interp_linear.h
)

#################### Subdirectories

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/cvode)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/domainvariables)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/radiation)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/domaincases)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/user_chemical_mechanisms)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/yaml)

#################### Doxygen documentation

find_package(Doxygen)
if(DOXYGEN_FOUND)
    add_custom_target(doxygen 
        COMMAND rm -rf ../docs/doxygen/html
        COMMAND mkdir  ../docs/doxygen/html
        COMMAND doxygen -s ../docs/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Building doxygen documentation"
        VERBATIM
    )
else()
    message("Cannot build documentation because doxygen was not found")
endif()

