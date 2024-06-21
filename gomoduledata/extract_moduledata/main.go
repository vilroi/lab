package main

import (
	"debug/elf"
	"errors"
	"fmt"
	"os"
	"unsafe"

	"github.com/vilroi/goutils"
)

var (
	SymbolNotFound     = errors.New("Symbol not found")
	ElfUnexpectedError = errors.New("Unexpected error")
)

func main() {
	if len(os.Args) != 2 {
		fmt.Fprintf(os.Stderr, "usage: %s file\n", os.Args[0])
		os.Exit(1)
	}

	elf, err := elf.Open(os.Args[1])
	check(err)
	defer elf.Close()

	if !isGoBinary(elf) {
		fmt.Fprintf(os.Stderr, "The supplied ELF file must be a Go binary")
		os.Exit(1)
	}

	moduledata := extractModuleData(elf)

	fmt.Printf("%#x\n", moduledata.types)
	fmt.Println(moduledata.modulename)
}

func extractModuleData(e *elf.File) moduledata {
	addr, err := searchSymbolByName(e, "runtime.firstmoduledata")
	check(err)

	loadaddr, err := getLoadAddr(e)
	check(err)

	offset := addr - loadaddr

	f, err := os.Open(os.Args[1])
	check(err)
	defer f.Close()

	var moduledata moduledata
	buf := make([]byte, goutils.SizeOf(moduledata))
	_, err = f.ReadAt(buf, int64(offset))
	check(err)

	srcp := unsafe.Pointer(unsafe.SliceData(buf))
	dstp := unsafe.Pointer(&moduledata)
	Memncpy(srcp, dstp, int(goutils.SizeOf(moduledata)))

	return moduledata
}

func Memncpy(src, dst unsafe.Pointer, n int) {
	var i uintptr
	for ; i < uintptr(n); i++ {
		srcp := (*byte)(unsafe.Pointer(uintptr(src) + i))
		dstp := (*byte)(unsafe.Pointer(uintptr(dst) + i))

		*dstp = *srcp
	}
}

func isGoBinary(file *elf.File) bool {
	for _, s := range file.Sections {
		if s.Name == ".gosymtab" {
			return true
		}
	}
	return false
}

func getLoadAddr(file *elf.File) (uint64, error) {
	for _, prog := range file.Progs {
		if prog.Type != elf.PT_LOAD {
			continue
		}
		return prog.Vaddr, nil
	}

	return 0, ElfUnexpectedError
}

func searchSymbolByName(elf *elf.File, symbol string) (uint64, error) {
	symbols, err := elf.Symbols()
	check(err)

	for _, sym := range symbols {
		if sym.Name == symbol {
			return sym.Value, nil
		}
	}

	return 0, SymbolNotFound
}

func check(e error) {
	if e != nil {
		panic(e)
	}
}
