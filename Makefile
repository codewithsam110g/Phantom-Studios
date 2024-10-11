cr:
	g++ -g -std=c++17 *.c *.cpp source/*.cpp -o main -I include -lglfw3 -lGL
	./main
