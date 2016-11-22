#ifndef ETHER_ADDR_H
#define ETHER_ADDR_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#ifndef ETHER_ADDR_LEN
#define ETHER_ADDR_LEN		(6)
#endif

#define ETHER_STR_LEN		(20)

/*
 * Binary format */
typedef uint8_t ether_addr_t[ETHER_ADDR_LEN];

/* 
 * string form of mac address.
 * ex) ff:ff:ff:ff:ff:ff. include \0 character */
typedef char ether_str_t[ETHER_STR_LEN];

#ifdef __cplusplus
extern "C" {
#endif

inline void ether_addr_to_str(ether_addr_t addr, ether_str_t str);
inline void ether_str_to_addr(ether_str_t str, ether_addr_t addr);
inline int ether_addr_cmp(ether_addr_t addr1, ether_addr_t addr2);

#ifdef __cplusplus
};
#endif /* __cplusplus */


void ether_addr_to_str(ether_addr_t addr, ether_str_t str)
{
	unsigned int buf[ETHER_ADDR_LEN];

	buf[0] = addr[0];
	buf[1] = addr[1];
	buf[2] = addr[2];
	buf[3] = addr[3];
	buf[4] = addr[4];
	buf[5] = addr[5];

	sprintf(str, "%02x:%02x:%02x:%02x:%02x:%02x",
			buf[0],
			buf[1],
			buf[2],
			buf[3],
			buf[4],
			buf[5]);

	return;
}

void ether_str_to_addr(ether_str_t str, ether_addr_t addr)
{
	unsigned int buf[ETHER_ADDR_LEN];

	sscanf(str, "%02x:%02x:%02x:%02x:%02x:%02x",
			buf + 0,
			buf + 1,
			buf + 2,
			buf + 3,
			buf + 4,
			buf + 5);

	addr[0] = buf[0];
	addr[1] = buf[1];
	addr[2] = buf[2];
	addr[3] = buf[3];
	addr[4] = buf[4];
	addr[5] = buf[5];

	return;
}


int ether_addr_cmp(ether_addr_t addr1, ether_addr_t addr2)
{
	return memcmp(addr1, addr2, sizeof(ether_addr_t));
}

#endif /* ETHER_ADDR_H */
