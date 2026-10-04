CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra

SRC = main.cpp Restricao.cpp ProblemaPL.cpp FormaPadrao.cpp Tableau.cpp Simplex.cpp

simplex: $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o simplex

clean:
	rm -f simplex