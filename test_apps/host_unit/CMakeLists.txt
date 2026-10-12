cmake_minimum_required(VERSION 3.16)
project(vigie_host_unit C)
set(CMAKE_C_STANDARD 11)

include(FetchContent)
FetchContent_Declare(unity
    GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
    GIT_TAG v2.6.0)
FetchContent_MakeAvailable(unity)

# Le code testé est celui du sketch Arduino : vigie_node/src/<composant>/ (en-têtes et sources ensemble).
set(SRC_ROOT ${CMAKE_CURRENT_LIST_DIR}/../../vigie_node/src)
# Composants PURS (C standard, aucune dépendance Arduino). Un composant absent est simplement ignoré.
set(PURE_COMPONENTS filters alarm_fsm telemetry_codec nonce_cache)
set(PURE_SRCS "")
set(PURE_INCS "")
foreach(c ${PURE_COMPONENTS})
    if(EXISTS ${SRC_ROOT}/${c})
        file(GLOB srcs ${SRC_ROOT}/${c}/*.c)
        list(APPEND PURE_SRCS ${srcs})
        list(APPEND PURE_INCS ${SRC_ROOT}/${c})
    endif()
endforeach()

add_library(vigie_pure STATIC ${PURE_SRCS})
target_include_directories(vigie_pure PUBLIC ${PURE_INCS})
target_compile_options(vigie_pure PRIVATE -Wall -Wextra -Werror)

enable_testing()
file(GLOB TEST_SOURCES ${CMAKE_CURRENT_LIST_DIR}/test_*.c)
foreach(src ${TEST_SOURCES})
    get_filename_component(t ${src} NAME_WE)
    add_executable(${t} ${src})
    target_link_libraries(${t} PRIVATE vigie_pure unity)
    add_test(NAME ${t} COMMAND ${t})
endforeach()
