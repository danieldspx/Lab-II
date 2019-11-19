PLANILHA=planilha.cpp
CELULA=celula.cpp
UTIL=util.cpp
MAIN=main.cpp
main: $(MAIN) $(CELULA) $(PLANILHA) $(UTIL)
	g++ -std=c++11 $(MAIN) $(CELULA) $(PLANILHA) $(UTIL) -o bin/main -lcaca -Wall

debug: $(MAIN) $(CELULA) $(PLANILHA) $(UTIL)
	g++ -g -std=c++11 $(MAIN) $(CELULA) $(PLANILHA) $(UTIL) -o bin/main.debug -lcaca -Wall