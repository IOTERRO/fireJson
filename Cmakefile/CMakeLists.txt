cmake_minimum_required(VERSION 3.10)

project(Main)

set(CMAKE_CXX_STANDARD 11)


set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /D_ITERATOR_DEBUG_LEVEL=0")


#Dossier de sortie des binaires générés
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_BINARY_DIR}/Debug/)
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BINARY_DIR}/Release/)

# Set MYLIB_EXPORTS macro to TRUE
add_compile_definitions(FIRST_APP_API_EXPORTS=TRUE
						PLAYERS_API_EXPORTS=TRUE
						GLEW_STATIC=TRUE)              
                        
# Include add_module.cmake file
include(Cmake/add_module.cmake)


#include(${CMAKE_CURRENT_SOURCE_DIR}/json-develop/CMakeLists.txt)

message(STATUS ${MODULE_CMAKES})

set(EXTERNAL_LIBS ${CMAKE_CURRENT_SOURCE_DIR}/../External)

# Include all module CMake files
file(GLOB_RECURSE MODULE_CMAKES ${CMAKE_CURRENT_SOURCE_DIR}/../modules/**/*.cmake)
foreach(MODULE_CMAKE ${MODULE_CMAKES})
	if ("${MODULE_CMAKE}" MATCHES ".*json-develop.*")
		# Do something if json-develop is in the path
	else()
		# Do something if json-develop is not in the path
		include(${MODULE_CMAKE})
		message(STATUS "cmake modules name: '${MODULE_CMAKE}'")
		# Add this module's target to the list of all module targets
		get_filename_component(MODULE_NAME ${MODULE_CMAKE} NAME_WE)

        # Vérifiez le type du module
        get_target_property(MODULE_TYPE ${MODULE_NAME} TYPE)
        
        # Vérifiez si le type n'est pas parmi les types supportés
        message(STATUS "MODULE_NAME: '${MODULE_NAME}'")
        message(STATUS "MODULE_TYPE: '${MODULE_TYPE}'")
        if(NOT MODULE_TYPE STREQUAL "INTERFACE" AND NOT MODULE_TYPE STREQUAL "OBJECT" AND NOT MODULE_TYPE STREQUAL "STATIC_LIBRARY" AND NOT MODULE_TYPE STREQUAL "SHARED_LIBRARY")
            message(STATUS "n'est pas parmi les types supportés: '${MODULE_CMAKE}'")
            list(APPEND EXTRA_MODULES ${MODULE_NAME})
        else()
            list(APPEND ALL_MODULE_TARGETS ${MODULE_NAME})
        endif()

		#Set output directories for static and shared libraries
		set_target_properties(${MODULE_NAME}
		PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/lib/debug
		ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/lib/release
		LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/debug
		LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/release
		)
		
		source_group("modules" FILES ${MODULE_NAME})
	endif()
	
endforeach()


#include boost librairy
cmake_policy(SET CMP0074 NEW)

if(DEFINED ENV{BOOST_INCLUDEDIR})
  set(Boost_INCLUDE_DIR $ENV{BOOST_INCLUDEDIR})
else()
  MESSAGE("BOOST INCLUDE DIR NOT FOUND: please make sure that u correctly added the 'BOOST_INCLUDEDIR' to your system environment variable")
endif()

if(DEFINED ENV{BOOST_LIBRARYDIR})
  set(Boost_LIBRARY_DIR $ENV{BOOST_LIBRARYDIR})
else()
  MESSAGE("BOOST LIBRARIES DIR NOT FOUND: please make sure that u correctly added the 'BOOST_LIBRARYDIR' to your system environment variable")
endif()

#find_package(Boost REQUIRED)
FIND_PACKAGE(Boost REQUIRED)

string(REPLACE "\\" "/" Boost_LIBRARY_DIR "${Boost_LIBRARY_DIR}")
string(REPLACE "\\" "/" Boost_INCLUDE_DIR "${Boost_INCLUDE_DIR}")

IF (Boost_FOUND)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
    ADD_DEFINITIONS(-DHAS_BOOST)
else()
	MESSAGE ("BOOST NOT FOUND: please make sure that u are correctly installed the boost libs (to install boost library go to: https://www.boost.org/users/download/)")
ENDIF()


# Find all source files (cpp) and header files (h) in the current directory
file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../app/*.cpp")
file(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../app/*.h")

# Add the source files and header files to the executable target
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})



#add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/../app/main.cpp)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/lib/debug
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/lib/release
    LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/debug
    LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/release
	)

# Add the path to the directory containing the necessary .lib files
link_directories(${CMAKE_BINARY_DIR}/lib)

message(STATUS "ALL_MODULE_TARGETS: '${ALL_MODULE_TARGETS}'")
#link all project libs
#target_link_libraries(${PROJECT_NAME} ${ALL_MODULE_TARGETS})
target_link_libraries(${PROJECT_NAME} ${ALL_MODULE_TARGETS})
