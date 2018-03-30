# Created on: Dez 24, 2014
# Author: Jorgeluis A. Guerra
# Based on tutorial available in: http://ubuntuforum-pt.org/index.php/topic,21155.0.html

CPPFLAGS= -std=gnu++11 -pedantic -O2 -Wall
LIBS= -lm
MAIN = huffman

$(MAIN): $(MAIN).cpp
	g++ $(CPPFLAGS) $(LIBS) -o $@ $<

clean:
	-rm -f $(MAIN) *~ *.log

debug:
	$(MAKE) clean
	g++ -ggdb $(CPPFLAGS) $(LIBS) -o $(MAIN) $(MAIN).cpp

remade:
	$(MAKE) clean
	$(MAKE)

