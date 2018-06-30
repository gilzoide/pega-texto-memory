# Find Judy headers and libraries.
# 
# The following variables are defined:
#  JUDY_FOUND        - True if Judy was found
#  JUDY_INCLUDE_DIRS - Judy include direcories
#  JUDY_LIBRARIES    - Judy libraries to link

find_path(JUDY_INCLUDE_DIRS Judy.h)
find_library(JUDY_LIBRARIES NAMES Judy judy)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Judy DEFAULT_MSG JUDY_LIBRARIES JUDY_INCLUDE_DIRS)
