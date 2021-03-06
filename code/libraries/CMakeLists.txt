# Copyright Petr Kmoch 2014-2015.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)

get_filename_component(AtonIncludeDir ${CMAKE_CURRENT_LIST_DIR}/../include/aton ABSOLUTE)

set(InternalDir aton_)

option(GENERATE_DEVELOPMENT_TARGETS "Generate targets for developing Aton (mainly for IDE generators)" OFF)

include_directories(${AtonIncludeDir}/..)

find_package(Boost)
if(NOT Boost_FOUND)
	set(BOOST_ROOT "" CACHE PATH "Boost installation prefix to search")
	message(SEND_ERROR "Could not find Boost, try setting BOOST_ROOT or one of the other variables supported by FindBoost module.")
endif()
include_directories(${Boost_INCLUDE_DIRS})

if(BUILD_TESTING)
	add_subdirectory(../unit_tests ${CMAKE_BINARY_DIR}/unit_tests)
endif()

macro(ListPop OutVar List)
	list(GET ${List} ${ARGN} ${OutVar})
	list(REMOVE_AT ${List} ${ARGN})
endmacro()

function(SetupSourceGroup Name)
	set(Groups ${Groups} ${Name} PARENT_SCOPE)
	set(Group_${Name} "" PARENT_SCOPE)
	set(ActiveGroup ${Name} PARENT_SCOPE)
endfunction()

function(AddAtonLibrary LibraryName LibraryKind)
	set(Args ${ARGN})
	set(LibraryDir "")
	set(Mode "")
	unset(Directory)
	set(Files "")
	set(HeaderGroup "Header Files")
	unset(ActiveGroup)
	set(Groups "")
	while(Args)
		ListPop(Arg Args 0)
		if(Arg STREQUAL "LIBRARY_DIR")
			if(LibraryDir)
				message(FATAL_ERROR "Duplicate LIBRARY_DIR.")
			endif()
			ListPop(LibraryDir Args 0)
			if(LibraryDir)
				set(LibraryDir /${LibraryDir})
			endif()
			set(Mode "")
		elseif(Arg STREQUAL "AGGREGATE")
			SetupSourceGroup("Aggregate Headers")
			set(FileRoot ${AtonIncludeDir})
			set(FileDir ${FileRoot})
			set(Mode FILES)
		elseif(Arg STREQUAL "TOPDIR")
			set(Directory "")
			SetupSourceGroup(${HeaderGroup})
			set(FileRoot ${AtonIncludeDir}${LibraryDir})
			set(Mode DIRECTORY)
		elseif(Arg STREQUAL "SUBDIR")
			ListPop(Directory Args 0)
			SetupSourceGroup(${HeaderGroup}\\${Directory})
			set(FileRoot ${AtonIncludeDir}${LibraryDir}/${Directory})
			set(Mode DIRECTORY)
		elseif(Arg STREQUAL "PUBLIC")
			if(NOT DEFINED Directory)
				message(FATAL_ERROR "PUBLIC outside of a directory specification.")
			endif()
			set(FileDir ${FileRoot})
			set(Mode FILES)
		elseif(Arg STREQUAL "INTERNAL")
			if(NOT DEFINED Directory)
				message(FATAL_ERROR "INTERNAL outside of a directory specification.")
			endif()
			SetupSourceGroup(${ActiveGroup}\\Internal)
			set(FileDir ${FileRoot}/${InternalDir})
			set(Mode FILES)
		else()
			if(Mode STREQUAL "FILES")
				set(File ${FileDir}/${Arg})
				list(APPEND Group_${ActiveGroup} ${File})
				list(APPEND Files ${File})
			else()
				message(FATAL_ERROR "Unknown argument `${Arg}`.")
			endif()
		endif()
	endwhile()
	if(NOT Files)
		return()
	endif()
	if(GENERATE_DEVELOPMENT_TARGETS)
		add_library(
			${LibraryName}-devel STATIC
			${Files}
		)
		if(LibraryKind STREQUAL "INCLUDED")
			set_property(TARGET ${LibraryName}-devel PROPERTY LINKER_LANGUAGE CXX)
		endif()
		foreach(Group IN LISTS Groups)
			source_group(${Group} FILES ${Group_${Group}})
		endforeach()
	endif()
endfunction()

function(AddTestDriver)
	set(Args ${ARGN})
	unset(LibraryName)
	set(Mode "")
	set(Files "")
	set(Tests "")
	while(Args)
		ListPop(Arg Args 0)
		if(Arg STREQUAL "LIBRARY")
			ListPop(LibraryName Args 0)
			set(Mode "")
		elseif(Arg STREQUAL "SOURCES")
			set(Mode SOURCES)
		elseif(Arg STREQUAL "TESTS")
			set(Mode TESTS)
		else()
			if(Mode STREQUAL "SOURCES")
				list(APPEND Files ${Arg})
			elseif(Mode STREQUAL "TESTS")
				list(APPEND Files ${Arg}.cpp)
				list(APPEND Tests ${Arg})
			else()
				message(FATAL_ERROR "Unknown argument `${Arg}`.")
			endif()
		endif()
	endwhile()
	if(NOT LibraryName)
		message(FATAL_ERROR "AddTestDriver() missing required LIBRARY argument.")
	endif()
	set(DriverTarget ${LibraryName}-test-driver)
	set(InitFile ${CMAKE_CURRENT_BINARY_DIR}/${DriverTarget}-init.cpp)
	file(WRITE ${InitFile} "#define BOOST_TEST_MODULE ${LibraryName}\n#include \"boost/test/unit_test.hpp\"\n")
	add_executable(${DriverTarget} ${Files} ${InitFile})
	set_target_properties(${DriverTarget} PROPERTIES FOLDER Testing)
	target_link_libraries(${DriverTarget} unit_tests)
	
	foreach(Test IN LISTS Tests)
		add_test(
			NAME ${LibraryName}:${Test}
			COMMAND ${DriverTarget} --run_test=${Test}
		)
	endforeach()
endfunction()

add_subdirectory(config)
add_subdirectory(infra)
add_subdirectory(range)
