# Autores: Jorgeluis Guerra
#          Francisco Erberto
# Baseado no tutorial disponivel em: http://ubuntuforum-pt.org/index.php/topic,21155.0.html

CPPFLAGS= -std=gnu99 -pedantic -Wall -O2
VPATH = ./src
LIBS= -lm
MAIN = lzw
SOURCES = ./src/tree_node.c ./src/functions.c

$(MAIN): $(SOURCES) $(MAIN).c
	gcc $(CPPFLAGS) -I$(VPATH) $(LIBS) -o $@ $^

clean:
	-rm -f $(MAIN) *~

debug:
	$(MAKE) clean
	gcc -Og -ggdb -DDEBUG $(CPPFLAGS) -I$(VPATH) $(LIBS) -o $(MAIN) $(SOURCES) $(MAIN).c
