.PHONY: all luo game clean

all: luo game

luo:
	$(MAKE) -C experiments/luo_model

game:
	$(MAKE) -C experiments/game_theory_model

clean:
	rm -rf bin/*
