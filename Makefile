build:
	g++ -g3 -O0 -Wall -Wextra -ggdb3 -std=c++20 main.cpp lexer.cpp parser.cpp -o algebruuh 
test: build
	./algebruuh -d -v -std=c++20 examples/math.bruh
