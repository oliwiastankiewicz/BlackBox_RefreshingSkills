CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Iblackbox_c/include

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -Iblackbox_cpp/include -Iblackbox_c/include


BUILD = blackbox_c/build
TEST_BUILD = blackbox_c/build/tests

CPP_BUILD = blackbox_cpp/build
CPP_TEST_BUILD = blackbox_cpp/build/tests

TARGET = $(BUILD)/blackbox.exe

# ---------------------------------------------------------
# Main program object files
# ---------------------------------------------------------

OBJECTS = \
	$(BUILD)/main.o \
	$(BUILD)/event.o \
	$(BUILD)/queue.o \
	$(BUILD)/timestamp.o \
	$(BUILD)/entity.o \
	$(BUILD)/scenario.o \
	$(BUILD)/simulation.o \
	$(BUILD)/logger.o

# ---------------------------------------------------------
# Test executables
# ---------------------------------------------------------

TEST_TARGETS = \
	$(TEST_BUILD)/test_event.exe \
	$(TEST_BUILD)/test_queue.exe \
	$(TEST_BUILD)/test_timestamp.exe \
	$(TEST_BUILD)/test_entity.exe \
	$(TEST_BUILD)/test_scenario.exe \
	$(TEST_BUILD)/test_simulation.exe \
	$(TEST_BUILD)/test_logger.exe

CPP_TEST_TARGETS = $(CPP_TEST_BUILD)/test_serializer.exe


# ---------------------------------------------------------
# Phony targets
# ---------------------------------------------------------

.PHONY: all run test cpp-test all-tests clean

# ---------------------------------------------------------
# Default target
# ---------------------------------------------------------

all: $(TARGET)

# ---------------------------------------------------------
# Main program
# ---------------------------------------------------------

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

# ---------------------------------------------------------
# Main object files
# ---------------------------------------------------------

$(BUILD)/main.o: blackbox_c/src/main.c \
                 blackbox_c/include/entity.h \
                 blackbox_c/include/event.h \
                 blackbox_c/include/queue.h \
                 blackbox_c/include/timestamp.h \
                 blackbox_c/include/scenario.h \
                 blackbox_c/include/simulation.h \
                 blackbox_c/include/logger.h
	$(CC) $(CFLAGS) -c blackbox_c/src/main.c -o $(BUILD)/main.o

$(BUILD)/event.o: blackbox_c/src/event.c \
                  blackbox_c/include/event.h \
                  blackbox_c/include/entity_base.h \
                  blackbox_c/include/timestamp.h
	$(CC) $(CFLAGS) -c blackbox_c/src/event.c -o $(BUILD)/event.o

$(BUILD)/queue.o: blackbox_c/src/queue.c \
                  blackbox_c/include/queue.h \
                  blackbox_c/include/event.h
	$(CC) $(CFLAGS) -c blackbox_c/src/queue.c -o $(BUILD)/queue.o

$(BUILD)/timestamp.o: blackbox_c/src/timestamp.c \
                      blackbox_c/include/timestamp.h
	$(CC) $(CFLAGS) -c blackbox_c/src/timestamp.c -o $(BUILD)/timestamp.o

$(BUILD)/entity.o: blackbox_c/src/entity.c \
                   blackbox_c/include/entity.h \
                   blackbox_c/include/entity_base.h \
                   blackbox_c/include/event.h \
                   blackbox_c/include/timestamp.h
	$(CC) $(CFLAGS) -c blackbox_c/src/entity.c -o $(BUILD)/entity.o

$(BUILD)/scenario.o: blackbox_c/src/scenario.c \
                     blackbox_c/include/scenario.h \
                     blackbox_c/include/entity.h \
                     blackbox_c/include/queue.h \
                     blackbox_c/include/timestamp.h
	$(CC) $(CFLAGS) -c blackbox_c/src/scenario.c -o $(BUILD)/scenario.o

$(BUILD)/simulation.o: blackbox_c/src/simulation.c \
                       blackbox_c/include/simulation.h \
                       blackbox_c/include/scenario.h \
                       blackbox_c/include/timestamp.h \
                       blackbox_c/include/queue.h \
                       blackbox_c/include/entity.h \
                       blackbox_c/include/event.h \
                       blackbox_c/include/entity_base.h
	$(CC) $(CFLAGS) -c blackbox_c/src/simulation.c -o $(BUILD)/simulation.o

