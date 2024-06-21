# gomoduledata

These programs are used to demonstrate the extraction of the `runtime.moduledata` from a go binary.

`get_moduledata_type` locates the `runtime.moduledata` from within its own address space at runtime, and `extract_moduledata` from a given go binary.

`runtime.moduledata` contains fields that are strings and slices. 

These fields will either by empty or be filled with garbage data for `extract_moduledata`.
