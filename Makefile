exe:
	g++ -c -std=c++11 streamer.cpp -o streamer.o
	g++ -c -std=c++11 input.cpp -o input.o
	g++ -c -std=c++11 term.cpp -o term.o
	g++ -c -std=c++11 matcher.cpp -o matcher.o
	g++ -c -std=c++11 fff.cpp -o fff.o

	g++ -o fff fff.o matcher.o term.o input.o streamer.o -lpthread

