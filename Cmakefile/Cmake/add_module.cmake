function(add_module MODULE_NAME)
    # Parse the arguments
    set(one_value_args MODULE_TYPE  VS_FOLDER)
    set(multi_value_args SOURCE_H_FILES SOURCE_CPP_FILES SOURCE_OTHERS_FILES PRIVATE_INCLUDES DEPENDENCIES PRECOMPILED_HEADER_FILE PREPROCESSOR_DEFINITIONS POSTBUILD_COPY IMPORT_SUFFIX MODULE_HELP)
    cmake_parse_arguments(ADD_MODULE "" "${one_value_args}" "${multi_value_args}" ${ARGN})

    # Set the module library name
    set(MODULE_LIB_NAME ${MODULE_NAME})

    # Set the module library output directory
    set(MODULE_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR})
	
	
	# Header precompiles
	set (SRC_FILES
         ${ADD_MODULE_SOURCE_CPP_FILES}
         ${ADD_MODULE_SOURCE_H_FILES}
    )
    
    if (ADD_MODULE_PRECOMPILED_HEADER_FILE)
        message(STATUS "ADD_MODULE_PRECOMPILED_HEADER_FILE '${ADD_MODULE_PRECOMPILED_HEADER_FILE}'")
		set ( SRC_FILES_FOR_PRECOMPILED_HEADER ${SRC_FILES})
        enable_precompiled_headers( ${MY_MODULE_PRECOMPILED_HEADER_FILE} SRC_FILES_FOR_PRECOMPILED_HEADER)
		set (SRC_FILES ${SRC_FILES_FOR_PRECOMPILED_HEADER})
        source_group(pch FILES ${MY_MODULE_PRECOMPILED_HEADER_FILE})

        set (SRC_FILES
            ${SRC_FILES}
            ${MY_MODULE_PRECOMPILED_HEADER_FILE}
        )
    endif()

    # Gather all source files
    set(MODULE_SOURCE_FILES "")
    list(APPEND MODULE_SOURCE_FILES ${ADD_MODULE_SOURCE_H_FILES})
    list(APPEND MODULE_SOURCE_FILES ${ADD_MODULE_SOURCE_CPP_FILES})

    # Build the module library
    if(NOT ADD_MODULE_MODULE_TYPE)
        message(FATAL_ERROR "ADD_MODULE_MODULE_TYPE not specified for module '${MODULE_NAME}'")
    endif()

    message(STATUS "SRC_FILES: " ${SRC_FILES})
    if ( "${ADD_MODULE_MODULE_TYPE}" STREQUAL "exe_win32")
        message(STATUS "exe added... SRC_FILES:'${SRC_FILES}'")
        add_executable(${MODULE_LIB_NAME} WIN32 ${SRC_FILES} )
    elseif ( ${ADD_MODULE_MODULE_TYPE} STREQUAL dll)
        message(STATUS "dll added... SRC_FILES:'${SRC_FILES}'")
        add_library(${MODULE_LIB_NAME} SHARED ${SRC_FILES} )
    elseif ( ${ADD_MODULE_MODULE_TYPE} STREQUAL static_lib)
        add_library(${MODULE_LIB_NAME} STATIC ${SRC_FILES} )        
    else ()
        message( FATAL_ERROR "Type de module inconnu")
    endif()


    # Option de precompilation
    if( ADD_MODULE_PREPROCESSOR_DEFINITIONS)
        message(STATUS "ADD_MODULE_PREPROCESSOR_DEFINITIONS: " ${ADD_MODULE_PREPROCESSOR_DEFINITIONS})
        FOREACH (preprocOpt ${ADD_MODULE_PREPROCESSOR_DEFINITIONS})
            #option du préprocesseur spécifiques a la target en cours
            #message(STATUS "preprocOpt ${MY_MODULE_PREPROCESSOR_DEFINITIONS}: " ${preprocOpt})
            target_compile_definitions( ${MODULE_LIB_NAME} PRIVATE -D${preprocOpt} )
        ENDFOREACH (preprocOpt)
    endif()

    # Set include directories for this module
    target_include_directories(${MODULE_LIB_NAME} PUBLIC "${CMAKE_CURRENT_LIST_DIR}")
    target_include_directories(${MODULE_LIB_NAME} PRIVATE ${ADD_MODULE_PRIVATE_INCLUDES})

    # Set library dependencies for this module
    target_link_libraries(${MODULE_LIB_NAME} PUBLIC ${ADD_MODULE_DEPENDENCIES})

    # Set the Visual Studio folder for this module
    if(ADD_MODULE_VS_FOLDER)
        set_target_properties(${MODULE_LIB_NAME} PROPERTIES FOLDER ${ADD_MODULE_VS_FOLDER})
		source_group(${ADD_MODULE_VS_FOLDER} FILES ${MODULE_LIB_NAME})
		message(STATUS "folder added...")
    endif()
    
    if( ADD_MODULE_SOURCE_OTHERS_FILES)
        set (SRC_FILES
             ${SRC_FILES}
             ${ADD_MODULE_SOURCE_OTHERS_FILES}
        )
    endif()
    
     # Copie postbuild
     message(STATUS "POSTBUILD_COPY: " ${ADD_MODULE_POSTBUILD_COPY})
    if( ADD_MODULE_POSTBUILD_COPY)
        FOREACH (filename ${ADD_MODULE_POSTBUILD_COPY})
            message(STATUS "filename: " ${filename})
            add_custom_command(TARGET ${MODULE_LIB_NAME} POST_BUILD 
                                 COMMAND  ${CMAKE_COMMAND} -E copy ${filename} "$<$<CONFIG:debug>:${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/>$<$<CONFIG:release>:${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/>"
                                 )
        ENDFOREACH (filename)
    endif()

    # Add this module's target to the list of all module targets
    list(APPEND ALL_MODULE_TARGETS ${MODULE_LIB_NAME})

    if(${ADD_MODULE_MODULE_HELP})
        message(STATUS  "MODULE_LIB_NAME:     ${MODULE_LIB_NAME}")
        message(STATUS  "MODULE_TYPE:         ${ADD_MODULE_MODULE_TYPE}")
        message(STATUS  "MODULE_SOURCE_FILES: ${MODULE_SOURCE_FILES}")
        message(STATUS  "VS_FOLDER:           ${ADD_MODULE_VS_FOLDER}")
        message(STATUS  "PRIVATE_INCLUDES:    ${ADD_MODULE_PRIVATE_INCLUDES}")
        message(STATUS  "DEPENDENCIES:        ${ADD_MODULE_DEPENDENCIES}")
		message(STATUS  "IMPORT_SUFFIX:       ${ADD_MODULE_IMPORT_SUFFIX}")
        message(STATUS  "MODULE_HELP:         ${ADD_MODULE_MODULE_HELP}")

        message(STATUS  "Current List Dir:    ${CMAKE_CURRENT_LIST_DIR}")
		
		message(STATUS  "Module out Dir:      ${MODULE_OUTPUT_DIR}")
		message(STATUS  "MODULE IMPORT LIB NAME:      ${MODULE_IMPORT_LIB_NAME}")
		message(STATUS  "CMAKE STATIC LIBRARY SUFFIX:      ${CMAKE_STATIC_LIBRARY_SUFFIX}")
		message(STATUS  "CMAKE SHARED LIBRARY SUFFIX:      ${CMAKE_SHARED_LIBRARY_SUFFIX}")
		message(STATUS  "LIBRARY OUTPUT DIRECTORY:      ${MODULE_OUTPUT_DIR}/${MODULE_NAME}.dir/Debug")
		
		message(STATUS  "currentDir/dependencies:       ${CMAKE_CURRENT_LIST_DIR}/../${ADD_MODULE_DEPENDENCIES}")
	
    endif()
endfunction()
