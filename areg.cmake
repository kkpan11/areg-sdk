# ###########################################################################
# Settings for external projects to integrate AREG framework
# Copyright (c) 2022-2023 Aregtech
# ###########################################################################

# include this cmake file in the project if want to use
# areg-sdk features like automatic code generator
# and constants.

# Define AREG_SDK_ROOT before including this file in the project.
if (NOT DEFINED AREG_SDK_ROOT OR "${AREG_SDK_ROOT}" STREQUAL "")
    # Make sure that AREG_SDK_ROOT is set before the 'setup.cmake' is included
    message(FATAL_ERROR "AREG: >>> Set \'AREG_SDK_ROOT\' before including \'setup.cmake\'. Stopping building the project.")
    return()
else()
    message(STATUS "AREG: >>> Specified AREG_SDK_ROOT = \'${AREG_SDK_ROOT}\'")
endif()

# The location of cmake configuration files.
if (NOT DEFINED AREG_CMAKE_CONFIG_DIR OR "${AREG_CMAKE_CONFIG_DIR}" STREQUAL "")
    set(AREG_CMAKE_CONFIG_DIR   "${AREG_SDK_ROOT}/conf/cmake")
else()
    message(STATUS "AREG: >>> Specified AREG_CMAKE_CONFIG_DIR = \'${AREG_CMAKE_CONFIG_DIR}\'")
endif()

# Disable or enable areg-sdk specific outputs
if (NOT DEFINED AREG_ENABLE_OUTPUTS OR "${AREG_ENABLE_OUTPUTS}" STREQUAL "")
    option(AREG_ENABLE_OUTPUTS "Disable AREG SDK specific outputs, use defaults" OFF)
    message(STATUS "AREG: >>> Disable AREG SDK specific outputs, use defaults")
endif()

# Include setup.cmake to initialize variables
message(STATUS "AREG: >>> Including \'${AREG_CMAKE_CONFIG_DIR}/setup.cmake\'")
include("${AREG_CMAKE_CONFIG_DIR}/setup.cmake")

# Include common.cmake to initialize compile options
message(STATUS "AREG: >>> Including \'${AREG_CMAKE_CONFIG_DIR}/common.cmake\'")
include("${AREG_CMAKE_CONFIG_DIR}/common.cmake")

# Set the 'framework' in the include directories
include_directories("${AREG_FRAMEWORK}")
# Now the header files of the areg framework can be included in the projects.
