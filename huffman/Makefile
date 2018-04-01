# Autores: Jorgeluis Guerra
#          Francisco Erberto
# Baseado no tutorial disponivel em: http://ubuntuforum-pt.org/index.php/topic,21155.0.html

CPPFLAGS= -std=gnu++11 -pedantic -O2 -Wall
VPATH = ./src
LIBS= -lm
MAIN = huffman
SOURCES = NodeArvore.cpp common_functions.cpp compress_functions.cpp decompress_functions.cpp

$(MAIN): $(SOURCES) $(MAIN).cpp
	g++ $(CPPFLAGS) -I$(VPATH) $(LIBS) -o $@ $^

clean:
	-rm -f $(MAIN) *~ *.huf

debug:
	$(MAKE) clean
	g++ -Og -ggdb $(CPPFLAGS) $(LIBS) -o $(MAIN) $(SOURCES) $(MAIN).cpp

remade:
	$(MAKE) clean
	$(MAKE)
