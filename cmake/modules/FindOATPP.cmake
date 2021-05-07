#
# Copyright 2013-2021 Software Radio Systems Limited
#
# This file is part of CoreScope
#
# CoreScope is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as
# published by the Free Software Foundation, either version 3 of
# the License, or (at your option) any later version.
#
# CoreScope is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Affero General Public License for more details.
#
# A copy of the GNU Affero General Public License can be found in
# the LICENSE file in the top-level directory of this distribution
# and at http://www.gnu.org/licenses/.
#

# - Try to find OATPP
# Once done this will define
#  OATPP_FOUND        - System has oatpp
#  OATPP_INCLUDE_DIRS - The oatpp include directories
#  OATPP_LIBRARIES    - The oatpp library

find_package(PkgConfig)
pkg_check_modules(PC_OATPP QUIET oATPP)
IF(NOT OATPP_FOUND)

FIND_PATH(
    OATPP_INCLUDE_DIRS
    NAMES oatpp-test/Checker.hpp
    HINTS ${PC_OATPP_INCLUDEDIR}
          ${PC_OATPP_INCLUDE_DIRS}
          $ENV{OATPP_DIR}/include/
    PATHS /usr/local/include
          /usr/include
          /usr/local/
          /usr/local/include/oatpp-1.2.5/oatpp/
)

FIND_LIBRARY(
    OATPP_LIBRARIES
    NAMES oatpp
    HINTS ${PC_OATPP_LIBDIR}
          ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          $ENV{OATPP_DIR}/lib
          /usr/local/lib/oatpp-1.2.5/
    PATHS /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

FIND_LIBRARY(
    OATPP_TEST_LIBRARIES
    NAMES oatpp-test
    HINTS ${PC_OATPP_LIBDIR}
          ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          $ENV{OATPP_DIR}/lib
          /usr/local/lib/oatpp-1.2.5/
    PATHS /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

message(STATUS "OATPP INCLUDE DIRS " ${OATPP_INCLUDE_DIRS})
message(STATUS "OATPP LIBRARIES " ${OATPP_LIBRARIES})
message(STATUS "OATPP TEST LIBRARIES " ${OATPP_TEST_LIBRARIES})


INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(OATPP DEFAULT_MSG OATPP_LIBRARIES OATPP_INCLUDE_DIRS)
MARK_AS_ADVANCED(OATPP_LIBRARIES OATPP_INCLUDE_DIRS)

ENDIF(NOT OATPP_FOUND)