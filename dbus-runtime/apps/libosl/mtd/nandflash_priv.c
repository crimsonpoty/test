/*
 * nand_priv.c
 *
 *  Created on: Mar 29, 2012
 *      Author: buttonfly
 */


#include "flash_priv.h"

#include <stdio.h>
#include <stdint.h>
#include <mtd/mtd-user.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "common.h"
#include <libmtd.h>
#include <mtd/mtd-user.h>
#include <mtd/jffs2-user.h>
#include <stdbool.h>
#include "osl.h"
#include "osldbg.h"


static void erase_buffer(void *buffer, size_t size) ;


int32_t ProgramNandFlash(osl_mtd handle, u_int32_t offset, u_int8_t* buf, u_int32_t length) {

	osl_mtd_t* context = (osl_mtd_t*)handle;
	struct mtd_dev_info mtd;
	uint8_t write_mode = 0;
	int pagelen = 0;
	size_t filebuf_max = 0;
	size_t filebuf_len = 0;
	int ret = 0;
	int cnt = 0;
	int errno;

	uint8_t* filebuf;
	int mtdoffset;
	int imglen = 0;
	int ebsize_aligned = 0;
	/* points to the current page inside filebuf */
	unsigned char *writebuf = NULL;
	/* points to the OOB for the current page in filebuf */
	unsigned char *oobbuf = NULL;
	long long blockstart = -1;
	long long offs;
	bool baderaseblock = false;
	int		blockalign = 1; /* default to using actual block size */
	bool pad = true;
	bool writeoob = false;
	bool onlyoob= false;
	bool markbad = false;
	long long __offset = 0;


	mtdoffset = offset;
	imglen = length;

	if(offset & (mtd.min_io_size - 1)) {
		fprintf(stderr, "The start address is not page-aligned ! The pagesize of this NAND Flash is 0x%x", mtd.min_io_size);
		return -1;
	}

	write_mode = MTD_OPS_PLACE_OOB;
	pagelen = mtd.min_io_size;
	ebsize_aligned =  mtd.eb_size * blockalign;

	filebuf_max = ebsize_aligned / mtd.min_io_size * pagelen;
	filebuf = xmalloc(filebuf_max);
	erase_buffer(filebuf, filebuf_max);

#if 1
	/*
	 * Get data from input and write to the device while there is
	 * still input to read and we are still within the device
	 * bounds. Note that in the case of standard input, the input
	 * length is simply a quasi-boolean flag whose values are page
	 * length or zero.
	 */
	while (((imglen > 0) || (writebuf < (filebuf + filebuf_len)))
		&& (mtdoffset < mtd.size)) {
		/*
		 * New eraseblock, check for bad block(s)
		 * Stay in the loop to be sure that, if mtdoffset changes because
		 * of a bad block, the next block that will be written to
		 * is also checked. Thus, we avoid errors if the block(s) after the
		 * skipped block(s) is also bad (number of blocks depending on
		 * the blockalign).
		 */
		while (blockstart != (mtdoffset & (~ebsize_aligned + 1))) {
			blockstart = mtdoffset & (~ebsize_aligned + 1);
			offs = blockstart;

			/*
			 * if writebuf == filebuf, we are rewinding so we must
			 * not reset the buffer but just replay it
			 */
			if (writebuf != filebuf) {
				erase_buffer(filebuf, filebuf_len);
				filebuf_len = 0;
				writebuf = filebuf;
			}

			baderaseblock = false;
			fprintf(stdout, "Writing data to block %lld at offset 0x%llx\n",
								 blockstart / ebsize_aligned, blockstart);

			/* Check all the blocks in an erase block for bad blocks */
#if fixme
			if (noskipbad)
				continue;
#endif

			do {
				if ((ret = mtd_is_bad(&mtd, context->fd, offs / ebsize_aligned)) < 0) {
					sys_errmsg("%s: MTD get bad block failed", context->device);
					goto closeall;
				} else if (ret == 1) {
					baderaseblock = true;
					fprintf(stderr, "Bad block at %llx, %u block(s) "
													"from %llx will be skipped\n",
													offs, blockalign, blockstart);
				}

				if (baderaseblock) {
					mtdoffset = blockstart + ebsize_aligned;
				}
				offs +=  ebsize_aligned / blockalign;
			} while (offs < blockstart + ebsize_aligned);

		}

		/* Read more data from the input if there isn't enough in the buffer */
		if ((writebuf + mtd.min_io_size) > (filebuf + filebuf_len)) {
			int readlen = mtd.min_io_size;

			int alreadyread = (filebuf + filebuf_len) - writebuf;
			int tinycnt = alreadyread;

			while (tinycnt < readlen) {
#if fixme
				cnt = read(ifd, writebuf + tinycnt, readlen - tinycnt);
#else
				memcpy(writebuf+tinycnt, &buf[__offset], readlen - tinycnt);
				__offset += readlen - tinycnt;
#endif

				if (cnt == 0) { /* EOF */
					break;
				} else if (cnt < 0) {
					perror("File I/O error on input");
					goto closeall;
				}
				tinycnt += cnt;
			}

			/* No padding needed - we are done */
			if (tinycnt == 0) {
				/*
				 * For standard input, set imglen to 0 to signal
				 * the end of the "file". For nonstandard input,
				 * leave it as-is to detect an early EOF.
				 */
#if fixme
				if (ifd == STDIN_FILENO) {
					imglen = 0;
				}
#endif
				break;
			}

			/* Padding */
			if (tinycnt < readlen) {
				if (!pad) {
					fprintf(stderr, "Unexpected EOF. Expecting at least "
							"%d more bytes. Use the padding option.\n",
							readlen - tinycnt);
					goto closeall;
				}
				erase_buffer(writebuf + tinycnt, readlen - tinycnt);
			}

			filebuf_len += readlen - alreadyread;
#if fixme
			if (ifd != STDIN_FILENO) {
				imglen -= tinycnt - alreadyread;
			}
			else if (cnt == 0) {
				/* No more bytes - we are done after writing the remaining bytes */
				imglen = 0;
			}
#else
			if(cnt == 0) {
				imglen = 0;
			}
			else {
				imglen -= tinycnt - alreadyread;
			}
#endif
		}

		if (writeoob) {
			oobbuf = writebuf + mtd.min_io_size;

			/* Read more data for the OOB from the input if there isn't enough in the buffer */
			if ((oobbuf + mtd.oob_size) > (filebuf + filebuf_len)) {
				int readlen = mtd.oob_size;
				int alreadyread = (filebuf + filebuf_len) - oobbuf;
				int tinycnt = alreadyread;

				while (tinycnt < readlen) {
#if fixme
					cnt = read(ifd, oobbuf + tinycnt, readlen - tinycnt);
#endif
					if (cnt == 0) { /* EOF */
						break;
					} else if (cnt < 0) {
						perror("File I/O error on input");
						goto closeall;
					}
					tinycnt += cnt;
				}

				if (tinycnt < readlen) {
					fprintf(stderr, "Unexpected EOF. Expecting at least "
							"%d more bytes for OOB\n", readlen - tinycnt);
					goto closeall;
				}

				filebuf_len += readlen - alreadyread;
				if(cnt == 0) {
					imglen = 0;
				}
				else {
					imglen -= tinycnt - alreadyread;
				}
			}
		}

		/* Write out data */


		ret = mtd_write(context->desc, &mtd, context->fd, mtdoffset / mtd.eb_size,
				mtdoffset % mtd.eb_size,
				onlyoob ? NULL : writebuf,
				onlyoob ? 0 : mtd.min_io_size,
				writeoob ? oobbuf : NULL,
				writeoob ? mtd.oob_size : 0,
				write_mode);
		if (ret) {
			int i;
			if (errno != EIO) {
				sys_errmsg("%s: MTD write failure", context->device);
				goto closeall;
			}

			/* Must rewind to blockstart if we can */
			writebuf = filebuf;

			fprintf(stderr, "Erasing failed write from %#08llx to %#08llx\n",
				blockstart, blockstart + ebsize_aligned - 1);
			for (i = blockstart; i < blockstart + ebsize_aligned; i += mtd.eb_size) {
				if (mtd_erase(context->desc, &mtd, context->fd, i / mtd.eb_size)) {
					int errno_tmp = errno;
					sys_errmsg("%s: MTD Erase failure",  context->device);
					if (errno_tmp != EIO) {
						goto closeall;
					}
				}
			}

			if (markbad) {
				fprintf(stderr, "Marking block at %08llx bad\n", (long long unsigned int)
						mtdoffset & (~mtd.eb_size + 1));
				if (mtd_mark_bad(&mtd, context->fd, mtdoffset / mtd.eb_size)) {
					sys_errmsg("%s: MTD Mark bad block failure", context->device);
					goto closeall;
				}
			}
			mtdoffset = blockstart + ebsize_aligned;

			continue;
		}
		mtdoffset += mtd.min_io_size;
		writebuf += pagelen;
	}
#endif

closeall:

	return 0;
}


int32_t ReadNandFlash(osl_mtd handle, u_int32_t offset, u_int8_t* buf, u_int32_t length) {
	// TODO
	return 0;
}


static void erase_buffer(void *buffer, size_t size) {
	const uint8_t kEraseByte = 0xff;

	if (buffer != NULL && size > 0) {
		memset(buffer, kEraseByte, size);
	}
}

