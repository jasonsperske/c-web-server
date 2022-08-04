
.PHONY: server

default: server

server: aisl/build/libaisl.a dirs
	gcc -I./aisl/include src/server.c aisl/build/libaisl.a -o build/$@

aisl/build/libaisl.a:
	cd aisl && make AISL_WITH_SSL=0 libaisl.a

dirs:
	mkdir -p build

clean:
	rm -Rf build