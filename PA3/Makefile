all: multi-lookup.c util.c
	gcc -Wall -Wextra multi-lookup.c -o multi-lookup util.c -pthread -g -w
	touch req.log res.log results.txt serviced.txt performance.txt
clean:
	rm *.txt *.log *.o *tar.gz multi-lookup
performance: all
	@echo "Number for requester thread: 1"
	@echo "Number for resolver thread = 1"
	./multi-lookup 1 1 req.log res.log input/*
	@echo ""
	@echo "Number for requester thread: 1"
	@echo "Number for resolver thread = 3"
	./multi-lookup 1 3 req.log res.log input/*
	@echo ""
	@echo "Number for requester thread: 3"
	@echo "Number for resolver thread = 1"
	./multi-lookup 3 1 req.log res.log input/*
	@echo ""
	@echo "Number for requester thread: 3"
	@echo "Number for resolver thread = 3"
	./multi-lookup 3 3 req.log res.log input/*
	@echo ""
	@echo "Number for requester thread: 5"
	@echo "Number for resolver thread = 5"
	./multi-lookup 5 5 req.log res.log input/*
	@echo ""
	@echo "Number for requester thread: 8"
	@echo "Number for resolver thread = 5"
	./multi-lookup 8 5 req.log res.log input/*
bundle:
	tar -cvf PA3_Hill.tar.gz multi-lookup.c multi-lookup.h Makefile README.md performance.txt
