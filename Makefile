hill_climbing: hill_climbing.o
	g++ hill_climbing.o -o hill_climbing

hill_climbing.o: hill_climbing.cpp
	g++ -c hill_climbing.cpp

clean: 
	rm -rf *o hill_climbing
