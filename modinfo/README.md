# modinfo

This is a simple implementation of `modinfo`. The elf parsing portion is by no means polished or comprehensive, but it gets the job done.

```console
$ ./modinfo *.ko
efivarfs.ko:
alias=fs-efivarfs
license=GPL
description=EFI Variable Filesystem
author=Matthew Garrett, Jeremy Kerr
import_ns=EFIVAR
depends=
retpoline=Y
intree=Y
name=efivarfs
vermagic=6.11.8 SMP preempt mod_unload 

hello.ko:
license=GPL
depends=
retpoline=Y
name=hello
vermagic=6.11.8 SMP preempt mod_unload 

iptable_nat.ko:
description=iptables legacy nat table
license=GPL
depends=
retpoline=Y
intree=Y
name=iptable_nat
vermagic=6.11.8 SMP preempt mod_unload 
```
