SRC=src
TARGET=target
VPATH=$(SRC)

LEX=flex
YACC=bison
MKDIR=mkdir
RM=rm

all: $(TARGET)/ocbl

$(TARGET)/ocbl: $(TARGET)/lexer.o $(TARGET)/parser.o
	$(CXX) -o $@ $?

$(TARGET):
	$(MKDIR) $(TARGET)

$(TARGET)/lexer.cc: lexer.l $(TARGET)/parser.hh
	$(LEX) -o $@ $<

$(TARGET)/parser.cc: parser.y $(TARGET)
	$(YACC) -d -o $@ $<

$(TARGET)/parser.hh: $(TARGET)/parser.cc

.PHONY: clean
clean:
	$(RM) -rf $(TARGET)
