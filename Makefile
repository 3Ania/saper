CC=gcc
objects = saper.o poczatek_gry.o ruch.o podsumowanie.o naj_wyniki.o plansza.o uproszczenie_kodu.o

saper: $(objects)
	$(CC) $^ -o saper

$(objects) : %.o:%.c
	$(CC) -c $^ -o $@

test_program: test_program.c
	$(CC) test_program.c -o test_program

test:
	./test_program

.PHONY: clean
clean:
	del $(objects) saper