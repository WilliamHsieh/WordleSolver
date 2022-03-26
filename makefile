SRC = main.cpp
TAR = wordle
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wshadow -fopenmp -Ofast

run: $(TAR)
	./$(TAR)

$(TAR): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm $(TAR)
