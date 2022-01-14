
cmake_minimum_required (VERSION 3.7)

project (roket_launcher VERSION 0.20 LANGUAGES CXX)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

if (UNIX)
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wconversion -Wextra")
endif()

set(Boost_USE_STATIC_LIBS ON)

if (WIN32)
	set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "C:/Program Files/Fort SDK/lib/win32-msvc2017/boost")
	set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "C:/Program Files/Fort SDK/lib/win32-msvc2017/boost/lib")
endif()

set (CMAKE_VERBOSE_MAKEFILE on)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/exe)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs)

set (h_dir "${CMAKE_CURRENT_LIST_DIR}/program/headers")
set (s_dir "${CMAKE_CURRENT_LIST_DIR}/program/sources")

set (conf_dir "${CMAKE_CURRENT_LIST_DIR}/program/config")

find_package(Boost 1.40 COMPONENTS log)

option (PRINT_USE_GEO "print geo points" ON)


if (Boost_FOUND)
	option (USE_BOOST_LOG "use boost log" ON)
	message ("boost log ${USE_BOOST_LOG}")

	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread -lboost_system")
	add_definitions(-DBOOST_ERROR_CODE_HEADER_ONLY)
	#ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)
endif()

file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/log")
set (LOG_DIR "${CMAKE_CURRENT_BINARY_DIR}/log")

configure_file(${conf_dir}/config.h.in ${conf_dir}/config.h)

#set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BUILD_TYPE RelWithDebInfo)

include_directories (${h_dir} ${h_dir} ${h_dir} ${conf_dir} ${ma_dir} program/space program/ ${Boost_INCLUDE_DIR})

set (HEADERS
	${h_dir}/csv_parser.h
	${h_dir}/function.h
	${h_dir}/log.h
	${CMAKE_CURRENT_LIST_DIR}/program/init.h
	${h_dir}/combinator.h
	${h_dir}/part_of_function.h
	${h_dir}/rotate.h
	${h_dir}/velocity.h
	${h_dir}/roket.h
	${h_dir}/roket_ode.h
	${h_dir}/metrics.h
	${h_dir}/trans_matrix.h
	${h_dir}/point.h
	${h_dir}/vector.h
	${h_dir}/bz_curve.h
	${h_dir}/xy_circle.h
	${h_dir}/sphere.h
	${conf_dir}/config.h
	${h_dir}/runge_kuta.h
	${h_dir}/ode_solver.h
	)

set (SOURCES
	${s_dir}/csv_parser.cpp
	${s_dir}/function.cpp
	${s_dir}/log.cpp
	${s_dir}/combinator.cpp
	${s_dir}/part_of_function.cpp
	${s_dir}/rotate.cpp
	${s_dir}/velocity.cpp
	${s_dir}/roket.cpp
	${s_dir}/roket_ode.cpp
	${s_dir}/metrics.cpp
	${s_dir}/trans_matrix.cpp
	${s_dir}/point.cpp
	${s_dir}/vector.cpp
	${s_dir}/bz_curve.cpp
	${s_dir}/xy_circle.cpp
	${s_dir}/sphere.cpp
	${HEADERS}
	#program/main.cpp)
	#${s_dir}/csv_parser.cpp ${h_dir}/csv_parser.h
	#${s_dir}/function.cpp ${h_dir}/function.h
	#program/space/init.h
	#${s_dir}/combinator.cpp ${h_dir}/combinator.h
	#${s_dir}/part_of_function.cpp ${h_dir}/part_of_function.h
	#${s_dir}/rotate.cpp ${h_dir}/rotate.h
	#${s_dir}/velocity.cpp ${h_dir}/velocity.h
	#${s_dir}/metrics.cpp ${h_dir}/metrics.h
	#${s_dir}/trans_matrix.cpp ${h_dir}/trans_matrix.h
	#${s_dir}/point.cpp ${h_dir}/point.h
	#${s_dir}/vector.cpp ${h_dir}/vector.h
	#${s_dir}/bz_curve.cpp ${h_dir}/bz_curve.h
	#${s_dir}/xy_circle.cpp ${h_dir}/xy_circle.h
	#${s_dir}/sphere.cpp ${h_dir}/sphere.h
	#program/main.cpp)
	)

#program/local_main.cpp

#include_directories (program)
#include_directories (program program/headers)
#add_subdirectory (program)

#set (SOURCES program/main.cpp)

set (STATIC_NAME _uir_static)
set (SHARED_NAME _uir_shared)

#add_library ( ${SHARED_NAME} SHARED ${SOURCES})
add_library ( ${STATIC_NAME} STATIC ${SOURCES})

if (USE_BOOST_LOG)
	#target_link_libraries(${SHARED_NAME} LINK_PUBLIC ${Boost_LIBRARIES})
	target_link_libraries(${STATIC_NAME} LINK_PUBLIC ${Boost_LIBRARIES})
endif()

set(HEADERS_DIR "./headers_dir")

if (WIN32)

	set(HEADERS_DIR "C:/Work/012/src/ric/traceeditor/include")

	message(STATUS "path = '${CMAKE_CURRENT_LIST_DIR}/libs/' filename = '${STATIC_NAME}.lib'")
	add_custom_command(TARGET ${STATIC_NAME} POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E copy
		"${CMAKE_CURRENT_BINARY_DIR}/${STATIC_NAME}.lib"
		"${CMAKE_CURRENT_LIST_DIR}/libs/"
		COMMENT "Copying ${STATIC_NAME}"
		)

endif()



find_package(Boost 1.40 COMPONENTS program_options log)



if (Boost_FOUND)

	add_executable (${PROJECT_NAME} ${SOURCES} program/main.cpp)
	#add_executable (local_${PROJECT_NAME} ${SOURCES} program/local_main.cpp)

	target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${Boost_LIBRARIES})
	#target_link_libraries(local_${PROJECT_NAME} LINK_PUBLIC ${Boost_LIBRARIES})

endif()

file(MAKE_DIRECTORY "${HEADERS_DIR}/${PROJECT_NAME}/")
foreach (myheader_path IN LISTS HEADERS)
	get_filename_component(myheader ${myheader_path} NAME)
	message(STATUS "path = '${myheader_path}' filename = '${myheader}'")
	add_custom_command(TARGET ${STATIC_NAME} POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E copy
		"${myheader_path}"
		"${HEADERS_DIR}/${PROJECT_NAME}/"
		COMMENT "Copying ${myheader_path}"
		)
endforeach()


