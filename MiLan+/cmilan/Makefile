#CFLAGS	= -Wall -W -Werror -O2
#LDFLAGS	=
#
#HEADERS	= scanner.h \
#	  parser.h \
#	  codegen.h \
#	  tokens.h
#
#OBJS	= main.o \
#	  codegen.o \
#	  scanner.o \
#	  parser.o \
#	  tokens.o
#	  
#EXE	= cmilan
#
#$(EXE): $(OBJS) $(HEADERS)
#	$(CXX) $(LDFLAGS) -o $@ $(OBJS)
#
#.cpp.o:
#	$(CXX) $(CFLAGS) -c $< -o $@
#
#clean:
#	-@rm -f $(EXE) $(OBJS)

milan:	codegen.cpp codegen.h enum_map.cpp main.cpp parser.cpp parser.h scanner.cpp scanner.h tokens.h tokens.cpp
	gcc -Wall -o milan codegen.cpp main.cpp parser.cpp scanner.cpp tokens.cpp

clean:
	rm parser.h parser.cpp
	
distclean:
	rm milan parser.cpp
