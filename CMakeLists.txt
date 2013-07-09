INCLUDE(FindCURL)
INCLUDE(FindZLIB)

cmake_minimum_required(VERSION 2.6)

SET(BUILD_PARTIAL TRUE)

IF(NOT ZLIB_FOUND)
	message(STATUS "zlib is required for partial!")
	SET(BUILD_PARTIAL FALSE)
ENDIF(NOT ZLIB_FOUND)

IF(NOT CURL_FOUND)
	message(STATUS "curl is required for partial!")
	SET(BUILD_PARTIAL FALSE)
ENDIF(NOT CURL_FOUND)

IF(BUILD_PARTIAL)
	include_directories(${ZLIB_INCLUDE_DIR})
	include_directories(${CURL_INCLUDE_DIR})
	include_directories(include)

	IF(WIN32)
		add_definitions(-DCURL_STATICLIB)
	ENDIF(WIN32)

	add_library(partial SHARED partial.c)
	target_link_libraries(partial ${CURL_LIBRARIES} ${ZLIB_LIBRARIES})

	add_executable(partialzip test.c)
	target_link_libraries(partialzip partial)
ENDIF(BUILD_PARTIAL)

install(TARGETS partialzip DESTINATION bin)
install(TARGETS partial DESTINATION lib)
install(FILES include/partial/partial.h DESTINATION include)

