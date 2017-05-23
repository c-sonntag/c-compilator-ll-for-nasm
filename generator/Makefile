COMPILO=../compilateur/compilo
COMPILOFLAGS=-n
CC=nasm
LD=ld
CFLAGS=-f elf -g -F dwarf 
LDFLAGS=-m elf_i386


NASMDIR=nasm
BINDIR=bin
OBJDIR=obj
MKDIR_P=mkdir -p



# -------------

SRC       := $(shell find . -maxdepth 1 -name '*.l' -printf '%f\n')
OBJ       := $(patsubst %.l,$(OBJDIR)/%.o,$(SRC))

# -------------

all: directories $(OBJ)

# -------------

clean:
	@rm -R $(OBJDIR) $(BINDIR) $(NASMDIR) 
#@rm $(OBJ) $(basename $(notdir $(OBJ)))
	
	
# ---------------------------------------
# to preserve nasm file with "SECONDARY"
.SECONDARY: 

# -------------
	
directories:
	$(MKDIR_P) $(OBJDIR) $(BINDIR) $(NASMDIR) 
	
# -------------

$(NASMDIR)/%.nasm: %.l
	$(COMPILO) $(COMPILOFLAGS) $< -o $@

$(OBJDIR)/%.o: $(NASMDIR)/%.nasm | directories
	$(CC) $(CFLAGS) $< -o $@
	$(LD) $(LDFLAGS) -o $(BINDIR)/$(basename $(notdir  $@)) $@
