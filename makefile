all:shell


shell: shell.o helper.o environmenthandler.o shellrunner.o getinput.o search.o shellrunnerscript.o
	g++ -Wall shell.o helper.o environmenthandler.o shellrunner.o getinput.o search.o shellrunnerscript.o -o shell

shell.o: shell.cpp 
	g++ -Wall -c shell.cpp 

helper.o: helper.cpp
	g++ -Wall -c helper.cpp

environmenthandler.o: environmenthandler.cpp
	g++ -Wall -c environmenthandler.cpp

shellrunner.o: shellrunner.cpp
	g++ -Wall -c shellrunner.cpp

getinput.o: getinput.cpp
	g++ -Wall -c getinput.cpp

search.o: search.cpp
	g++ -Wall -c search.cpp

shellrunnerscript.o: shellrunnerscript.cpp
	g++ -Wall -c shellrunnerscript.cpp