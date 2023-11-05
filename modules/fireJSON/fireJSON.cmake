#if(BUILD_TEST_APPS)

   file(GLOB H_FILES ${CMAKE_CURRENT_LIST_DIR}/*.h)

   file(GLOB CPP_FILES ${CMAKE_CURRENT_LIST_DIR}/*.cpp)

   if(NOT DEFINED wxWidgets_ROOT_DIR)
      message(FATAL_ERROR "Please provide the wxWidgets 3.2.0 root dir by setting wxWidgets_ROOT_DIR (PATH type) variable")
      message(FATAL_ERROR "cf. https://www.wxwidgets.org/downloads/ used : wxWidgets-3.2.0-headers.7z wxMSW-3.2.0_vc14x_Dev.7z wxMSW-3.2.0_vc14x_ReleaseDLL.7z")
   endif()
   if(NOT EXISTS "${wxWidgets_ROOT_DIR}/include/wx/wx.h")
      message(FATAL_ERROR "wxWidgets 3.2.0 not found in ${wxWidgets_ROOT_DIR}. Check ${wxWidgets_ROOT_DIR} value")
   endif()
   message(STATUS "wxWidgets 3.2.0 is configured to be in ${wxWidgets_ROOT_DIR}")
   set(wxWidgets_CONFIGURATION mswu)

   set(moduleType "exe_win32")
  
   message(STATUS "wxWidgets_CONFIGURATION: " ${wxWidgets_CONFIGURATION})
  
   add_module(fireJSON
      MODULE_TYPE
         ${moduleType}
      SOURCE_H_FILES
         ${H_FILES}
      SOURCE_CPP_FILES
         ${CPP_FILES}
      SOURCE_OTHERS_FILES
         ${CMAKE_CURRENT_LIST_DIR}/AppFrame.fbp
      PRIVATE_INCLUDES
         ${wxWidgets_ROOT_DIR}/include
         ${wxWidgets_ROOT_DIR}/lib/vc14x_dll/${wxWidgets_CONFIGURATION}$<$<CONFIG:debug>:d>
      PREPROCESSOR_DEFINITIONS
         __WXMSW__
         __WX__
         WXUSINGDLL
         wxUSE_UNICODE
         wxNEEDS_DECL_BEFORE_TEMPLATE
      DEPENDENCIES
         ${wxWidgets_ROOT_DIR}/lib/vc14x_dll/wxbase32u$<$<CONFIG:debug>:d>.lib
         ${wxWidgets_ROOT_DIR}/lib/vc14x_dll/wxmsw32u$<$<CONFIG:debug>:d>_core.lib
		 webSocket
		 jsonParser
      POSTBUILD_COPY
         ${wxWidgets_ROOT_DIR}/lib/vc14x_dll/wxbase32u$<$<CONFIG:debug>:d>_vc14x.dll
         ${wxWidgets_ROOT_DIR}/lib/vc14x_dll/wxmsw32u$<$<CONFIG:debug>:d>_core_vc14x.dll
   )

#endif()