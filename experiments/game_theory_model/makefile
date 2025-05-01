CXX = g++
CXXFLAGS = -std=c++11 -O2 -I../../include -I../../include/game_theory_model

UTILS = ../../src/game_theory_model/utils.cpp


EXP1_MAIN = exp1_base_model/main.cpp
EXP1_EXEC = ../../bin/game_theory_model/exp1_base_model/exec
EXP1_BIN_DIR = ../../bin/game_theory_model/exp1_base_model

EXP2_MAIN = exp2_model_mutation/main.cpp
EXP2_EXEC = ../../bin/game_theory_model/exp2_model_mutation/exec
EXP2_BIN_DIR = ../../bin/game_theory_model/exp2_model_mutation

EXP3_MAIN = exp3_G_mutation/main.cpp
EXP3_EXEC = ../../bin/game_theory_model/exp3_G_mutation/exec
EXP3_BIN_DIR = ../../bin/game_theory_model/exp3_G_mutation


all: $(EXP1_EXEC) $(EXP2_EXEC) $(EXP3_EXEC)

exp1: $(EXP1_EXEC)

exp2: $(EXP2_EXEC)

exp3: $(EXP3_EXEC)


$(EXP1_EXEC): $(EXP1_MAIN) $(UTILS)
	@mkdir -p $(EXP1_BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^


$(EXP2_EXEC): $(EXP2_MAIN) $(UTILS)
	@mkdir -p $(EXP2_BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(EXP3_EXEC): $(EXP3_MAIN) $(UTILS)
	@mkdir -p $(EXP3_BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -rf ../bin/game_theory_model/*
