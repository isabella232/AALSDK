## Copyright(c) 2015-2016, Intel Corporation
##
## Redistribution  and  use  in source  and  binary  forms,  with  or  without
## modification, are permitted provided that the following conditions are met:
##
## * Redistributions of  source code  must retain the  above copyright notice,
##   this list of conditions and the following disclaimer.
## * Redistributions in binary form must reproduce the above copyright notice,
##   this list of conditions and the following disclaimer in the documentation
##   and/or other materials provided with the distribution.
## * Neither the name  of Intel Corporation  nor the names of its contributors
##   may be used to  endorse or promote  products derived  from this  software
##   without specific prior written permission.
##
## THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
## AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,  BUT NOT LIMITED TO,  THE
## IMPLIED WARRANTIES OF  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
## ARE DISCLAIMED.  IN NO EVENT  SHALL THE COPYRIGHT OWNER  OR CONTRIBUTORS BE
## LIABLE  FOR  ANY  DIRECT,  INDIRECT,  INCIDENTAL,  SPECIAL,  EXEMPLARY,  OR
## CONSEQUENTIAL  DAMAGES  (INCLUDING,  BUT  NOT LIMITED  TO,  PROCUREMENT  OF
## SUBSTITUTE GOODS OR SERVICES;  LOSS OF USE,  DATA, OR PROFITS;  OR BUSINESS
## INTERRUPTION)  HOWEVER CAUSED  AND ON ANY THEORY  OF LIABILITY,  WHETHER IN
## CONTRACT,  STRICT LIABILITY,  OR TORT  (INCLUDING NEGLIGENCE  OR OTHERWISE)
## ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,  EVEN IF ADVISED OF THE
## POSSIBILITY OF SUCH DAMAGE.
##****************************************************************************
##  Accelerator Abstraction Layer Library Software Developer Kit (SDK)
##
##  Content:
##     samples/SigTap/Makefile
##  Author:
##     Tim Whisonant, Intel Corporation
##     Joseph Grecco, Intel Corporation
##  History:
##     11/19/2015    JG   Initial version based off of canonical sample makefile
##******************************************************************************
CPPFLAGS ?=
CXX      ?= g++
LDFLAGS  ?=

ifeq (,$(CFLAGS))
CFLAGS = -ggdb3 -O0
endif

ifneq (,$(ndebug))
else
CPPFLAGS += -DENABLE_DEBUG=1
endif
ifneq (,$(nassert))
else
CPPFLAGS += -DENABLE_ASSERT=1
endif

ifeq (,$(DESTDIR))
ifneq (,$(prefix))
CPPFLAGS += -I$(prefix)/include
LDFLAGS  += -L$(prefix)/lib -Wl,-rpath-link -Wl,$(prefix)/lib -Wl,-rpath -Wl,$(prefix)/lib \
            -L$(prefix)/lib64 -Wl,-rpath-link -Wl,$(prefix)/lib64 -Wl,-rpath -Wl,$(prefix)/lib64
endif
else
ifeq (,$(prefix))
prefix = /usr/local
endif
CPPFLAGS += -I$(DESTDIR)$(prefix)/include
LDFLAGS  += -L$(DESTDIR)$(prefix)/lib -Wl,-rpath-link -Wl,$(prefix)/lib -Wl,-rpath -Wl,$(DESTDIR)$(prefix)/lib \
            -L$(DESTDIR)$(prefix)/lib64 -Wl,-rpath-link -Wl,$(prefix)/lib64 -Wl,-rpath -Wl,$(DESTDIR)$(prefix)/lib64
endif

all: mmlink

mmlink: SigTap.o mmlink_server.o mmlink_connection.o mm_debug_link.o
	$(CXX) $(CPPFLAGS) -o mmlink SigTap.o mmlink_server.o mmlink_connection.o mm_debug_link.o $(LDFLAGS) -lOSAL -lAAS -laalrt

SigTap.o: SigTap.cpp  mmlink_server.h mm_debug_link_interface.h Makefile
	$(CXX) $(CPPFLAGS) -D__AAL_USER__=1 $(CFLAGS) -c -o SigTap.o SigTap.cpp

mmlink_connection.o: mmlink_connection.cpp mmlink_connection.h 
	$(CXX) $(CPPFLAGS) -c mmlink_connection.cpp

mm_debug_link.o: mm_debug_link_linux.cpp mm_debug_link_linux.h  
	$(CXX) -I $(KERNEL_SRC_DIR)/include $(CPPFLAGS) -c mm_debug_link_linux.cpp -o mm_debug_link.o
	
mmlink_server.o: mmlink_server.cpp mmlink_server.h mmlink_connection.h mm_debug_link_interface.h 
	$(CXX) $(CPPFLAGS) -c mmlink_server.cpp

clean:
	$(RM) mmlink SigTap.o mmlink_server.o mmlink_connection.o mm_debug_link.o 

.PHONY:all clean
