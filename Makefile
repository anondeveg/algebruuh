build:
	g++ -g3 -O0 -Wall -Wextra -ggdb3 main.cpp lexer.cpp parser.cpp -o algebruuh 
test: build
	./algebruuh -d -v examples/math.bruh
