SHELL = /bin/sh
CC    = gcc
FLAGS        = #-std=gnu99 -Iinclude
CFLAGS       = -fPIC -g #-pedantic -Wall -Wextra -march=native -ggdb3
LDFLAGS      = -shared

TARGET  = example.so
SOURCES = $(shell echo ./*.c)
HEADERS = $(shell echo ./*.h)
OBJECTS = $(SOURCES:.c=.o)
C_INCLUDE_PATH = ../src/
PREFIX = $(DESTDIR)/usr/local
BINDIR = $(PREFIX)/bin

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(FLAGS) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJECTS)

