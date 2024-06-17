package main

import (
	"debug/elf"
	"fmt"
	"strings"
	"unsafe"
)

func main() {
	module := getModuleDataAddr()

	modp := (*moduledata)(unsafe.Pointer(module))
	fmt.Printf("%#x\n", modp.types)
}

func getModuleDataAddr() uintptr {
	elf, err := elf.Open("/proc/self/exe")
	check(err)
	defer elf.Close()

	symtab, err := elf.Symbols()
	check(err)

	for _, sym := range symtab {
		if strings.Contains(sym.Name, "firstmodule") {
			return uintptr(sym.Value)
		}
	}

	return 0 // should not reach here
}

func check(e error) {
	if e != nil {
		panic(e)
	}
}