$(BUILD)/logger.o: blackbox_c/src/logger.c \
                   blackbox_c/include/logger.h
	$(CC) $(CFLAGS) -c blackbox_c/src/logger.c -o $(BUILD)/logger.o


$(CPP_BUILD)/serializer.o: blackbox_cpp/src/serializer.cpp \
                           blackbox_cpp/include/serializer.h \
                           blackbox_c/include/event.h
	if not exist "$(CPP_BUILD)" mkdir "$(CPP_BUILD)"
	$(CXX) $(CXXFLAGS) -c blackbox_cpp/src/serializer.cpp -o $(CPP_BUILD)/serializer.o

# ---------------------------------------------------------
# Test directory
# ---------------------------------------------------------

$(TEST_BUILD):
	if not exist "$(TEST_BUILD)" mkdir "$(TEST_BUILD)"

# ---------------------------------------------------------
# Event tests
# ---------------------------------------------------------

$(TEST_BUILD)/test_event.exe: blackbox_c/tests/test_event.c \
                              $(BUILD)/event.o \
                              $(BUILD)/timestamp.o \
                              $(BUILD)/logger.o \
                              | $(TEST_BUILD)
	$(CC) $(CFLAGS) blackbox_c/tests/test_event.c \
		$(BUILD)/event.o \
		$(BUILD)/timestamp.o \
		$(BUILD)/logger.o \
		-o $(TEST_BUILD)/test_event.exe

# ---------------------------------------------------------
# Queue tests
# ---------------------------------------------------------

$(TEST_BUILD)/test_queue.exe: blackbox_c/tests/test_queue.c \
                              $(BUILD)/queue.o \
                              $(BUILD)/event.o \
                              $(BUILD)/timestamp.o \
                              $(BUILD)/logger.o \
                              | $(TEST_BUILD)
	$(CC) $(CFLAGS) blackbox_c/tests/test_queue.c \
		$(BUILD)/queue.o \
		$(BUILD)/event.o \
		$(BUILD)/timestamp.o \
		$(BUILD)/logger.o \
		-o $(TEST_BUILD)/test_queue.exe

# ---------------------------------------------------------
# Timestamp tests
# ---------------------------------------------------------

$(TEST_BUILD)/test_timestamp.exe: blackbox_c/tests/test_timestamp.c \
                                  $(BUILD)/timestamp.o \
                                  $(BUILD)/logger.o \
                                  | $(TEST_BUILD)
	$(CC) $(CFLAGS) blackbox_c/tests/test_timestamp.c \
		$(BUILD)/timestamp.o \
		$(BUILD)/logger.o \
		-o $(TEST_BUILD)/test_timestamp.exe

# ---------------------------------------------------------
# Entity tests
# ---------------------------------------------------------

$(TEST_BUILD)/test_entity.exe: blackbox_c/tests/test_entity.c \
                               $(BUILD)/entity.o \
                               $(BUILD)/event.o \
                               $(BUILD)/timestamp.o \
                               $(BUILD)/logger.o \
                               | $(TEST_BUILD)
	$(CC) $(CFLAGS) blackbox_c/tests/test_entity.c \
		$(BUILD)/entity.o \
		$(BUILD)/event.o \
		$(BUILD)/timestamp.o \
		$(BUILD)/logger.o \
		-o $(TEST_BUILD)/test_entity.exe

# ---------------------------------------------------------
# Scenario tests
# ---------------------------------------------------------

$(TEST_BUILD)/test_scenario.exe: blackbox_c/tests/test_scenario.c \
                                 $(BUILD)/scenario.o \
                                 $(BUILD)/entity.o \
                                 $(BUILD)/queue.o \
                                 $(BUILD)/event.o \
                                 $(BUILD)/timestamp.o \
                                 $(BUILD)/logger.o \
                                 | $(TEST_BUILD)
	$(CC) $(CFLAGS) blackbox_c/tests/test_scenario.c \
		$(BUILD)/scenario.o \
		$(BUILD)/entity.o \
		$(BUILD)/queue.o \
		$(BUILD)/event.o \
		$(BUILD)/timestamp.o \
		$(BUILD)/logger.o \
		-o $(TEST_BUILD)/test_scenario.exe

