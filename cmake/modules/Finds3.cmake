# Copyright (C) 2007-2012 Hypertable, Inc.
#
# This file is part of Hypertable.
#
# Hypertable is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 3
# of the License, or any later version.
#
# Hypertable is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Hypertable. If not, see <http://www.gnu.org/licenses/>
#

# - Find libs3
# Find the s3 library and includes
#
# S3_INCLUDE_DIR - where to find libs3.h, etc.
# S3_LIBRARIES - List of libraries when using s3.
# S3_FOUND - True if s3 found.

find_path(S3_INCLUDE_DIR libs3.h)

find_library(S3_LIBRARIES s3)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(s3 DEFAULT_MSG S3_LIBRARIES S3_INCLUDE_DIR)

mark_as_advanced(S3_LIBRARIES S3_INCLUDE_DIR)
