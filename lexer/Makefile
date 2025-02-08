CFLAGS ?= -W -Wall -Wextra -Wundef -Wshadow -Wdouble-promotion \
		  -Wformat-truncation -fno-common -Wconversion

SOURCEDIR := src
SOURCES := $(shell find $(SOURCEDIR) -name '*.c')
LEXER := $(shell find $(SOURCEDIR) -name '*.l')

INCDIR := inc
INCLUDES := $(addprefix -I, ./$(INCDIR))

TARGET := jacc_lexer

$(TARGET):
	@flex -o $(SOURCEDIR)/$@.c $(LEXER)
	@gcc $(INCLUDES) $(CFLAGS) $(SOURCES) -o $@

.PHONY: clean
clean:
	@rm $(TARGET)