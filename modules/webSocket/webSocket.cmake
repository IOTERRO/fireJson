file(GLOB  LIB_H
    ${CMAKE_CURRENT_LIST_DIR}/*.h
)

file(GLOB  LIB_CPP
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /D_ITERATOR_DEBUG_LEVEL=0")
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
#add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)


set(H_FILES ${LIB_H})

set(CPP_FILES ${LIB_CPP})

set(Boost_LIBRARY_DIR $ENV{BOOST_LIBRARYDIR})

file(GLOB  BOOST_LIBS_RELEASE 
	${Boost_LIBRARY_DIR}/libboost_thread-vc142-mt-x32-1_71.lib
	${Boost_LIBRARY_DIR}/libboost_chrono-vc142-mt-x32-1_71.lib
	${Boost_LIBRARY_DIR}/libboost_date_time-vc142-mt-x32-1_71.lib
	${Boost_LIBRARY_DIR}/libboost_regex-vc142-mt-x32-1_71.lib
)

file(GLOB  BOOST_LIBS_DEBUG
    ${Boost_LIBRARY_DIR}/libboost_date_time-vc142-mt-gd-x32-1_71.lib
	${Boost_LIBRARY_DIR}/libboost_regex-vc142-mt-gd-x32-1_71.lib
	${Boost_LIBRARY_DIR}/libboost_thread-vc142-mt-gd-x32-1_71.lib
	${Boost_LIBRARY_DIR}/libboost_chrono-vc142-mt-gd-x32-1_71.lib
)

message(STATUS "boost lib: " ${BOOST_LIBS_DEBUG})

add_module(webSocket
      MODULE_TYPE
         dll 
      SOURCE_H_FILES
         ${H_FILES}
      SOURCE_CPP_FILES
         ${CPP_FILES}
      DEPENDENCIES
		${BOOST_LIBS_RELEASE}
	  MODULE_HELP
		FALSE
)