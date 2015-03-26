C = gcc
CXX = g++
LINKFLAGS = -lSDL2 -lGLEW -lGL -lSDL2_image -lSDL2_ttf

OBJDIR = obj
DEPDIR = dep

CFLAGS = -I..
CXXFLAGS = -I.. -std=c++11

HEADERS = $(shell ./getheader.sh)
SOURCES = $(shell ./getsource.sh)
OBJ = $(SOURCES:%=$(OBJDIR)/%.o)
DEP = $(SOURCES:%=$(DEPDIR)/%.d)
BIN = run

all: $(DEP) $(BIN)

$(BIN): $(OBJ)
	$(CXX) $(LINKFLAGS) $^ -o $@

$(DEPDIR)/%.cpp.d: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -MM $< -MT $(OBJDIR)/$<.o > $@
	echo '	$$(CXX) $$(CXXFLAGS) -c $$< -o $$@' >> $@
	
$(DEPDIR)/%.c.d: %.c $(HEADERS)
	$(C) $(CFLAGS) -MM $< -MT $(OBJDIR)/$<.o > $@
	echo '	$$(C) $$(CFLAGS) -c $$< -o $$@' >> $@

-include $(DEP)
