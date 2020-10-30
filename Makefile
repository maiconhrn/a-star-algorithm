PROJECT_NAME = a-star
SRC_DIR = src
BUILD_DIR = build
RM = rm -r -f
MKDIR = mkdir -p
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(SRCS:$(SRC_DIR)%.cpp=$(BUILD_DIR)%.o)
BIN = $(BUILD_DIR)/$(PROJECT_NAME)
SHELL=/bin/bash

.PHONY: clean run

all: $(PROJECT_NAME)

$(PROJECT_NAME): mkdir-build $(OBJS)
	$(CXX) -std=c++11 -o $(BIN) $(OBJS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) -std=c++11 -c $< -o $@

mkdir-build:
	@$(MKDIR) $(BUILD_DIR)

clean:
	@$(RM) $(BUILD_DIR)

run: build/a-star
	build/a-star
	
tests: build/a-star
	@echo "MOA REPORT - 15 PUZZLE A* TESTS"
	@echo
	@echo "CASE 1 ------------------------- EXPECTED 10"
	@echo -n "answer = "
	@time build/a-star <<< " 1 3 4 5 12 2 14 6 11 13 15 7 0 10 9 8"
	@echo
	@echo "CASE 2 ------------------------- EXPECTED 22"
	@echo -n "answer = "
	@time build/a-star <<< " 11 1 2 3 12 15 13 4 9 10 14 5 8 7 0 6"
	@echo
	@echo "CASE 3 ------------------------- EXPECTED 30"
	@echo -n "answer = "
	@time build/a-star <<< " 11 1 2 0 15 10 12 3 9 14 13 4 8 7 6 5"
	@echo
	@echo "CASE 4 ------------------------- EXPECTED 25"
	@echo -n "answer = "
	@time build/a-star <<< " 11 12 1 5 10 2 4 6 9 13 3 7 8 0 14 15"
	@echo
	@echo "CASE 5 ------------------------- EXPECTED 28"
	@echo -n "answer = "
	@time build/a-star <<< " 12 0 3 15 11 1 8 14 10 2 7 4 9 13 6 5"
	@echo
	@echo "CASE 6 ------------------------- EXPECTED 38"
	@echo -n "answer = "
	@time build/a-star <<< " 10 11 1 0 9 12 13 2 8 15 6 3 7 5 14 4"
	@echo
	@echo "CASE 7 ------------------------- EXPECTED 30"
	@echo -n "answer = "
	@time build/a-star <<< " 12 1 2 3 10 11 13 14 6 4 5 0 15 9 8 7"
	@echo
	@echo "CASE 8 ------------------------- EXPECTED ?"
	@echo -n "answer = "
	@time build/a-star <<< " 0 10 14 11 9 8 15 12 7 5 13 1 4 6 3 2"