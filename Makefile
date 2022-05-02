# directories
SRC=src
SPEC=test
TARGET=target
BIN=$(TARGET)/bin
GEN=$(TARGET)/generated
TEST=$(TARGET)/test

# tools
LEX=flex
YACC=bison
MKDIR=mkdir
RM=rm

$(BIN)/%.o: $(SRC)/%.cc $(BIN)
	$(CXX) -std=c++17 -I$(SRC) -I$(GEN) -c -o $@ $<

$(BIN)/%.o: $(GEN)/%.cc $(BIN)
	$(CXX) -std=c++17 -I$(SRC) -I$(GEN) -c -o $@ $<

$(TEST)/%.o: $(SPEC)/%.cc $(TEST)
	$(CXX) -std=c++17 -I$(SPEC) -I$(SRC) -I$(GEN) -c -o $@ $<

all: test $(TARGET)/driver

test: $(TEST)/test
	$(TEST)/test

$(TARGET)/driver: $(BIN)/driver.o $(BIN)/scanner.o $(BIN)/parser.o
	$(CXX) -o $@ $^

$(TEST)/test: $(TEST)/altea.o $(TEST)/altea-poc1.spec.o $(TEST)/altea-poc2.spec.o
	$(CXX) -o $@ $^

$(GEN):
	$(MKDIR) -p $@

$(BIN):
	$(MKDIR) -p $@

$(TEST):
	$(MKDIR) -p $@

$(BIN)/driver.o: $(GEN)/parser.hh

$(BIN)/scanner.o: $(GEN)/parser.hh

$(GEN)/scanner.cc: $(SRC)/scanner.l $(GEN)
	$(LEX) -o $@ $<

$(GEN)/parser.cc: $(SRC)/parser.y $(GEN)
	$(YACC) -d -o $@ $<

$(GEN)/parser.hh: $(GEN)/parser.cc

.PHONY: clean
clean:
	$(RM) -rf $(TARGET)
