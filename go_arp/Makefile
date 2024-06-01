all: check build

check:
	go vet *.go

build:
	go build -o arp *.go

test: all
	sudo setcap 'cap_net_raw=ep' ./arp
	./arp 
