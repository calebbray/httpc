build:
	@gcc server.c http.c main.c -o main

run: build
	@./main

