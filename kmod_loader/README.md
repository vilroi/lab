# Kernel Module Loader POC

## Summary 

This small program does the following:
1. Determines the kernel release version with `uname(2)`
2. Fetches a kernel module built for the host kernel from the specified web server
3. Loads the kernel module from memory (a buffer) using the `init_module(2)` syscall


## Details
The program makes a http GET request against the web server with an url with the following format:

```
http://host/X.Y.Z-AB
```

...where X.Y.Z-AB is the kernel release version.

For example, if the target kernel release version is 6.1.0-21, and the web server ip address is 192.168.122.1:

```console
http://192.168.122.1/6.1.0-21
```

The program is expecting to receive back a kernel module from the server.

On the server side, the web server should be configured such that a kernel module of the corresponding release version will be served when a request is made from the client.

For instance, here is the directory layout of my nginx VM used in the demo video:
```console
$ tree
.
├── 6.1.0-21
│   └── module.ko
└── index.html

2 directories, 2 files

```

For more details, please reference the nginx config that is included.

## But why?
When I first found out about ```init_module(2)```, what stood out to me was that it took a buffer containg the data of the kernel module instead of the file path as an argument.

This gave me the thought that it should be possible to load a kernel module without touching disk, if I just fetched it over the network and used that buffer as an argument.

The program was written to test this idea.
