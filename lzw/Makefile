# Autores: Jorgeluis Guerra
#          Francisco Erberto
# Baseado no tutorial disponivel em: http://ubuntuforum-pt.org/index.php/topic,21155.0.html

CPPFLAGS= -std=gnu99 -pedantic -Wall -O2
VPATH = ./src
LIBS= -lm
MAIN = lzw
#SOURCES =

$(MAIN): $(SOURCES) $(MAIN).cpp
	g++ $(CPPFLAGS) -I$(VPATH) $(LIBS) -o $@ $^

clean:
	-rm -f $(MAIN) *~

debug:
	$(MAKE) clean
	g++ -Og -ggdb -DDEBUG $(CPPFLAGS) -I$(VPATH) $(LIBS) -o $(MAIN) $(SOURCES) $(MAIN).cpp
