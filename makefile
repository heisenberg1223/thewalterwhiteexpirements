# Define the link target
link-target = ./thewalterwhiteexpirements

# Define the libs
LIBS = -lraylib -lGL -lglfw -lopenal -lm -lpthread -ldl

# Deifine any CFLAGS
CFLAGS = --std=c++20 -O3 -Wall

# Define the objects
objects = $(patsubst %.cpp, %.o, $(wildcard ./src/*.cpp))

# Define the rebuildables
rebuildables = $(link-target) $(objects)

# The rule to perform the build
$(link-target): $(objects)
	g++ -o $(link-target) $(objects) $(CFLAGS) $(LIBS)

# Rule to create the object files
%.o: %.cpp
	g++ -o $@ -c $< $(CFLAGS)

.PHONY: clean
clean:
	rm -vrf $(rebuildables)
