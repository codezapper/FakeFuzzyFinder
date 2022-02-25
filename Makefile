CC=g++ -c -std=c++11 -I./inc
LD=g++

all: fff

obj/fullreader.o: src/fullreader.cpp inc/fullreader.h
	@mkdir -p obj
	$(CC) src/fullreader.cpp -o obj/fullreader.o

obj/streamer.o: src/streamer.cpp inc/streamer.h
	@mkdir -p obj
	$(CC) src/streamer.cpp -o obj/streamer.o

obj/input.o: src/input.cpp inc/input.h
	@mkdir -p obj
	$(CC) src/input.cpp -o obj/input.o

obj/term.o: src/term.cpp inc/term.h
	@mkdir -p obj
	$(CC) src/term.cpp -o obj/term.o

obj/matcher.o: src/matcher.cpp inc/matcher.h
	@mkdir -p obj
	$(CC) src/matcher.cpp -o obj/matcher.o

obj/fff.o: src/fff.cpp
	@mkdir -p obj
	$(CC) src/fff.cpp -o obj/fff.o

fff: obj/fullreader.o obj/streamer.o obj/input.o obj/term.o obj/matcher.o obj/fff.o
	$(LD) -O3 -o fff obj/fff.o obj/matcher.o obj/term.o obj/input.o obj/streamer.o obj/fullreader.o -lpthread

clean:
	rm -f obj/fullreader.o obj/streamer.o obj/input.o obj/term.o obj/matcher.o obj/fff.o fff

