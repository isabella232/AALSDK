# INTEL CONFIDENTIAL - For Intel Internal Use Only

# valapps/RAS/Makefile

CPPFLAGS ?=
CXX      ?= g++
LDFLAGS  ?= -L../vallib -L../valapp

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

all: RAS

RAS: main.o
	$(CXX) -g -O2 -o RAS main.o $(LDFLAGS) -lOSAL -lAAS -laalrt -lvallib -lvalutils

main.o: main.cpp ../vallib/vallib.h Makefile
	$(CXX) $(CPPFLAGS) -D__AAL_USER__=1 -I../vallib -I../valapp -g -O2 -c -o main.o main.cpp

clean:
	$(RM) RAS main.o

.PHONY:all clean
