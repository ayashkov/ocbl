# directories
SRC=src
TARGET=target
VPATH=$(SRC)

# tools
LEX=flex
YACC=bison
MKDIR=mkdir
RM=rm

$(TARGET)/%.o: %.cc $(TARGET)
	$(CXX) -I$(SRC) -I$(TARGET) -c -o $@ $<

$(TARGET)/%.o: $(TARGET)/%.cc
	$(CXX) -I$(SRC) -I$(TARGET) -c -o $@ $<

all: $(TARGET)/driver

$(TARGET)/driver: $(TARGET)/driver.o $(TARGET)/scanner.o $(TARGET)/parser.o
	$(CXX) -o $@ $^

$(TARGET):
	$(MKDIR) $(TARGET)

$(TARGET)/driver.o: $(TARGET)/parser.hh

$(TARGET)/scanner.o: $(TARGET)/parser.hh

$(TARGET)/scanner.cc: scanner.l $(TARGET)
	$(LEX) -o $@ $<

$(TARGET)/parser.cc: parser.y $(TARGET)
	$(YACC) -d -o $@ $<

$(TARGET)/parser.hh: $(TARGET)/parser.cc

.PHONY: clean
clean:
	$(RM) -rf $(TARGET)
