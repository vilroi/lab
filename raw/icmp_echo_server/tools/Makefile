all: check build

check:
	go vet *.go

build:
	go build *.go

test: all
	./verify_checksum test.out

clean:
	find -executable -type f -exec rm {} +

.PHONY: clean test


