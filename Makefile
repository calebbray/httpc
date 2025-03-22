build:
	@gcc src/*.c main.c -o target/main

run: build
	@./target/main

