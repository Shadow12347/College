cc=gcc -std=c99 -ggdb

s=iser
c=icli

ss=iser.c
sc=icli.c

os=${ss:.c=.o}
oc=${sc:.c=.o}

.SUFFIXES: .out .o .c .h
.c.o:
	$(cc) -c $(ss) $(sc)


all: $s $c
.PHONY: all

$s: $(os)
	$(cc) -o $@ $(os)

$c: $(oc)
	$(cc) -o $@ $(oc)

clean:
	-rm -f $s $c $(os) $(oc)

strip:
	strip -g $s $c
