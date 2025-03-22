build:
	@gcc server.c http.c main.c -o target/main

run: build
	@./target/main

