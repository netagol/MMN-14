#paths
BUILD = ../build
SRC = ../src
HEADERS = ../headers

#macros
CC = gcc
CFLAGS = -ansi -Wall -pedantic
GLOBAL_DEPS = $(HEADERS)/globals.h
EXE_DEPS = $(BUILD)/operations.o $(BUILD)/preProcessor.o $(BUILD)/utils.o $(BUILD)/tests.o $(BUILD)/errors.o $(BUILD)/labels.o 

#executables
tests: $(EXE_DEPS) $(GLOBAL_DEPS)
	$(CC) $(EXE_DEPS) $(CFLAGS) -o $(BUILD)/$@

$(BUILD)/tests.o: tests.c $(GLOBAL_DEPS)
	$(CC) -c tests.c $(CFLAGS) -o $@

$(BUILD)/utils.o: $(SRC)/utils.c $(HEADERS)/utils.h $(GLOBAL_DEPS)
	$(CC) -c $(SRC)/utils.c $(CFLAGS) -o $@

$(BUILD)/operations.o: $(SRC)/operations.c $(HEADERS)/operations.h $(GLOBAL_DEPS)
	$(CC) -c $(SRC)/operations.c $(CFLAGS) -o $@

$(BUILD)/errors.o: $(SRC)/errors.c $(HEADERS)/errors.h $(GLOBAL_DEPS)
	$(CC) -c $(SRC)/errors.c $(CFLAGS) -o $@

$(BUILD)/preProcessor.o: $(SRC)/preProcessor.c $(HEADERS)/preProcessor.h $(GLOBAL_DEPS)
	$(CC) -c $(SRC)/preProcessor.c $(CFLAGS) -o $@

$(BUILD)/labels.o: $(SRC)/labels.c $(HEADERS)/labels.h $(GLOBAL_DEPS)
	$(CC) -c $(SRC)/labels.c $(CFLAGS) -o $@

clean:
	rm -rf $(BUILD)/*.o
	