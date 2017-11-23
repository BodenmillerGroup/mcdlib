set(PYBIND11_CPP_STANDARD -std=c++11)

add_subdirectory(pybind11)
find_package(PythonInterp REQUIRED)
find_package(PythonLibs REQUIRED)

pybind11_add_module(mcdpy PythonModule.cpp)
target_link_libraries(mcdpy PUBLIC mcd ${PYTHON_LIBRARIES})
target_include_directories(mcdpy PUBLIC ../src ${PYTHON_INCLUDE_DIRS})

if (NOT DEFINED PYTHON_SITE)
    execute_process(COMMAND ${PYTHON_EXECUTABLE} -c "from distutils.sysconfig import get_python_lib; print(get_python_lib(plat_specific=True))" OUTPUT_VARIABLE PYTHON_SITE OUTPUT_STRIP_TRAILING_WHITESPACE)
endif ()
install(TARGETS mcdpy DESTINATION ${PYTHON_SITE})