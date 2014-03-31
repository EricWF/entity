
CXX_FLAGS = -std=c++11 -Wall -Wextra -pedantic -Ielib/ -Iinclude/

ENTITY_HEADERS = include/entity.hpp $(wildcard include/entity/*.hpp)
SAMPLE_HEADERS = include/sample.hpp $(wildcard include/sample/*.hpp)
HEADERS = ${ENTITY_HEADERS} ${SAMPLE_HEADERS}

.PHONY: all
all: entity_example.out concept_example.out

.PHONY: clean
clean:
	rm -f *.out

.PHONY: e
e: clean all

entity_example.out: example/entity_example.cpp ${HEADERS}
	$(CXX) $(CXX_FLAGS) example/entity_example.cpp -o entity_example.out

concept_example.out: example/concept_example.cpp ${HEADERS}
	$(CXX) $(CXX_FLAGS) example/concept_example.cpp -o concept_example.out
