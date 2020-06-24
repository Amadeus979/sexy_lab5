/*
 * operations on IDE disk.
 */

#include "fs.h"
#include "lib.h"
#include <mmu.h>

#define IDE_BEGIN_ADDR 0x13000000
#define IDE_OFFSET_ADDR IDE_BEGIN_ADDR + 0x0000
#define IDE_OFFSETHI_ADDR IDE_BEGIN_ADDR + 0x0008
#define IDE_ID_ADDR IDE_BEGIN_ADDR + 0x0010
#define IDE_OP_ADDR IDE_BEGIN_ADDR + 0x0020
#define IDE_STATUS_ADDR IDE_BEGIN_ADDR + 0x0030
#define IDE_BUFFER_ADDR IDE_BEGIN_ADDR + 0x4000
#define IDE_BUFFER_SIZE 0x0200
// Overview:
// 	read data from IDE disk. First issue a read request through
// 	disk register and then copy data from disk buffer
// 	(512 bytes, a sector) to destination array.
//
// Parameters:
//	diskno: disk number.
// 	secno: start sector number.
// 	dst: destination for data read from IDE disk.
// 	nsecs: the number of sectors to read.
//
// Post-Condition:
// 	If error occurred during read the IDE disk, panic. 
// 	
// Hint: use syscalls to access device registers and buffers
void
ide_read(u_int diskno, u_int secno, void *dst, u_int nsecs)
{
	// 0x200: the size of a sector: 512 bytes.
	int offset_begin = secno * 0x200;
	int offset_end = offset_begin + nsecs * 0x200;
	int offset = 0;
	int r;
	int op = 0;
	int status;
	int vaToWrite;
	while (offset_begin + offset < offset_end) {
            // Your code here
            // error occurred, then panic.
		vaToWrite = offset_begin + offset;
		r = syscall_write_dev(&diskno,IDE_ID_ADDR,4);
		if (r < 0) {
			user_panic("write secno(id) failed\n");
		}
		r = syscall_write_dev(&vaToWrite,IDE_OFFSET_ADDR,4);
		if (r < 0) {
			user_panic("write offset failed\n");
		}
		r = syscall_write_dev(&op,IDE_OP_ADDR,4);
		if (r < 0) {
			user_panic("write opration failed\n");
		}
		status = 0;
		r = syscall_read_dev(&status,IDE_STATUS_ADDR,4);
		if (r < 0) {
			user_panic("read status failed\n");
		}
		if (status == 0) {
			user_panic("read status wrong!\n");
		}
		r = syscall_read_dev(dst + offset,IDE_BUFFER_ADDR,IDE_BUFFER_SIZE);
		if (r < 0) {
			user_panic("fuck, the last step read from went wrong\n");
		}
		offset+=0x200;
	}
}


// Overview:
// 	write data to IDE disk.
//
// Parameters:
//	diskno: disk number.
//	secno: start sector number.
// 	src: the source data to write into IDE disk.
//	nsecs: the number of sectors to write.
//
// Post-Condition:
//	If error occurred during read the IDE disk, panic.
//	
// Hint: use syscalls to access device registers and buffers
void
ide_write(u_int diskno, u_int secno, void *src, u_int nsecs)
{
        // Your code here
	// int offset_begin = ;
	// int offset_end = ;
	// int offset = ;
	writef("diskno: %d\n", diskno);
	int offset_begin = secno * 0x200;
	int offset_end = offset_begin + nsecs * 0x200;
	int offset = 0;
	int r;
	int op = 1;
	int status;
	int vaToWrite;
	while (offset_begin + offset < offset_end) {
            // Your code here
            // error occurred, then panic.
		vaToWrite = offset_begin + offset;
		r = syscall_write_dev(&diskno,IDE_ID_ADDR,4);
		if (r < 0) {
			user_panic("write secno(id) failed\n");
		}
		r = syscall_write_dev(&vaToWrite,IDE_OFFSET_ADDR,4);
		if (r < 0) {
			user_panic("write offset failed\n");
		}
		r = syscall_write_dev(src + offset, IDE_BUFFER_ADDR,IDE_BUFFER_SIZE);
		if (r < 0) {
			user_panic("fuck, the last step write to went wrong\n");
		}
		r = syscall_write_dev(&op,IDE_OP_ADDR,4);
		if (r < 0) {
			user_panic("write opration failed\n");
		}
		r = syscall_read_dev(&status,IDE_STATUS_ADDR,4);
		if (r < 0) {
			user_panic("read status failed\n");
		}
		if (status == 0) {
			user_panic("read status wrong!\n");
		}
		offset+=0x200;
	}
}

