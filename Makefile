
CXX_FLAGS = -std=c++11 -Wall -Wextra -pedantic -Ielib/ -Iinclude/

ENTITY_HEADERS = include/entity.hpp $(wildcard include/entity/*.hpp)
SAMPLE_HEADERS = include/sample.hpp $(wildcard include/sample/*.hpp)
HEADERS = ${ENTITY_HEADERS} ${SAMPLE_HEADERS}

.PHONY: all
all: entity_example.out concept_example.out

.PHONY: clean
clean:
	rm -f *.out

entity_example.out: entity_example.cpp ${HEADERS}
	$(CXX) $(CXX_FLAGS) entity_example.cpp -o entity_example.out

concept_example.out: concept_example.cpp ${HEADERS}
	$(CXX) $(CXX_FLAGS) concept_example.cpp -o concept_example.out
