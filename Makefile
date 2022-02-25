
CC=g++ -c -std=c++11 -I./inc

exe:
	mkdir -p obj
	$(CC) src/fullreader.cpp -o obj/fullreader.o
	$(CC) src/streamer.cpp -o obj/streamer.o
	$(CC) src/input.cpp -o obj/input.o
	$(CC) src/term.cpp -o obj/term.o
	$(CC) src/matcher.cpp -o obj/matcher.o
	$(CC) src/fff.cpp -o obj/fff.o

	g++ -O3 -o fff obj/fff.o obj/matcher.o obj/term.o obj/input.o obj/streamer.o obj/fullreader.o -lpthread

clean:
	rm -f obj/fullreader.o obj/streamer.o obj/input.o obj/term.o obj/matcher.o obj/fff.o fff

