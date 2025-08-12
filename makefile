# paths
BUILD = build
SRC = src
HEADERS = headers

# macros
CC = gcc
CFLAGS = -ansi -Wall -pedantic -g
BIN = bin
GLOBAL_DEPS = $(HEADERS)/globals.h
EXE_DEPS = $(BUILD)/operations.o $(BUILD)/utils.o $(BUILD)/assembler.o $(BUILD)/preProcessor.o $(BUILD)/firstRun.o  $(BUILD)/errors.o $(BUILD)/labels.o $(BUILD)/tables.o

# main target
assembler: $(EXE_DEPS) $(GLOBAL_DEPS)
	$(CC) $(EXE_DEPS) $(CFLAGS) -g -o $(BIN)/$@

$(BUILD)/assembler.o: $(SRC)/assembler.c  $(HEADERS)/assembler.h $(GLOBAL_DEPS)
	$(CC) -c $(SRC)/assembler.c $(CFLAGS) -o $@

$(BUILD)/utils.o: $(SRC)/utils.c $(HEADERS)/utils.h $(GLOBAL_DEPS)
	$(CC) -c $(SRC)/utils.c $(CFLAGS) -o $@

$(BUILD)/operations.o: $(SRC)/operations.c $(HEADERS)/operations.h $(GLOBAL_DEPS)
	$(CC) -c $(SRC)/operations.c $(CFLAGS) -o $@

$(BUILD)/errors.o: $(SRC)/errors.c $(HEADERS)/errors.h $(GLOBAL_DEPS)
	$(CC) -c $(SRC)/errors.c $(CFLAGS) -o $@

$(BUILD)/preProcessor.o: $(SRC)/preProcessor.c $(HEADERS)/preProcessor.h $(GLOBAL_DEPS)
	$(CC) -c $(SRC)/preProcessor.c $(CFLAGS) -o $@

$(BUILD)/firstRun.o: $(SRC)/firstRun.c $(HEADERS)/firstRun.h $(GLOBAL_DEPS)
	$(CC) -c $(SRC)/firstRun.c $(CFLAGS) -o $@

$(BUILD)/labels.o: $(SRC)/labels.c $(HEADERS)/labels.h $(GLOBAL_DEPS)
	$(CC) -c $(SRC)/labels.c $(CFLAGS) -o $@

$(BUILD)/tables.o: $(SRC)/tables.c $(HEADERS)/tables.h $(GLOBAL_DEPS)
	$(CC) -c $(SRC)/tables.c $(CFLAGS) -o $@

clean:
	rm -rf $(BUILD)/*.o $(BIN)/assembler

.PHONY: clean
