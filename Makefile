CXX = g++
CC = gcc
CXXFLAGS = -std=c++11 -Wall 
CFLAGS = -std=c11 -Wall 
LDFLAGS_CPP = -lX11 -lXtst 
LDFLAGS_C = -lX11 -lXtst

all: libcautogui_cpp.a libcautogui_c.a test_cpp test_c

libcautogui_cpp.a: cautogui.o
	ar rcs $@ $^

cautogui.o: cautogui.cpp cautogui.hpp
	$(CXX) $(CXXFLAGS) -c cautogui.cpp -o $@

libcautogui_c.a: cautogui_c.o
	ar rcs $@ $^

cautogui_c.o: cautogui.c cautogui.h
	$(CC) $(CFLAGS) -c cautogui.c -o $@

test_cpp: main.cpp libcautogui_cpp.a
	$(CXX) $(CXXFLAGS) main.cpp libcautogui_cpp.a $(LDFLAGS_CPP) -o $@

test_c: main.c libcautogui_c.a
	$(CC) $(CFLAGS) main.c libcautogui_c.a $(LDFLAGS_C) -o $@

clean:
	rm -f *.o *.a test_cpp test_c
