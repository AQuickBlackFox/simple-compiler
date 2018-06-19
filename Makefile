# Build graph builder

CC=~/llvm/bin/clang
CXX=~/llvm/bin/clang++
CXXFLAGS=-Wno-deprecated-register
all: frontend

frontend: Parser.o Tokens.o Main.o VisitorCheck.o
	${CXX} Tokens.o Parser.o Main.o VisitorCheck.o -o Main

Tokens.o: Tokens.l
	lex -o Tokens.cpp Tokens.l
	${CXX} -c ${CXXFLAGS} Tokens.cpp

Parser.o: Parser.y
	bison -d -o Parser.cpp Parser.y
	${CXX} -c Parser.cpp

Main.o: Main.cpp
	${CXX} -c Main.cpp

VisitorCheck.o: VisitorCheck.cpp
	${CXX} -c VisitorCheck.cpp

clean:
	rm *.o Parser.cpp Parser.hpp Tokens.cpp Main