/*
 * This file and its contents are supplied under the terms of the
 * Common Development and Distribution License ("CDDL"), version 1.0.
 * You may only use this file in accordance with the terms of version
 * 1.0 of the CDDL.
 *
 * A full copy of the text of the CDDL should have accompanied this
 * source.  A copy of the CDDL is also available via the Internet at
 * http://www.illumos.org/license/CDDL.
 *
 * Copyright 2018 Joyent, Inc.
 */

// FIXME endianness, 512 bytes

#include <assert.h>

#include <sys/types.h>
#include <sys/dktp/fdisk.h>

#include <mdb/mdb_modapi.h>
#include <mdb/mdb_debug.h>

// FIXME
#include "../../../../boot/installboot/i386/installboot.h"

static void
print_fdisk_part(struct ipart *ip, size_t nr)
{
	if (ip->systid == 0) {
		mdb_printf("%-5llu 0x0\n", nr);
		return;
	}

	mdb_printf("%-5llu 0x%hhx(", nr, ip->systid);

	switch (ip->systid) {
	case EFI_PMBR: mdb_printf("%-7s ", "EFI_PMBR) "); break;
	case EFI_FS: mdb_printf("%-7s", "EFI_FS) "); break;
	// FIXME:
	default: mdb_printf("%-7s) ", "?) "); break;
	}

	mdb_printf("0x%hhx(", ip->bootid);
	switch (ip->bootid) {
	case NOTACTIVE: mdb_printf("inactive) "); break;
	case ACTIVE: mdb_printf("active) "); break;
	default: mdb_printf("invalid) "); break;
	}

	mdb_printf("%hu/%hu/%hu ",
	    (uint16_t)ip->begcyl | (uint16_t)(ip->begsect & ~0x3f) << 2,
	    (uint16_t)ip->beghead,
	    (uint16_t)ip->begsect & 0x3f);

	mdb_printf("%hu/%hu/%hu ",
	    (uint16_t)ip->endcyl | (uint16_t)(ip->endsect & ~0x3f) << 2,
	    (uint16_t)ip->endhead,
	    (uint16_t)ip->endsect & 0x3f);

	mdb_printf("%u %u\n", ip->relsect, ip->numsect);
}

/*ARGSUSED*/
static int
cmd_mbr(uintptr_t addr, uint_t flags, int argc, const mdb_arg_t *argv)
{
	struct mboot mbr;

	assert(sizeof (mbr) == SECTOR_SIZE);

	if (argc != 0)
		return (DCMD_USAGE);

	if (!(flags & DCMD_ADDRSPEC))
		addr = 0;

	if (mdb_vread(&mbr, sizeof (mbr), addr) == -1) {
		mdb_warn("failed to read MBR");
		return (DCMD_ERR);
	}

	// FIXME BPB

	mdb_printf("mbr_version: %hhu\n",
	    *(uint8_t *)&mbr.bootinst[STAGE1_MBR_VERSION]);

	mdb_printf("stage2_lba: %llu\n",
	    *(uint64_t *)&mbr.bootinst[STAGE1_STAGE2_LBA]);

	mdb_printf("stage2_size: %hu\n",
	    *(uint16_t *)&mbr.bootinst[STAGE1_STAGE2_SIZE]);

	// FIXME: STAGE1_STAGE2_UUID

	mdb_printf("stage1_sig: 0x%lx\n",
	    *(uint32_t *)&mbr.bootinst[STAGE1_SIG]);

	mdb_printf("mbr_magic: 0x%hx\n", mbr.signature);

	mdb_printf("%<u>%-5s %-10s %s %s %s %s %s%</u>\n",
	    "PART", "TYPE", "ACTIVE", "STARTCHS", "ENDCHS",
	    "SECTOR", "NUMSECT");

	for (size_t i = 0; i < FD_NUMPART; i++) {
		struct ipart *ip = (struct ipart *)
		   (mbr.parts + (sizeof (struct ipart) * i));
		print_fdisk_part(ip, i);
	}

	return (DCMD_OK);
}

static const mdb_dcmd_t dcmds[] = {
	{ "mbr", NULL, "dump Master Boot Record", cmd_mbr },
	{ NULL }
};

#if 0
static const mdb_walker_t walkers[] = {
	{ NULL }
};
#endif

static const mdb_modinfo_t modinfo = {
	MDB_API_VERSION, dcmds, NULL //walkers
};

const mdb_modinfo_t *
_mdb_init(void)
{
	return (&modinfo);
}
