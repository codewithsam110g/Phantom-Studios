cr:
	g++ -g -std=c++17 *.c *.cpp source/*.cpp source/imgui/*.cpp -o main -I include -I "include/imgui" -lglfw3 -lGL
	./main
