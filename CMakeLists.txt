set(TARGET OTP)

project(${TARGET})

cmake_minimum_required(VERSION 3.18)

set(ROOT ..)
set(SRC_DIR ${ROOT}/Sources)
set(INC_DIR ${ROOT}/Includes)
set(LIB_DIR ${ROOT}/Libraries)

include_directories(Includes)
include_directories(Libraries/freeglut/include)

# 소스 설정
set(SOURCE ${SRC_DIR}/Main.cpp ${SRC_DIR}/Client.cpp)
set(HEADERS ${INC_DIR}/Client.hpp)

# 프로젝트 생성
add_executable(${TARGET} ${SOURCE} ${HEADERS})

# 윈도우 종류 설정
set_target_properties(${TARGET} PROPERTIES LINK_FLAGS_RELEASE /SUBSYSTEM:WINDOWS)
set_target_properties(${TARGET} PROPERTIES LINK_FLAGS_MINSIZEREL /SUBSYSTEM:WINDOWS)
set_target_properties(${TARGET} PROPERTIES LINK_FLAGS_RELWITHDEBINFO /SUBSYSTEM:WINDOWS)
set_target_properties(${TARGET} PROPERTIES LINK_FLAGS_DEBUG /SUBSYSTEM:CONSOLE)

if (CMAKE_SIZEOF_VOID_P EQUAL 8) # 64bit
    # .lib 파일 연결
    target_link_libraries(${TARGET} ${LIB_DIR}/freeglut/x64/freeglut)

    # .dll 파일 복사
    add_custom_command(TARGET ${TARGET} POST_BUILD 
        COMMAND ${CMAKE_COMMAND} -E copy_if_different 
            ${LIB_DIR}/freeglut/x64/freeglut.dll $<TARGET_FILE_DIR:${TARGET}>
    )
elseif (CMAKE_SIZEOF_VOID_P EQUAL 4) # 32bit
    # .lib 파일 연결
    target_link_libraries(${TARGET} ${LIB_DIR}/freeglut/x86/freeglut)

    # .dll 파일 복사
    add_custom_command(TARGET ${TARGET} POST_BUILD 
        COMMAND ${CMAKE_COMMAND} -E copy_if_different 
            ${LIB_DIR}/freeglut/x86/freeglut.dll $<TARGET_FILE_DIR:${TARGET}>
    )
endif()