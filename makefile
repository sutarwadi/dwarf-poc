all: main.clang main.gcc

main.clang: main.cpp
	clang++ -o main.clang main.cpp -g -ldw

main.gcc: main.cpp
	g++ -o main.gcc main.cpp -O0 -g -ldw

clean: main.clang main.gcc
	rm main.gcc main.clang
