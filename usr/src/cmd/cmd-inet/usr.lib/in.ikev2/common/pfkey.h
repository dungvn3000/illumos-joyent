/*
 * This file and its contents are supplied under the terms of the
 * Common Development and Distribution License ("CDDL"), version 1.0.
 * You may only use this file in accordance with the terms of version
 * 1.0 of the CDDL.
 *
 * A full copy of the text of the CDDL should have accompanied this
 * source.  A copy of the CDDL is also available via the Internet at
 * http://www.illumos.org/license/CDDL.
 */

/*
 * Copyright 2017 Joyent, Inc.
 */

#ifndef _PFKEY_H
#define	_PFKEY_H

#include <sys/types.h>
#include <sys/socket.h>
#include <net/pfkeyv2.h>
#include "ikev2.h"

#ifdef __cplusplus
extern "C" {
#endif

#define	IKEV2_SADB_INITIATOR	SADB_X_SAFLAGS_KM1

struct ikev2_sa_s;
struct ikev2_child_sa_s;
struct ikev2_sa_result_s;
struct parsedmsg_s;
struct ts_s;

void pfkey_send_error(const sadb_msg_t *, uint8_t);
boolean_t pfkey_getspi(const struct parsedmsg_s *restrict, uint8_t,
    uint32_t *restrict);
boolean_t pfkey_inverse_acquire(const struct ts_s *, const struct ts_s *,
    const struct ts_s *, const struct ts_s *, struct parsedmsg_s **);
boolean_t pfkey_sadb_add_update(struct ikev2_sa_s *restrict,
    struct ikev2_child_sa_s *restrict, const uint8_t *restrict,
    const uint8_t *restrict, const struct parsedmsg_s *restrict);
boolean_t pfkey_delete(uint8_t, uint32_t, sockaddr_u_t, sockaddr_u_t,
    boolean_t);

ikev2_spi_proto_t satype_to_ikev2(uint8_t);
void sadb_log(bunyan_level_t, const char *restrict, sadb_msg_t *restrict);

ikev2_spi_proto_t satype_to_ikev2(uint8_t);
uint8_t ikev2_to_satype(ikev2_spi_proto_t);

#ifdef __cplusplus
}
#endif

#endif /* _PFKEY_H */
