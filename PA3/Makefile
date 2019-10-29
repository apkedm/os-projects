all: multi-lookup.c util.c
	gcc -Wall -Wextra multi-lookup.c -o multi-lookup util.c -pthread -g -w
	touch req.log res.log results.txt serviced.txt
clean:
	rm *.txt *.log *.o multi-lookup
