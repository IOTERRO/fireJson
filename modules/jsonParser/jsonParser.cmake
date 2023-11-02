file(GLOB  LIB_H
    ${CMAKE_CURRENT_LIST_DIR}/*.h
)

file(GLOB  LIB_CPP
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

set(H_FILES ${LIB_H})

set(CPP_FILES ${LIB_CPP})

set(EXTERNAL_LIBS ${CMAKE_CURRENT_LIST_DIR}/../../External)

# Set the source and binary directories
set(JSON_DEVELOP_SOURCE_DIR ${EXTERNAL_LIBS}/json-develop)
set(JSON_DEVELOP_BINARY_DIR ${CMAKE_BINARY_DIR}/json-develop)

# Add the json-develop subdirectory with the specified binary directory
add_subdirectory(${JSON_DEVELOP_SOURCE_DIR} ${JSON_DEVELOP_BINARY_DIR})

add_module(jsonParser
      MODULE_TYPE
         dll 
      SOURCE_H_FILES
         ${H_FILES}
      SOURCE_CPP_FILES
         ${CPP_FILES}
      VS_FOLDER
         
      PRIVATE_INCLUDES
         
      DEPENDENCIES
		nlohmann_json
	  IMPORT_SUFFIX
		
	  MODULE_HELP
		FALSE
)