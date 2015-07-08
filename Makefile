#
# Copyright 2014 WonderMedia Technologies, Inc. All Rights Reserved.
#
# This PROPRIETARY SOFTWARE is the property of WonderMedia Technologies, Inc.
# and may contain trade secrets and/or other confidential information of
# WonderMedia Technologies, Inc. This file shall not be disclosed to any third party,
# in whole or in part, without prior written consent of WonderMedia.
#
# THIS PROPRIETARY SOFTWARE AND ANY RELATED DOCUMENTATION ARE PROVIDED AS IS,
# WITH ALL FAULTS, AND WITHOUT WARRANTY OF ANY KIND EITHER EXPRESS OR IMPLIED,
# AND WonderMedia TECHNOLOGIES, INC. DISCLAIMS ALL EXPRESS OR IMPLIED WARRANTIES
# OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
#

# Directories:
CUR_DIR         = .

# Sources:
SRC_CUR         = $(wildcard $(CUR_DIR)/*.cpp) $(wildcard $(CUR_DIR)/*.c)

### set variables before include make.rules
debug ?= 1
quiet ?= 1

out_executable  = ColorWeight
include_dirs    = $(CUR_DIR)
cpp_sources     = $(SRC_CUR)
link_libs       = opencv_highgui opencv_core

CFLAGS         += -Wno-deprecated -g -std=c99

include make.rules
