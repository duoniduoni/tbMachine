main.o: main.cpp
	g++ -c main.cpp -o main.o

run_cmd.o: run_cmd.cpp
	g++ -c run_cmd.cpp -o run_cmd.o

all: main.o run_cmd.o
	g++ main.o run_cmd.o -o tool

clean:
	rm *.o tool
