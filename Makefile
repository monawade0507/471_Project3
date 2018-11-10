#
# Makefile
# Programming Assignment #3
#
CXX = g++
LD = g++
CXXFLAGS = -g -pthread -std=c++11
LDFLAGS = -g -pthread

#
# Any libraries we might need.
#
LIBRARYS = -lpthread

web_server : web_server.o web_server_help.o log.o
	${LD} ${LDFLAGS} web_server.o web_server_help.o log.o -o $@ ${LIBRARYS}

web_server.o : web_server.cc web_server.h
	${CXX} -c ${CXXFLAGS} -o $@ $<

web_server_help.o : web_server_help.cc web_server_help.h log.cc log.h
	${CXX} -c ${CXXFLAGS} -o $@ $<

log.o : log.cc log.h
		${CXX} -c ${CXXFLAGS} -o $@ $<

#
# Please remember not to submit objects or binarys.
#
clean:
	rm -f core web_server.o web_server_help.o log.o web_server

#
# This might work to create the submission tarball in the formal I asked for.
#
submit:
		rm -f core project3 web_server.o web_server_help.o log.o web_server
		mkdir `whoami`
		cp Makefile README.txt *.h *.cc `whoami`
	  tar cf `whoami`.tar `whoami`
