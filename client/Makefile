CXX=g++
CXXFLAGS=-std=gnu++11
LDFLAGS=-lcurl
SOURCES=client.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXEC=client

.PHONY: clean mrproper

all: $(EXEC)

$(EXEC): $(OBJECTS) 
	$(CXX) -o $@ $(OBJECTS) $(LDFLAGS)

%.o: %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
