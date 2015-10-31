MYSQL_ROOT=/root/rpmbuild/SOURCES/mysql-cluster-gpl-7.4.7
NDB_ROOT=$(MYSQL_ROOT)/storage/ndb
CPLUS_INCLUDE_PATH=$(MYSQL_ROOT)/include:$(NDB_ROOT)/include:$(NDB_ROOT)/include/portlib:src
WFLAGS=-Wall -Wextra -Wformat-security -Wvla -Woverloaded-virtual -Wunused -Wno-unused-parameter
#DFLAGS=-DDBUG_OFF -D_DEBUG
DFLAGS=-DDBUG_OFF
#CXXFLAGS=-c -std=c++11 -O3 -g -fno-omit-frame-pointer $(INCLUDE) $(WFLAGS) $(DFLAGS)
CXXFLAGS=-c -std=c++11 -O3 -fno-omit-frame-pointer $(INCLUDE) $(WFLAGS) $(DFLAGS)
LDFLAGS+=-lpthread
#LDFLAGS+=-L$(NDB_ROOT)/src
#LDFLAGS+=-L$(MYSQL_ROOT)/libmysql
#LDFLAGS+=-lndbclient_static
#LDFLAGS+=-lmysqlclient

export CPLUS_INCLUDE_PATH

VPATH=src:src/util:src/ndb:src/user:obj

HPPS=$(wildcard src/*.hpp src/util/*.hpp src/ndb/*.hpp src/ndb/*.inl src/user/*.hpp)
CPPS=$(wildcard src/*.cpp src/util/*.cpp src/ndb/*.cpp src/user/*.cpp)
OBJS=$(addprefix obj/, $(notdir $(CPPS:.cpp=.o)))
EXEC=ndbAsynch

NDBLIBS+=$(NDB_ROOT)/src/libndbclient_static.a
NDBLIBS+=$(MYSQL_ROOT)/libmysql/libmysqlclient.a

#NDBLIBS+=$(NDB_ROOT)/src/ndbapi/libndbapi.a
#NDBLIBS+=$(NDB_ROOT)/src/mgmapi/libndbmgmapi.a
#NDBLIBS+=$(NDB_ROOT)/src/mgmsrv/libndbconf.a

#NDBLIBS+=$(NDB_ROOT)/src/common/transporter/libndbtransport.a
#NDBLIBS+=$(NDB_ROOT)/src/common/logger/libndblogger.a
#NDBLIBS+=$(NDB_ROOT)/src/common/util/libndbgeneral.a
#NDBLIBS+=$(NDB_ROOT)/src/common/debugger/libndbtrace.a
#NDBLIBS+=$(NDB_ROOT)/src/common/portlib/libndbportlib.a
#NDBLIBS+=$(NDB_ROOT)/src/common/mgmcommon/libndbmgmcommon.a
#NDBLIBS+=$(NDB_ROOT)/src/common/debugger/signaldata/libndbsignaldata.a

#NDBLIBS+=$(MYSQL_ROOT)/mysys_ssl/libmysys_ssl.a
#NDBLIBS+=$(MYSQL_ROOT)/mysys/libmysys.a
#NDBLIBS+=$(MYSQL_ROOT)/extra/yassl/taocrypt/libtaocrypt.a
#NDBLIBS+=$(MYSQL_ROOT)/strings/libstrings.a



all: $(EXEC)

$(EXEC): Makefile obj/stdafx.hpp.gch $(OBJS)
	@echo "[Link] $(EXEC)"
	@g++ -o $(EXEC) $(OBJS) $(LDFLAGS) $(NDBLIBS)

clean:
	rm -rf $(EXEC) obj

obj/stdafx.hpp.gch: src/stdafx.hpp
	@mkdir -p obj
	@echo "[Build] PreCompiled Header"
	@g++ $(CXXFLAGS) -c $< -o $@

obj/%.o: %.cpp
	@mkdir -p obj
	@echo "[Compile] $<"
	@g++ $(CXXFLAGS) -c -MD -o $@ $<

-include $(OBJS:%.o=%.d)

