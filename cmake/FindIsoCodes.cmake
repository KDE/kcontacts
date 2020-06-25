# - Try to find iso-codes
# Once done this will define
#
#  IsoCodes_FOUND - system has iso-codes
#  IsoCodes_DOMAINS - the available domains provided by iso-codes
#
# SPDX-FileCopyrightText: 2016 Pino Toscano <pino@kde.org>
#
# SPDX-License-Identifier: BSD-3-Clause

macro(_ISOCODES_GET_PKGCONFIG_VAR _outvar _varname)
  execute_process(
    COMMAND ${PKG_CONFIG_EXECUTABLE} --variable=${_varname} iso-codes
    OUTPUT_VARIABLE _result
    RESULT_VARIABLE _null
  )

  if (_null)
  else ()
    string(REGEX REPLACE "[\r\n]" " " _result "${_result}")
    string(REGEX REPLACE " +$" ""  _result "${_result}")
    separate_arguments(_result)
    set(${_outvar} ${_result} CACHE INTERNAL "")
  endif ()
endmacro ()

find_package(PkgConfig)
if (PkgConfig_FOUND)
  if (IsoCodes_MIN_VERSION)
    pkg_check_modules(_pc_ISOCODES iso-codes>=${IsoCodes_MIN_VERSION})
  else ()
    pkg_check_modules(_pc_ISOCODES iso-codes)
  endif ()
  if (_pc_ISOCODES_FOUND)
    _isocodes_get_pkgconfig_var(IsoCodes_DOMAINS "domains")
  endif ()
endif ()

include(FindPackageHandleStandardArgs)
#find_package_handle_standard_args(IsoCodes DEFAULT_MSG IsoCodes_DOMAINS)
find_package_handle_standard_args(IsoCodes REQUIRED_VARS _pc_ISOCODES_FOUND IsoCodes_DOMAINS VERSION_VAR _pc_ISOCODES_VERSION)

mark_as_advanced(IsoCodes_DOMAINS)
