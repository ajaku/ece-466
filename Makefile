TARGET := jacc

FLEX_TARGET := $(TARGET)_lexer
BISON_TARGET := $(TARGET)_parser.tab

CFLAGS ?= -g3 -W -Wall -Wextra -Wundef -Wshadow -Wdouble-promotion \
          -Wformat-truncation -fno-common -Wconversion

BUILDDIR := build

SOURCEDIR := src
SOURCES := $(shell find $(SOURCEDIR) -name '*.c')
SOURCES_ONLY := $(basename $(notdir $(SOURCES)))

LEXER := $(shell find $(SOURCEDIR) -name '*.l')
PARSER := $(shell find $(SOURCEDIR) -name '*.y')

INCDIR := inc
INCDIR += build
INCLUDES := $(addprefix -I, ./$(INCDIR))

BISON_DEP := $(BUILDDIR)/$(BISON_TARGET).c
FLEX_DEP += $(BUILDDIR)/$(FLEX_TARGET).c

OBJ := $(BUILDDIR)/$(SOURCES_ONLY).o

all: $(TARGET) 

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(BUILDDIR)/$(BISON_TARGET).c: $(PARSER) $(BUILDDIR)
	bison -d -o $@ $<

$(BUILDDIR)/$(FLEX_TARGET).c: $(LEXER) $(BUILDDIR)
	flex -o $@ $<

$(TARGET): $(BISON_DEP) $(FLEX_DEP)
	gcc $(INCLUDES) $(CFLAGS) -c $(SOURCEDIR)/$(SOURCES_ONLY).c -o $(BUILDDIR)/$(SOURCES_ONLY).o
	gcc -D PARSER $(INCLUDES) $(CFLAGS) $^ $(OBJ) -o $@


run: $(TARGET)
	./$(TARGET)

debug: $(TARGET)
	gdb -tui ./$(TARGET)


.PHONY: clean
clean:
	@rm -rf $(TARGET) $(BUILDDIR) $(SOURCEDIR)/$(BISON_TARGET).c $(SOURCEDIR)/$(FLEX_TARGET).c $(SOURCEDIR)/$(BISON_TARGET).h $(SOURCEDIR)/$(TARGET)_parser.output

