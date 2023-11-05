file(GLOB  LIB_H
    ${CMAKE_CURRENT_LIST_DIR}/*.h
)

file(GLOB  LIB_CPP
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

set(H_FILES ${LIB_H})

set(CPP_FILES ${LIB_CPP})


set(Boost_LIBRARY_DIR $ENV{BOOST_LIBRARYDIR})


add_module(webSocket
      MODULE_TYPE
         dll 
      SOURCE_H_FILES
         ${H_FILES}
      SOURCE_CPP_FILES
         ${CPP_FILES}
      DEPENDENCIES
		${Boost_LIBRARY_DIR}/libboost_thread-vc142-mt$<$<CONFIG:debug>:-gd>-x32-1_71.lib
		${Boost_LIBRARY_DIR}/libboost_chrono-vc142-mt$<$<CONFIG:debug>:-gd>-x32-1_71.lib
		${Boost_LIBRARY_DIR}/libboost_date_time-vc142-mt$<$<CONFIG:debug>:-gd>-x32-1_71.lib
		${Boost_LIBRARY_DIR}/libboost_regex-vc142-mt$<$<CONFIG:debug>:-gd>-x32-1_71.lib
	  MODULE_HELP
		FALSE
)