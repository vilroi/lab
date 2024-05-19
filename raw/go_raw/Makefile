all: check build

check:
	go vet *.go

build:
	go build -o raw *.go

test: all
	sudo setcap 'cap_net_raw=ep' raw
	./raw 
