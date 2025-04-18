# ###########################################################################
# User specific settings
# Copyright 2022-2023 Aregtech
# ###########################################################################

# ---------------------------------------------------------------------------
# This CMake file allows configuration of various AREG Framework build options.
# You can pass these options via the command line, or the system will use default values.
#
# Available Options:
#   1. AREG_COMPILER_FAMILY -- A quick way to set the C++ and C compilers (CMAKE_CXX_COMPILER and CMAKE_C_COMPILER).
#   2. AREG_COMPILER        -- Sets a specific compiler for both C++ and C projects.
#   3. AREG_TARGET          -- Specifies the compiler and library architecture target. Defaults to the system-defined compiler and architecture.
#   4. AREG_PROCESSOR       -- The processor architect. Ignore if need to use system default.
#   5. AREG_BINARY          -- Specifies the library type for the AREG Framework ('shared' or 'static'). Defaults to 'shared'.
#   6. AREG_LOGGER_BINARY   -- Specifies the type of the Log Observer API library ('shared' or 'static'). Defaults to 'shared'.
#   7. AREG_BUILD_TYPE      -- Specifies the build configuration ('Debug' or 'Release').
#   8. AREG_BUILD_TESTS     -- Enables or disables building unit tests for the AREG Framework.
#   9. AREG_BUILD_EXAMPLES  -- Enables or disables building examples for the AREG Framework.
#  10. AREG_EXTENDED        -- Enables or disables extended AREG Framework features. May require additional dependencies.
#  11. AREG_LOGS            -- Enables or disables logging during compilation. Defaults to 'enabled'.
#  12. AREG_USE_PACKAGES    -- Enables or disables using installed packages. Controls other package options like SQLite and GTest.
#  13. AREG_SQLITE_PACKAGE  -- Determines if the system's SQLite3 package should be used or compiled from source.
#  14. AREG_GTEST_PACKAGE   -- Determines if the system's GTest package should be used or compiled from source.
#  15. AREG_ENABLE_OUTPUTS  -- If disabled, output directories will match the CMake binary directory.
#  16. AREG_BUILD_ROOT      -- Specifies the root directory for build files. Defaults to './product' within the AREG SDK root.
#  17. AREG_OUTPUT_DIR      -- Directory where build outputs are placed.
#  18. AREG_OUTPUT_BIN      -- Directory for output binaries (executables and shared libraries).
#  19. AREG_OUTPUT_LIB      -- Directory for output static libraries.
#  20. AREG_PACKAGES        -- Location for fetching third-party packages such as GTest.
#  21. AREG_INSTALL         -- Enables or disables installation of AREG SDK binaries, headers and dependencies like 'sqlite3' and 'ncurses'.
#  22. AREG_INSTALL_PATH    -- Location where AREG SDK binaries, headers, and tools are installed. Defaults to the user's home directory.
#
# Default Values:
#   1. AREG_COMPILER_FAMILY = <default> (possible values: gnu, cygwin, llvm, msvc)
#   2. AREG_COMPILER        = <default> (possible values: g++, gcc, c++, cc, clang++, clang, clang-cl, cl)
#   3. AREG_TARGET          = <default> (possible values: 'i386-linux-gnu', 'x86_64-linux-gnu', 'arm-linux-gnueabihf', 'aarch64-linux-gnu')
#   4. AREG_PROCESSOR       = System    (possible values: x86 (i386, i486), x64 (x86_64, x86-64, amd64, ia64), arm (arm32, armv7), aarch64 (arm64))
#   5. AREG_BINARY          = shared    (possible values: shared, static)
#   6. AREG_LOGGER_BINARY   = shared    (possible values: shared, static)
#   7. AREG_BUILD_TYPE      = Release   (possible values: Release, Debug)
#   8. AREG_BUILD_TESTS     = ON        (possible values: ON, OFF)
#   9. AREG_BUILD_EXAMPLES  = ON        (possible values: ON, OFF)
#  10. AREG_EXTENDED        = OFF       (possible values: ON, OFF)
#  11. AREG_LOGS            = ON        (possible values: ON, OFF)
#  12. AREG_USE_PACKAGES    = ON        (possible values: ON, OFF)
#  13. AREG_SQLITE_PACKAGE  = ON        (possible values: ON, OFF)
#  14. AREG_GTEST_PACKAGE   = ON        (possible values: ON, OFF)
#  15. AREG_ENABLE_OUTPUTS  = ON        (possible values: ON, OFF)
#  16. AREG_BUILD_ROOT      = '<areg-sdk>/product'    (path for output and generated files)
#  17. AREG_OUTPUT_DIR      = '<areg-sdk>/product/build/<default-compiler family-name>/<os>-<bitness>-<cpu>-release-<areg-lib>'
#  18. AREG_OUTPUT_BIN      = '<areg-sdk>/product/build/<default-compiler family-name>/<os>-<bitness>-<cpu>-release-<areg-lib>/bin'
#  19. AREG_OUTPUT_LIB      = '<areg-sdk>/product/build/<default-compiler family-name>/<os>-<bitness>-<cpu>-release-<areg-lib>/lib'
#  20. AREG_PACKAGES        = '${CMAKE_BINARY_DIR}/packages'
#  21. AREG_INSTALL         = ON        (possible values: ON, OFF)
#  22. AREG_INSTALL_PATH    = '${HOME}/areg-sdk' (or '${USERPROFILE}' on Windows, defaults to current directory if unset)
#
# Hints:
#   - AREG_COMPILER_FAMILY is an easy way to set compilers:
#       - 'gnu' sets g++ and gcc for C++ and C.
#       - 'cygwin' sets g++ and gcc for C++ and C under Cygwin.
#       - 'llvm' sets clang++, clang, and clang-cl (on Windows) for C++ and C.
#       - 'msvc' sets Microsoft Visual C++ (cl) for C++ and C.
#
# Example Command:
# $ cmake -B ./build -DCMAKE_BUILD_TYPE=Debug -DAREG_COMPILER_FAMILY=llvm -DAREG_BUILD_TESTS=ON -DAREG_BUILD_EXAMPLES=ON
#
# Notes:
#   - To integrate manually build binaries of AREG SDK into another project, you can simply set `AREG_BUILD_ROOT` to the project's root directory.
#
# Example Command with AREG_BUILD_ROOT to output binaries in custom directory:
# $ cmake -B ./build -DAREG_BUILD_ROOT="/home/developer/projects/my_project/product"
#
# Hint:
# To use AREG SDK cmake options in other project, the 'user.cmake' file should be included before first call of "project()".
# Otherwise, AREG SDK settings uses cmake options CMAKE_CXX_COMPILER and CMAKE_BUILD_TYPE options.
#
# Integration:
#   - Visit https://github.com/aregtech/areg-sdk-demo repository to see various ways of AREG Framework integration.
# ---------------------------------------------------------------------------

