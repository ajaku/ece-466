TARGET := jacc_lexer

CFLAGS ?= -g3 -W -Wall -Wextra -Wundef -Wshadow -Wdouble-promotion \
          -Wformat-truncation -fno-common -Wconversion

BUILDDIR := build

SOURCEDIR := src
SOURCES := $(shell find $(SOURCEDIR) -name '*.c')
LEXER := $(shell find $(SOURCEDIR) -name '*.l')

INCDIR := inc
INCLUDES := $(addprefix -I, ./$(INCDIR))

OBJ := $(patsubst $(SOURCEDIR)/%.c, $(BUILDDIR)/%.o, $(SOURCES))
OBJ += $(BUILDDIR)/$(TARGET).o

all: $(TARGET)

$(TARGET): $(OBJ)
	gcc $(INCLUDES) $(CFLAGS) $^ -o $@

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.c | $(BUILDDIR)
	gcc $(INCLUDES) $(CFLAGS) -c $< -o $@

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(SOURCEDIR)/$(TARGET).c: $(LEXER)
	flex -o $@ $<

debug: $(TARGET)
	gdb -tui ./$(TARGET)

.PHONY: clean
clean:
	@rm -rf $(TARGET) $(BUILDDIR) $(SOURCEDIR)/$(TARGET).c
