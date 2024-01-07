cmake_minimum_required(VERSION 3.20.0 FATAL_ERROR)

include_directories(C:/msys64/mingw64/include)
link_directories(C:/msys64/mingw64/lib)

#Функция прерывания 
function(exit_cmake)
  message("Прерывание настройки")
  if(UNIX)
    set(KILL_COMMAND "killall")
    execute_process(COMMAND ${KILL_COMMAND} -9 cmake
      WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    )
  else()
    set(KILL_COMMAND "taskkill")
    execute_process(COMMAND ${KILL_COMMAND} /IM cmake.exe /F
      WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    )
  endif()
endfunction(exit_cmake)

#Выставление версионности проекта
set(PATCH_VERSION "2" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(helloworld VERSION ${PROJECT_VESRION})

#Подключение тестов 
include(CTest)
find_package(GTest REQUIRED)
if(GTest_FOUND)
  message("Тесты найдены")
  include(GoogleTest)
  #include_directories(${GTEST_INCLUDE_DIR})
endif()

enable_testing()

#Поиск BOOST 
cmake_policy(SET CMP0074 NEW)
set(Boost_DEBUG ON)
find_package(Boost REQUIRED)

#Файл конфигурации и версионности

configure_file(version.h.in version.h)

#Создание исполняемого файла и линковка с библиотеками
if(Boost_FOUND)
    add_library(helloworld lib.cpp)
    target_include_directories(helloworld
    PRIVATE "${CMAKE_BINARY_DIR}"
    )
    include_directories( ${Boost_INCLUDE_DIRS})
    add_executable(Test_ci_cd main.cpp)
    # Подключение библы для поддержки версионности
    target_link_libraries(Test_ci_cd PRIVATE
    helloworld  ${Boost_LIBRARIES}
    )
    set_target_properties(Test_ci_cd helloworld  PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    )

    message("Исполняемый файл создан")

    # if(GTest_FOUND)
    #   message("GTEST найден")
    #   set(FooTestArgs --foo 1 --bar 2)
    #   add_executable(test_version test_version.cpp)
    #   target_link_libraries(test_version
    #   ${GTEST_LIBRARY} helloworld)
    #   set_target_properties(test_version PROPERTIES CXX_STANDARD 20 
    #   CXX_STANDARD_REQUIRED ON)
    # endif()

else()
  exit_cmake()
endif()


if(MSVC)
    target_compile_options(Test_ci_cd PRIVATE /W4)
else()
    target_compile_options(Test_ci_cd PRIVATE
    -Wall -Wextra -pedantic
    )
endif()

#set(CPACK_GENERATOR DEB) Позволяет сделать дебку 
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT yachkuch@gmail.com)

include(CPack)

install(TARGETS Test_ci_cd RUNTIME DESTINATION bin)

#Добавление теста
#add_test(test_version test_version)
#gtest_add_tests(test_version "${FooTestArgs}" AUTO)