CC=gcc
LD=gcc
#CFLAGS=-Wall -g -ggdb -I.
CFLAGS=-Wall -O3 -I.
LDFLAGS = -lm -s
# Do debug with '-s' : strip binary
HEADERS=$(wildcard *.h)

MKDIR_P=mkdir -p
OBJDIR=builds/makefile

# -------------

# from help :http://stackoverflow.com/questions/231229/how-to-generate-a-makefile-with-source-in-sub-directories-using-just-one-makefil
MODULES   := generation lexical syntaxical util
SRC_DIR   := $(addprefix ,$(MODULES))
BUILD_DIR := $(addprefix $(OBJDIR)/,$(MODULES))
SRC       := $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.c))
OBJ       := $(patsubst %.c,$(OBJDIR)/%.o,$(SRC))

# -------------

all: compilo

compilo: $(OBJDIR)/compilo.o $(OBJ)
	$(info ) 
	$(info Linking now)
	$(LD) -o $@ $^ $(LDFLAGS) $(CFLAGS)

# -------------

clean:
	rm -R $(OBJDIR)
	rm compilo

# -------------

$(OBJDIR):
	$(MKDIR_P) $(BUILD_DIR)

# -------------

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJDIR)/generation/%.o: generation/%.c generation/%.h  | $(OBJDIR)
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJDIR)/lexical/%.o: lexical/%.c lexical/%.h | $(OBJDIR)
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJDIR)/syntaxical/%.o: syntaxical/%.c syntaxical/%.h | $(OBJDIR)
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJDIR)/util/%.o: util/%.c util/%.h | $(OBJDIR)
	$(CC) $(CFLAGS) -o $@ -c $<


