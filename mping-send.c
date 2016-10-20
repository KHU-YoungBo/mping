#include<netinet/in.h>
#include<errno.h>
#include<netdb.h>
#include<stdio.h> //For standard things
#include<stdlib.h>    //malloc
#include<string.h>    //strlen
 
#include<netinet/ip_icmp.h>   //Provides declarations for icmp header
#include<netinet/udp.h>   //Provides declarations for udp header
#include<netinet/tcp.h>   //Provides declarations for tcp header
#include<netinet/ip.h>    //Provides declarations for ip header
#include<netinet/if_ether.h>  //For ETH_P_ALL
#include<net/ethernet.h>  //For ether_header
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/ioctl.h>
#include<sys/time.h>
#include<sys/types.h>
#include<unistd.h>

#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/ethernet.h> /* the L2 protocols */

#include <stdio.h>
#include <string.h>
#include "mping.h"

#define ARGV_INDEX_DEV	(1)
#define ARGV_INDEX_MACARR	(2)

static int _get_iface_index(int socket, const char *iface_name) {
        struct ifreq ifr;

        // Ugly hard coded, will be changed
        memset(&ifr, 0, sizeof(struct ifreq));
        strncpy((char *)ifr.ifr_name, iface_name, IFNAMSIZ);
        if (ioctl(socket, SIOCGIFINDEX, &ifr) < 0){ 
                perror("ioctl: ");
		exit(EXIT_FAILURE);
        }

        return ifr.ifr_ifindex;
        // Always success here 2 for eth0, 3 for eth1
} // _get_iface_index()

static int _link_fd(uint16_t ethertype, int broadcastEnable)
{

        int err = -1; 
        int fd  = -1; 

        fd = socket(AF_PACKET, SOCK_DGRAM ,htons(ethertype));
        if(fd < 0) {
                perror("socket()");
		exit(EXIT_FAILURE);
        }

	if(broadcastEnable != 0)
	{
		err = setsockopt(fd,
			SOL_SOCKET,
			SO_BROADCAST,
			&broadcastEnable,
			sizeof(broadcastEnable));
		if(err < 0) {
			perror("setsockopt()");
			exit(EXIT_FAILURE);
		}
	}

        return fd; 
} // _link_fd()

static void _bind_fd(
                /* output. it will be filed. */
                struct sockaddr_ll *ifaddr,
                /* input. sock_raw should be AF_PACKET && SOCKDGRAM */
                int sock_raw,
                /* input. ex) "eth0" */
                const char *iface_name)
{
        socklen_t len = sizeof(struct sockaddr_ll);

        ifaddr->sll_family = PF_PACKET; 
        ifaddr->sll_ifindex =_get_iface_index(sock_raw, iface_name);
        ifaddr->sll_protocol = 0;       // this field will be ignored
                                        // because the socket was created with protocol field.

        if((bind(sock_raw, (const struct sockaddr *)ifaddr, sizeof(*ifaddr))) == -1)
        {
                perror("bind: ");
		exit(EXIT_FAILURE);
        }

        // get mac address which is bound to interface. 
        getsockname(sock_raw, (struct sockaddr*)ifaddr, &len);

        return;
} // _bind_fd()

int main(int argc, char **argv)
{
	int broadcastEnable = 0;
	int fd = _link_fd(ETHERTYPE_MPING, broadcastEnable);
	struct sockaddr_ll addr_src[1];
	struct sockaddr_ll addr_dst[1];
	uint16_t mac[6];
	char buf[1024] = "Hello MPING";
	size_t buflen = strlen(buf);

	sscanf(argv[ARGV_INDEX_MACARR],"%hx:%hx:%hx:%hx:%hx:%hx",
			mac+0,
			mac+1,
			mac+2,
			mac+3,
			mac+4,
			mac+5);


	_bind_fd(addr_src, fd, argv[ARGV_INDEX_DEV]);

	addr_dst->sll_family	= AF_PACKET;
	addr_dst->sll_protocol	= htons(ETHERTYPE_MPING);
	addr_dst->sll_ifindex	= addr_src->sll_ifindex;
	addr_dst->sll_hatype	= ARPHRD_ETHER;
	addr_dst->sll_pkttype	= PACKET_OTHERHOST;
	addr_dst->sll_halen	= 6;
	memcpy(addr_dst->sll_addr, mac, 6);

	for(int i=1;;i++)
	{
		int sendbyte = sendto	(fd,
				buf,
				buflen,
				0, /* flags */
				(struct sockaddr*)addr_dst,
				sizeof(addr_dst)
				);

		printf("#%d(%d bytes)\n", i, sendbyte);


		sleep(1);
	}
	

	return 0;
}







