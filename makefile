all:shell


shell: shell.o helper.o environmenthandler.o shellrunner.o 
	g++ -Wall shell.o helper.o environmenthandler.o shellrunner.o -o shell

shell.o: shell.cpp 
	g++ -Wall -c shell.cpp 

helper.o: helper.cpp
	g++ -Wall -c helper.cpp

environmenthandler.o: environmenthandler.cpp
	g++ -Wall -c environmenthandler.cpp

shellrunner.o: shellrunner.cpp
	g++ -Wall -c shellrunner.cpp