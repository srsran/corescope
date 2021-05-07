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

# Link libstdc++ and libgcc
if(BUILD_STATIC)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++ -static-libgcc")
endif(BUILD_STATIC)


if (RPATH)
  SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
endif (RPATH)

set(SRSRAN_SOURCES srsran_common srsran_mac srsran_phy srsran_radio rrc_asn1 s1ap_asn1 ngap_nr_asn1 enb_cfg_parser srslog system)

add_subdirectory(rest_api)
add_subdirectory(gnb)
add_subdirectory(ue)

add_executable(corescope main.cc scope.cc rest_api/scope_rest.cc rest_api/error_handler.cc clock.cc)
target_link_libraries(corescope  PUBLIC oatpp::oatpp
                                 PUBLIC oatpp::oatpp-test
                                 PUBLIC oatpp::oatpp-swagger
                                 srscs_gnb
                                 srscs_ue
                                 srsenb_upper
                                 srsenb_rrc
                                 srsenb_s1ap
                                 srsenb_common
                                 srsgnb_upper
                                 srsgnb_ngap
                                 srsue_upper
                                 ${SRSRAN_SOURCES}
                                 ${CMAKE_THREAD_LIBS_INIT}
                                 ${Boost_LIBRARIES}
                                 ${SEC_LIBRARIES}
                                 ${LIBCONFIGPP_LIBRARIES}
                                 ${SCTP_LIBRARIES}
                                 )

if (RPATH)
  set_target_properties(corescope_lib PROPERTIES INSTALL_RPATH ".")
endif (RPATH)

# Run clang-tidy if available
#if (DO_CLANG_TIDY)
#  set_target_properties(srsCoreScope PROPERTIES CXX_CLANG_TIDY "${DO_CLANG_TIDY}")
#endif (DO_CLANG_TIDY)

install(TARGETS corescope DESTINATION ${RUNTIME_DIR})



########################################################################
# Option to run command after build (useful for remote builds)
########################################################################
if (NOT ${BUILDENB_CMD} STREQUAL "")
  message(STATUS "Added custom post-build-ENB command: ${BUILDENB_CMD}")
  add_custom_command(TARGET corescope POST_BUILD COMMAND ${BUILDENB_CMD})
else(NOT ${BUILDENB_CMD} STREQUAL "")
  message(STATUS "No post-build-ENB command defined")
endif (NOT ${BUILDENB_CMD} STREQUAL "")