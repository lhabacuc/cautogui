#******************#
# == SF LIEDSON == #
#******************#

CXX = g++
CC = gcc
CXXFLAGS = -std=c++11 -Wall 
CFLAGS = -std=c11 -Wall 
LDFLAGS_CPP = -lX11 -lXtst 
LDFLAGS_C = -lX11 -lXtst

SHELL := /bin/bash

PROFILE := $(shell \
	if [ -n "$$ZSH_VERSION" ]; then echo "$(HOME)/.zshrc"; \
	elif [ -n "$$BASH_VERSION" ]; then echo "$(HOME)/.bashrc"; \
	elif [ -f "$(HOME)/.zshrc" ]; then echo "$(HOME)/.zshrc"; \
	elif [ -f "$(HOME)/.bashrc" ]; then echo "$(HOME)/.bashrc"; \
	else echo "$(HOME)/.profile"; fi )

PREFIX ?= /usr/local
LIBDIR ?= $(PREFIX)/lib
INCLUDEDIR ?= $(PREFIX)/include

all: libcautogui_cpp.a libcautogui_c.a test_cpp test_c

libcautogui_cpp.a: cautogui.o
	ar rcs $@ $^

cautogui.o: cautogui.cpp cautogui.hpp
	$(CXX) $(CXXFLAGS) -c cautogui.cpp -o $@

libcautogui_c.a: cautogui_c.o
	ar rcs $@ $^

cautogui_c.o: cautogui.c cautogui.h
	$(CC) $(CFLAGS) -c cautogui.c -o $@

setup-env:
	@echo ">> Adicionando variáveis de ambiente em $(PROFILE)"
	@echo '' >> $(PROFILE)
	@echo '# ===== Configuração libcautogui =====' >> $(PROFILE)
	@echo 'export C_INCLUDE_PATH=$$HOME/.local/include:$$C_INCLUDE_PATH' >> $(PROFILE)
	@echo 'export CPLUS_INCLUDE_PATH=$$HOME/.local/include:$$CPLUS_INCLUDE_PATH' >> $(PROFILE)/.zshrc
	@echo 'export LIBRARY_PATH=$$HOME/.local/lib:$$LIBRARY_PATH' >> $(PROFILE)
	@echo 'export LD_LIBRARY_PATH=$$HOME/.local/lib:$$LD_LIBRARY_PATH' >> $(PROFILE)
	@echo '# ====================================' >> $(PROFILE)
	@echo ">> Agora rode: source $(PROFILE)"
	
test_cpp: main.cpp libcautogui_cpp.a
	$(CXX) $(CXXFLAGS) main.cpp libcautogui_cpp.a $(LDFLAGS_CPP) -o $@

test_c: main.c libcautogui_c.a
	$(CC) $(CFLAGS) main.c libcautogui_c.a $(LDFLAGS_C) -o $@

install-user: all
	mkdir -p $(HOME)/.local/lib
	mkdir -p $(HOME)/.local/include
	cp libcautogui_c.a $(HOME)/.local/lib/
	cp libcautogui_cpp.a $(HOME)/.local/lib/
	cp cautogui.h cautogui.hpp $(HOME)/.local/include/
	setup-env

install: all
	mkdir -p $(DESTDIR)$(LIBDIR)
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp libcautogui_c.a $(DESTDIR)$(LIBDIR)/
	cp libcautogui_cpp.a $(DESTDIR)$(LIBDIR)/
	cp cautogui.h cautogui.hpp $(DESTDIR)$(INCLUDEDIR)/
	setup-env

clean:
	rm -f *.o *.a test_cpp test_c
	
	
	
	

