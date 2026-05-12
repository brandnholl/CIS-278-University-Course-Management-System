CXX      = g++
CXXFLAGS = -std=c++17 -Wall

all:
	$(CXX) $(CXXFLAGS) main.cpp -o cms
	@echo "Done. Run: ./cms"

clean:
	rm -f cms *.csv
