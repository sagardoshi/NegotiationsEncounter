# Compiler, Linker, and Target Executable
CXX 		:= g++
TARGET 		:= game
MAKE		:= make
CLEAN		:= clean
EXECUTE		:= run
GO			:= go

# Directories
SRCDIR		:= src
INCDIR		:= inc
OBJDIR		:= obj
TARGETDIR	:= bin

# Extensions
SRCEXT		:= cpp
DEPEXT		:= d
OBJEXT		:= o

# Flags
CXXFLAGS 	:= -Wall -g -std=c++11

# Objects
_OBJ 		:= main.o \
			   GameMaster.o \
			   Encounter.o \
			   Inventory.o \
			   Negotiator.o \
			   PlayerCharacter.o \

OBJ			:= $(patsubst %,$(OBJDIR)/%,$(_OBJ))

# Rules
$(TARGETDIR)/$(TARGET): $(OBJ)
	@$(CXX) $(OBJ) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(GO):
	@$(MAKE) $(CLEAN)
	@$(MAKE)
	@$(MAKE) $(EXECUTE)

$(EXECUTE):
	@$(TARGETDIR)/$(TARGET)

.PHONY: $(CLEAN)
$(CLEAN):
	@rm -f $(OBJDIR)/*.o *.gch $(TARGETDIR)/$(TARGET)
