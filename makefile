all : pgmrotacao pgmlbp pgmnega pgmlimiar pgmmedia pgmmediana
	echo "Finished!"

#--
CFLAGS = -Wall

objects = datastruct.o parser.o

datastruct.o : datastruct.c
	gcc $(CFLAGS) -c datastruct.c

parser.o : parser.c
	gcc $(CFLAGS) -c parser.c

#--

pgmrotacao : pgmrotacao.o $(objects)
	gcc -o pgmrotacao pgmrotacao.o $(objects) -lm

pgmrotacao.o : pgmrotacao.c
	gcc $(CFLAGS) -c pgmrotacao.c -lm


pgmnega : pgmnega.o $(objects)
	gcc -o pgmnega pgmnega.o $(objects)

pgmnega.o : pgmnega.c
	gcc $(CFLAGS) -c pgmnega.c


pgmlimiar : pgmlimiar.o $(objects)
	gcc -o pgmlimiar pgmlimiar.o $(objects)

pgmlimiar.o : pgmlimiar.c
	gcc $(CFLAGS) -c pgmlimiar.c


pgmmedia : pgmmedia.o $(objects)
	gcc -o pgmmedia pgmmedia.o $(objects)

pgmmedia.o : pgmmedia.c
	gcc $(CFLAGS) -c pgmmedia.c


pgmmediana : pgmmediana.o $(objects)
	gcc -o pgmmediana pgmmediana.o $(objects)

pgmmediana.o : pgmmediana.c
	gcc $(CFLAGS) -c pgmmediana.c


pgmlbp : pgmlbp.o $(objects)
	gcc -o pgmlbp pgmlbp.o $(objects)

pgmlbp.o : pgmlbp.c
	gcc $(CFLAGS) -c pgmlbp.c

#______________________________________________________

clean:
	rm pgmrotacao pgmrotacao.o pgmnega pgmnega.o pgmlimiar pgmlimiar.o pgmmedia pgmmedia.o pgmmediana pgmmediana.o pgmlbp pgmlbp.o $(objects)

#______________________________________________________

purge:
	ls | grep -v -F -e ".c" -e ".h" -e "makefile" | xargs rm