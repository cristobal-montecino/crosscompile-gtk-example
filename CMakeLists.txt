cmake_minimum_required(VERSION 3.21)
project(GtkExample)

# ----

if (CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE "Debug")
endif()

string(TOLOWER CMAKE_BUILD_TYPE CMAKE_BUILD_TYPE_LC)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin/debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/release")

# ----

# ---- GTKMM3 ----

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM3 REQUIRED gtkmm-3.0)

# ----

# ---- EXECUTABLE ----

set(gtkexample_CXX_SOURCES
"src/main.cpp"
)

add_executable(gtkexample ${gtkexample_CXX_SOURCES})

if (CMAKE_BUILD_TYPE_LC STREQUAL "debug")
    set(gtkexample_CXX_BUILD_TYPE_COMPILE_OPTIONS "-g")
elseif (CMAKE_BUILD_TYPE_LC STREQUAL "release")
    set(gtkexample_CXX_BUILD_TYPE_COMPILE_OPTIONS "-O3")
endif()

target_compile_options(gtkexample
    PUBLIC ${gtkexample_CXX_BUILD_TYPE_COMPILE_OPTIONS}
)

target_include_directories(gtkexample
    PUBLIC ${GTKMM3_INCLUDE_DIRS}
)

if(MINGW)
    # -mwindows: Windows GUI Application instead of Console Application
    set(LINK_OPTION_MWINDOWS "-mwindows")
else()
    set(LINK_OPTION_MWINDOWS "")
endif()

target_link_options(gtkexample
    PUBLIC ${LINK_OPTION_MWINDOWS}
)

target_link_libraries(gtkexample
    PUBLIC ${GTKMM3_LINK_LIBRARIES}
)

# ----

# ---- INSTALL DLLS ----

if (MINGW)

find_program(MINGW_OBJDUMP objdump REQUIRED)

install(CODE "set(GTKMM3_PREFIX \"${GTKMM3_PREFIX}\")")
install(CODE "set(MINGW_OBJDUMP \"${MINGW_OBJDUMP}\")")
install(CODE [[
    set(CMAKE_GET_RUNTIME_DEPENDENCIES_PLATFORM "windows+pe")
    set(CMAKE_GET_RUNTIME_DEPENDENCIES_TOOL "objdump")    
    set(CMAKE_GET_RUNTIME_DEPENDENCIES_COMMAND "${MINGW_OBJDUMP}")

    file(GET_RUNTIME_DEPENDENCIES
        RESOLVED_DEPENDENCIES_VAR RESOLVED_DEPS
        UNRESOLVED_DEPENDENCIES_VAR UNRESOLVED_DEPS
        EXECUTABLES "$<TARGET_FILE:gtkexample>"
        DIRECTORIES "${GTKMM3_PREFIX}/bin"
    )

    # Ignore some native windows dlls
    list(REMOVE_ITEM RESOLVED_DEPS "advapi32.dll" "comctl32.dll" "comdlg32.dll" "dnsapi.dll" "dwmapi.dll" "gdi32.dll" "hid.dll" "imm32.dll" "iphlpapi.dll" "kernel32.dll" "msimg32.dll" "msvcrt.dll" "ole32.dll" "setupapi.dll" "shell32.dll" "shlwapi.dll" "user32.dll" "winmm.dll" "ws2_32.dll")
    list(REMOVE_ITEM UNRESOLVED_DEPS "advapi32.dll" "comctl32.dll" "comdlg32.dll" "dnsapi.dll" "dwmapi.dll" "gdi32.dll" "hid.dll" "imm32.dll" "iphlpapi.dll" "kernel32.dll" "msimg32.dll" "msvcrt.dll" "ole32.dll" "setupapi.dll" "shell32.dll" "shlwapi.dll" "user32.dll" "winmm.dll" "ws2_32.dll")
    
    # Warn about unresolved dlls
    list(LENGTH UNRESOLVED_DEPS UNRESOLVED_DEPS_LEN)
    if (UNRESOLVED_DEPS_LEN GREATER 0)
        set(MSG "Unresolved DLLs:\n")

        foreach(UNRESOLVED ${UNRESOLVED_DEPS})
            list(APPEND MSG "  ${UNRESOLVED}\n")
        endforeach()

        message(WARNING "" ${MSG})
    endif()

    foreach(RESOLVED ${RESOLVED_DEPS})
        file(INSTALL "${RESOLVED}" DESTINATION "$<TARGET_FILE_DIR:gtkexample>")
    endforeach() 
]])

endif()

# ----

# ---- INSTALL GTK ICONS ---

if (MINGW)
    install(CODE [[
        file(INSTALL "${GTKMM3_PREFIX}/share/icons" DESTINATION "$<TARGET_FILE_DIR:gtkexample>/share")
    ]])
endif()

# ----