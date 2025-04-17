CXX = g++
CXXFLAGS = -std=c++11 -O2 -I../../include -I../../include/luo_model

UTILS = ../../src/luo_model/utils.cpp


EXP1_MAIN = exp1_base_model/main.cpp
EXP1_EXEC = ../../bin/luo_model/exp1_base_model/exec
EXP1_BIN_DIR = ../../bin/luo_model/exp1_base_model

EXP2_MAIN = exp2_model_mutation/main.cpp
EXP2_EXEC = ../../bin/luo_model/exp2_model_mutation/exec
EXP2_BIN_DIR = ../../bin/luo_model/exp2_model_mutation


all: $(EXP1_EXEC) $(EXP2_EXEC)

exp1: $(EXP1_EXEC)

exp2: $(EXP2_EXEC)


$(EXP1_EXEC): $(EXP1_MAIN) $(UTILS)
	@mkdir -p $(EXP1_BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^


$(EXP2_EXEC): $(EXP2_MAIN) $(UTILS)
	@mkdir -p $(EXP2_BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^


clean:
	rm -rf ../bin/luo_model/*
