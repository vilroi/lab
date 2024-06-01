package main

import (
	"fmt"
	"os"
	"syscall"
	"unsafe"
)

const BUFSIZE = 65535

type EtherHeader struct {
	DstMac    [6]uint8
	SrcMac    [6]uint8
	EtherType [2]uint8
}

type Arp struct {
	HardwareType       uint16
	ProtocolAddrType   uint16
	HardwareLen        uint8
	ProtocolAddrLen    uint8
	Operation          uint16
	SenderHardwareAddr [6]uint8
	SenderProtocolAddr [4]uint8
	TargetHardwareAddr [6]uint8
	TargetProtocolAddr [4]uint8
}

func (a *Arp) Dump() {
	fmt.Printf("Hardware Type: %#x\n", htons(a.HardwareType))
	fmt.Printf("Protocol Type: %#x\n", htons(a.ProtocolAddrType))
	fmt.Printf("Hardware len: %#x\n", a.HardwareLen)
	fmt.Printf("Protocol len: %#x\n", a.ProtocolAddrLen)
	fmt.Printf("Message Type: %#x\n", htons(a.Operation))
	fmt.Printf("Sender Mac: %s\n", bytesToMacAddr(a.SenderHardwareAddr))
	fmt.Printf("Target Mac: %s\n", bytesToMacAddr(a.TargetHardwareAddr))
	fmt.Printf("Sender IP Address: %s\n", bytesToIpAddr(a.SenderProtocolAddr))
	fmt.Printf("Target IP Address: %s\n", bytesToIpAddr(a.TargetProtocolAddr))
}

type Packet struct {
	EtherHeader EtherHeader
	Arp         Arp
}

func main() {
	fd, err := syscall.Socket(syscall.AF_PACKET, syscall.SOCK_RAW, int(htons(syscall.ETH_P_ARP)))
	check(err)

	f := os.NewFile(uintptr(fd), "rawsocket")

	buf := make([]byte, BUFSIZE)

	for {
		_, err := f.Read(buf)
		check(err)

		packet := decode(buf)
		packet.Arp.Dump()
		fmt.Println("")
	}

}

func decode(buf []byte) Packet {
	var packet Packet

	rawbufptr := unsafe.Pointer(unsafe.SliceData(buf))

	etherptr := unsafe.Pointer(&packet.EtherHeader)
	memcpy(rawbufptr, etherptr, unsafe.Sizeof(packet.EtherHeader))

	rawbufptr = unsafe.Pointer(uintptr(rawbufptr) + unsafe.Sizeof(packet.EtherHeader))
	arpptr := unsafe.Pointer(&packet.Arp)
	memcpy(rawbufptr, arpptr, unsafe.Sizeof(packet.Arp))

	return packet
}

func memcpy(src, dst unsafe.Pointer, n uintptr) {
	for i := 0; i < int(n); i++ {
		offset := uintptr(i)
		srcp := (*byte)(unsafe.Pointer(uintptr(src) + offset))
		dstp := (*byte)(unsafe.Pointer(uintptr(dst) + offset))

		*dstp = *srcp
	}
}

func dumpBuffer(buf []byte, length int) {
	for i := 0; i < length/16; i += 16 {
		fmt.Printf("%02x%02x %02x%02x %02x%02x %02x%02x %02x%02x %02x%02x %02x%02x %02x%02x\n",
			buf[i], buf[i+1], buf[i+2], buf[i+3], buf[i+4], buf[i+5], buf[i+6], buf[i+7],
			buf[i+8], buf[i+9], buf[i+10], buf[i+11], buf[i+12], buf[i+13], buf[i+14], buf[i+15])
	}
	fmt.Println("")
}

func bytesToMacAddr(macaddr [6]byte) string {
	return fmt.Sprintf("%02x:%02x:%02x:%02x:%02x:%02x", macaddr[0], macaddr[1], macaddr[2], macaddr[3], macaddr[4], macaddr[5])
}

func bytesToIpAddr(ipaddr [4]byte) string {
	return fmt.Sprintf("%d.%d.%d.%d", ipaddr[0], ipaddr[1], ipaddr[2], ipaddr[3])
}

func check(err error) {
	if err != nil {
		panic(err)
	}
}

func htons(val uint16) uint16 {
	return (val >> 8) | (val << 8)
}
