TARGET := jacc

LEX_TARGET := jacc_gen_lexer
FLEX_TARGET := jacc_gen_parser

CFLAGS ?= -g3 -W -Wall -Wextra -Wundef -Wshadow -Wdouble-promotion \
          -Wformat-truncation -fno-common -Wconversion

BUILDDIR := build

SOURCEDIR := src
SOURCES := $(shell find $(SOURCEDIR) -name '*.c')
LEXER := $(shell find $(SOURCEDIR) -name '*.l')
PARSER := $(shell find $(SOURCEDIR) -name '*.y')

INCDIR := inc
INCLUDES := $(addprefix -I, ./$(INCDIR))

OBJ := $(patsubst $(SOURCEDIR)/%.c, $(BUILDDIR)/%.o, $(SOURCES))
OBJ += $(BUILDDIR)/$(LEX_TARGET).o
OBJ += $(BUILDDIR)/$(FLEX_TARGET).o

all: $(TARGET)

$(TARGET): $(OBJ)
	gcc -D PARSER $(INCLUDES) $(CFLAGS) $^ -o $@

$(SOURCEDIR)/$(LEX_TARGET).c: $(LEXER)
	flex -o $@ $<

$(SOURCEDIR)/$(FLEX_TARGET).c: $(PARSER)
	bison -d -o $@ $<

$(BUILDDIR)/$(LEX_TARGET).o: $(SOURCEDIR)/$(LEX_TARGET).c
	gcc $(INCLUDES) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/$(FLEX_TARGET).o: $(SOURCEDIR)/$(FLEX_TARGET).c
	gcc $(INCLUDES) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.c | $(BUILDDIR)
	gcc $(INCLUDES) $(CFLAGS) -c $< -o $@

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

run: $(TARGET)
	./$(TARGET)

debug: $(TARGET)
	gdb -tui ./$(TARGET)

.PHONY: clean
clean:
	@rm -rf $(TARGET) $(BUILDDIR) $(SOURCEDIR)/$(LEX_TARGET).c $(SOURCEDIR)/$(FLEX_TARGET).c $(SOURCEDIR)/$(FLEX_TARGET).h