# Set variables for C++ and C compilers and their short name
set(AREG_CXX_COMPILER)
set(AREG_C_COMPILER)
set(AREG_COMPILER_SHORT)

# Specify CPU platform here, the system CPU platform is detected in 'commmon.cmake'
if (DEFINED AREG_PROCESSOR AND NOT "${AREG_PROCESSOR}" STREQUAL "")
    macro_get_processor(${AREG_PROCESSOR} AREG_PROCESSOR AREG_BITNESS _found_proc)
    set(CMAKE_SYSTEM_PROCESSOR ${AREG_PROCESSOR})
endif()

if (DEFINED CMAKE_CXX_COMPILER_TARGET AND NOT "${CMAKE_CXX_COMPILER_TARGET}" STREQUAL "")
    set(AREG_TARGET ${CMAKE_CXX_COMPILER_TARGET})
endif()

# If CMake compilers are specified, use them
if ((DEFINED CMAKE_CXX_COMPILER OR DEFINED CMAKE_C_COMPILER) AND (NOT "${CMAKE_CXX_COMPILER}" STREQUAL "" OR NOT "${CMAKE_C_COMPILER}" STREQUAL ""))

    # Determine the system compiler based on the available CMake variables
    if (NOT "${CMAKE_CXX_COMPILER}" STREQUAL "")
        set(_sys_compiler "${CMAKE_CXX_COMPILER}")
    else()
        set(_sys_compiler "${CMAKE_C_COMPILER}")
    endif()

    message(STATUS "AREG: >>> Using CMake specified C++ compiler '${_sys_compiler}'")

    if (DEFINED AREG_PROCESSOR AND NOT "${AREG_PROCESSOR}" STREQUAL "")
        set(_sys_process ${AREG_PROCESSOR})
    elseif(DEFINED CMAKE_SYSTEM_PROCESSOR AND NOT "${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "")
        set(_sys_process ${CMAKE_SYSTEM_PROCESSOR})
    endif()

    # Setup compiler details based on the identified system compiler
    macro_setup_compilers_data("${_sys_compiler}" _compiler_family _compiler_short _cxx_compiler _c_compiler _sys_target _sys_process _sys_bitness _compiler_found)

    if (_compiler_found)
        # Check for existing compiler family or specific compiler and issue warnings if necessary
        if (NOT "${AREG_COMPILER_FAMILY}" STREQUAL "" AND NOT "${AREG_COMPILER_FAMILY}" STREQUAL "${_compiler_family}")
            message(WARNING "AREG: >>> Selected compiler family '${AREG_COMPILER_FAMILY}' is ignored; using '${_compiler_family}'")
        endif()

        # Only if AREG_COMPILER is defined, compare the short compiler name to the full path
        if (NOT "${AREG_COMPILER}" STREQUAL "")
            string(FIND "${AREG_COMPILER}" "${_compiler_short}" _found_pos)
            if (_found_pos LESS 0)
                message(WARNING "AREG: >>> Selected compiler '${AREG_COMPILER}' is ignored; using '${_compiler_short}'")
            endif()
        endif()

        # Set the relevant variables for the compiler
        set(AREG_COMPILER_FAMILY    "${_compiler_family}")
        set(AREG_COMPILER           "${_sys_compiler}")
        set(AREG_COMPILER_SHORT     "${_compiler_short}")
        set(AREG_CXX_COMPILER       "${_sys_compiler}")
        set(AREG_C_COMPILER         "${_c_compiler}")
        if("${AREG_TARGET}" STREQUAL "")
            set(AREG_TARGET         "${_sys_target}")
        endif()
        if (NOT "${_sys_process}" STREQUAL "")
            set(AREG_PROCESSOR ${_sys_process})
            set(AREG_BITNESS   ${_sys_bitness})
            set(CMAKE_SYSTEM_PROCESSOR ${AREG_PROCESSOR})
        endif()
    else()
        message(WARNING "AREG: >>> Unknown C++ compiler '${_sys_compiler}'; results may be unpredictable")
    endif()

    unset(_sys_compiler)
    unset(_sys_process)
    unset(_sys_bitness)
    unset(_sys_target)
    unset(_compiler_found)

# If a specific compiler family is set, use that to determine compilers
elseif (DEFINED AREG_COMPILER_FAMILY AND NOT "${AREG_COMPILER_FAMILY}" STREQUAL "")

    message(STATUS "AREG: >>> Using user-specified C/C++ compiler family '${AREG_COMPILER_FAMILY}'")
    if ("${AREG_PROCESSOR}" STREQUAL "")
        set(AREG_PROCESSOR ${CMAKE_SYSTEM_PROCESSOR})
    endif()
    macro_setup_compilers_data_by_family("${AREG_COMPILER_FAMILY}" _compiler_short _cxx_compiler _c_compiler _compiler_target _compiler_found)

    if (_compiler_found)
        # Set the relevant variables for the chosen compiler family
        set(AREG_COMPILER           "${_cxx_compiler}")
        set(AREG_COMPILER_SHORT     "${_compiler_short}")
        set(AREG_CXX_COMPILER       "${_cxx_compiler}")
        set(AREG_C_COMPILER         "${_c_compiler}")
        if("${AREG_TARGET}" STREQUAL "")
            set(AREG_TARGET         "${_compiler_target}")
        endif()
    else()
        message(WARNING "AREG: >>> Unknown compiler family '${AREG_COMPILER_FAMILY}'; results may be unpredictable")
    endif()

    unset(_compiler_short)
    unset(_cxx_compiler)
    unset(_c_compiler)
    unset(_compiler_target)
    unset(_compiler_found)

# If a specific compiler is set, use that to determine compilers
elseif (DEFINED AREG_COMPILER AND NOT "${AREG_COMPILER}" STREQUAL "")

    message(STATUS "AREG: >>> Using user-specified C/C++ compiler '${AREG_COMPILER}'")
    # Set both C and C++ compilers based on AREG_COMPILER
    macro_setup_compilers_data("${AREG_COMPILER}" _compiler_family _compiler_short _cxx_compiler _c_compiler _sys_target _sys_process _sys_bitness _compiler_found)

    if (_compiler_found)
        # Set the relevant variables for the chosen compiler
        set(AREG_COMPILER_FAMILY    "${_compiler_family}")
        set(AREG_COMPILER_SHORT     "${_compiler_short}")
        set(AREG_CXX_COMPILER       "${_cxx_compiler}")
        set(AREG_C_COMPILER         "${_c_compiler}")
        if("${AREG_TARGET}" STREQUAL "")
            set(AREG_TARGET         "${_sys_target}")
        endif()
        if (NOT "${_sys_process}" STREQUAL "")
            set(AREG_PROCESSOR ${_sys_process})
            set(AREG_BITNESS   ${_sys_bitness})
            set(CMAKE_SYSTEM_PROCESSOR ${AREG_PROCESSOR})
        endif()
    else()
        message(WARNING "AREG: >>> Unknown compiler '${AREG_COMPILER}'; results may be unpredictable")
    endif()

    unset(_sys_compiler)
    unset(_sys_process)
    unset(_sys_bitness)
    unset(_sys_target)
    unset(_compiler_found)

# If no specific compiler or family is set, use the system default
else()
    message(STATUS "AREG: >>> No compiler specified; using system default compilers.")
endif()

# Set build configuration. Set "Debug" for debug build, and "Release" for release build.
if (NOT DEFINED AREG_BUILD_TYPE OR "${AREG_BUILD_TYPE}" STREQUAL "")
    if (NOT DEFINED CMAKE_BUILD_TYPE OR "${CMAKE_BUILD_TYPE}" STREQUAL "")
        set(AREG_BUILD_TYPE "Release")
    else()
        set(AREG_BUILD_TYPE "${CMAKE_BUILD_TYPE}")
    endif()
endif()

# Set the AREG binary library type to compile. Set "shared" if not "static"
if (NOT DEFINED AREG_BINARY)
    if (DEFINED VCPKG_LIBRARY_LINKAGE AND "${VCPKG_LIBRARY_LINKAGE}" STREQUAL "static")
        set(AREG_BINARY "static")
    else()
        set(AREG_BINARY "shared")
    endif()
elseif (NOT "${AREG_BINARY}" STREQUAL "static")
    set(AREG_BINARY "shared")
endif()

# Set the areg log observer API library type.
if (NOT DEFINED AREG_LOGGER_BINARY)
    if (DEFINED VCPKG_LIBRARY_LINKAGE AND "${VCPKG_LIBRARY_LINKAGE}" STREQUAL "static")
        set(AREG_LOGGER_BINARY "static")
    else()
        set(AREG_LOGGER_BINARY "shared")
    endif()
elseif (NOT "${AREG_LOGGER_BINARY}" STREQUAL "static")
    set(AREG_LOGGER_BINARY "shared")
endif()

# Build tests. By default it is disabled. To enable, set ON
macro_create_option(AREG_BUILD_TESTS ON "Build unit tests")

# Build examples. By default it is disabled. To enable, set ON
macro_create_option(AREG_BUILD_EXAMPLES ON "Build examples")

# Set AREG extended features enable or disable flag to compiler additional optional features. By default, it is disabled.
macro_create_option(AREG_EXTENDED OFF "Enable extended feature")

# Modify 'AREG_LOGS' to enable or disable compilation with logs. By default, compile with logs
macro_create_option(AREG_LOGS ON "Compile with logs")

# Modify 'AREG_INSTALL' to enable or disable installation of AREG SDK
macro_create_option(AREG_INSTALL ON "Enable installation")

# Check the request of using installed packages
if (NOT DEFINED AREG_USE_PACKAGES)
    # Set default values
    macro_create_option(AREG_USE_PACKAGES      ON  "Enable using installed packages")
    macro_create_option(AREG_SQLITE_PACKAGE    ON  "Use SQLite3 installed package")
    macro_create_option(AREG_GTEST_PACKAGE     ON  "Use GTest installed package")
elseif(AREG_USE_PACKAGES)
    # Using packages is enabled by default, set values
    macro_create_option(AREG_USE_PACKAGES      ON  "Enable using installed packages")
    macro_create_option(AREG_SQLITE_PACKAGE    ON  "Use SQLite3 installed package")
    macro_create_option(AREG_GTEST_PACKAGE     ON  "Use GTest installed package")
else()
    # Using packages is disabled by default, set values
    macro_create_option(AREG_USE_PACKAGES      OFF "Enable using installed packages")
    macro_create_option(AREG_SQLITE_PACKAGE    OFF "Use SQLite3 installed package")
    macro_create_option(AREG_GTEST_PACKAGE     OFF "Use GTest installed package")
    macro_create_option(AREG_INSTALL           OFF "Enable installation")
endif()

