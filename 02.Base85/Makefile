CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic

all: base85 base85ed_test

base85: base85.cpp base85ed.cpp base85ed.h
	$(CXX) $(CXXFLAGS) base85.cpp base85ed.cpp -o base85

base85ed_test: base85ed_test.cpp base85ed.cpp base85ed.h
	$(CXX) $(CXXFLAGS) base85ed_test.cpp base85ed.cpp -o base85ed_test

test: base85ed_test base85
	./base85ed_test
	./integration_test.sh

clean:
	rm -f base85 base85ed_test roundtrip.txt numbers.txt
