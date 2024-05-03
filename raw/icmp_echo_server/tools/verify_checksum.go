package main

import (
	"fmt"
	"os"
)

func main() {
	if len(os.Args) != 2 {
		fmt.Fprintf(os.Stderr, "usage: %s file\n", os.Args[0])
		os.Exit(1)
	}

	data, err := os.ReadFile(os.Args[1])
	check(err)

	verify_checksum(data)
}

func verify_checksum(data []byte) {
	checksum := calc_checksum(data)

	if checksum == 0xffff {
		fmt.Println("Checksum Ok")
	} else {
		fmt.Printf("Wrong checksum: %#x\n", checksum)
	}
}

func calc_checksum(data []byte) uint16 {
	var tmp uint32 = 0
	var checksum uint16 = 0

	length := len(data)
	if (length % 2) != 0 {
		length--
		tmp += uint32(data[length]) << 8
	}

	for i := 0; i < length; i += 2 {
		tmp += (uint32(data[i]) << 8)
		tmp += (uint32(data[i+1]))
	}

	checksum = uint16(tmp&0xffff) + uint16(tmp>>16)
	return checksum
}

func check(e error) {
	if e != nil {
		panic(e)
	}
}