# ---------------------------------------------------------
# Simulation tests
# ---------------------------------------------------------

$(TEST_BUILD)/test_simulation.exe: blackbox_c/tests/test_simulation.c \
                                   $(BUILD)/simulation.o \
                                   $(BUILD)/scenario.o \
                                   $(BUILD)/entity.o \
                                   $(BUILD)/queue.o \
                                   $(BUILD)/event.o \
                                   $(BUILD)/timestamp.o \
                                   $(BUILD)/logger.o \
                                   | $(TEST_BUILD)
	$(CC) $(CFLAGS) blackbox_c/tests/test_simulation.c \
		$(BUILD)/simulation.o \
		$(BUILD)/scenario.o \
		$(BUILD)/entity.o \
		$(BUILD)/queue.o \
		$(BUILD)/event.o \
		$(BUILD)/timestamp.o \
		$(BUILD)/logger.o \
		-o $(TEST_BUILD)/test_simulation.exe

# ---------------------------------------------------------
# Logger tests
# ---------------------------------------------------------

$(TEST_BUILD)/test_logger.exe: blackbox_c/tests/test_logger.c \
                               $(BUILD)/logger.o \
                               | $(TEST_BUILD)
	$(CC) $(CFLAGS) blackbox_c/tests/test_logger.c \
		$(BUILD)/logger.o \
		-o $(TEST_BUILD)/test_logger.exe


# ---------------------------------------------------------
# C++ tests
# ---------------------------------------------------------

$(CPP_TEST_BUILD):
	if not exist "$(CPP_TEST_BUILD)" mkdir "$(CPP_TEST_BUILD)"

# ---------------------------------------------------------
# Serializer tests
# ---------------------------------------------------------

$(CPP_TEST_BUILD)/test_serializer.exe: blackbox_cpp/tests/test_serializer.cpp \
                                       $(CPP_BUILD)/serializer.o \
                                       $(BUILD)/event.o \
                                       $(BUILD)/timestamp.o \
                                       | $(CPP_TEST_BUILD)
	$(CXX) $(CXXFLAGS) blackbox_cpp/tests/test_serializer.cpp \
		$(CPP_BUILD)/serializer.o \
		$(BUILD)/event.o \
		$(BUILD)/timestamp.o \
		-o $(CPP_TEST_BUILD)/test_serializer.exe

# ---------------------------------------------------------
# Run all tests
# ---------------------------------------------------------

test: $(TEST_TARGETS)
	@echo.
	@echo =========================
	@echo Running BlackBox tests
	@echo =========================
	@echo.

	$(TEST_BUILD)/test_event.exe
	$(TEST_BUILD)/test_queue.exe
	$(TEST_BUILD)/test_timestamp.exe
	$(TEST_BUILD)/test_entity.exe
	$(TEST_BUILD)/test_scenario.exe
	$(TEST_BUILD)/test_simulation.exe
	$(TEST_BUILD)/test_logger.exe

	@echo.
	@echo =========================
	@echo All tests finished
	@echo =========================


cpp-test: $(CPP_TEST_TARGETS)
	@echo.
	@echo =========================
	@echo Running C++ tests
	@echo =========================
	@echo.

	$(CPP_TEST_BUILD)/test_serializer.exe

	@echo.
	@echo =========================
	@echo C++ tests finished
	@echo =========================

all-tests: test cpp-test

# ---------------------------------------------------------
# Run main program
# ---------------------------------------------------------

run: $(TARGET)
	$(TARGET)

# ---------------------------------------------------------
# Clean build files
# ---------------------------------------------------------

clean:
	if exist "$(BUILD)\*.o" del /Q "$(BUILD)\*.o"
	if exist "$(BUILD)\blackbox.exe" del /Q "$(BUILD)\blackbox.exe"
	if exist "$(TEST_BUILD)\*.exe" del /Q "$(TEST_BUILD)\*.exe"
	if exist "$(CPP_BUILD)\*.o" del /Q "$(CPP_BUILD)\*.o"
	if exist "$(CPP_TEST_BUILD)\*.exe" del /Q "$(CPP_TEST_BUILD)\*.exe"