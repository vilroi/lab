// Copyright 2014 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

package main

type pclntab struct {
	magic          uint32
	pad0, pad1     uint8
	minInsSize     uint8
	ptrSize        uint8
	nfunc          int
	nfiles         uint
	textStart      uintptr
	funcnameOffset uintptr
	cuOffset       uintptr
	filetabOffset  uintptr
	pactabOffset   uintptr
	pclnOffset     uintptr
}

type initTask struct {
	state uint32
	nfns  uint32
}

type _type uint // dummy. An actual type is abi.Type
type typeOff int32

type ptabEntry struct {
	name int32
	typ  int32
}

type bitvector struct {
	n        int32
	bytedata *uint8
}

type modulehash struct {
	modulename   string
	linktimehash string
	runtimehash  string
}

type fntab struct {
	entryoff uint32
	funcoff  uint32
}

type textsect struct {
	vaddr    uintptr
	end      uintptr
	baseaddr uintptr
}

type itab struct {
	Inter uintptr
	Type  uintptr
	Hash  uint32
	Func  [1]uintptr
}

type moduledata struct {
	pcheader     *pclntab
	funcnametab  []byte
	cutab        []uint32
	filetab      []byte
	pctab        []byte
	pclntable    []byte
	ftab         []fntab
	findfunctab  uintptr
	minpc, maxpc uintptr

	text, etext           uintptr
	noptrdata, enoptrdata uintptr
	data, edata           uintptr
	nbss, ebss            uintptr
	noptrbss, eoptrbss    uintptr
	covctrs, ecovctrs     uintptr
	end, gcdata, gcbss    uintptr
	types, etypes         uintptr
	rodata                uintptr
	gofunc                uintptr
	textsectmap           []textsect
	typelinks             []int32
	itablinks             []*itab

	ptab []ptabEntry

	pluginpath string
	pkghashes  []modulehash

	// This slice records the initializing tasks that need to be
	// done to start up the program. It is built by the linker.
	inittasks []*initTask

	modulename   string
	modulehashes []modulehash

	hasmain uint8 // 1 if module contains the main function, 0 otherwise
	bad     bool  // module failed to load and should be ignored

	gcdatamask, gcbssmask bitvector

	typemap map[typeOff]*_type // offset to *_rtype in previous module

	next *moduledata
}
