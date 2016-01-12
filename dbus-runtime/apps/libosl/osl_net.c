/*
 * osl_net.h
 *
 *  Created on: Dec 18, 2012
 *      Author: buttonfly
 */


#define CFG_CURL_SUPPORT		0


#include "osl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/rtnetlink.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <asm/types.h>
#include <netinet/ether.h>
#include <netinet/in.h>
#include <syslog.h>
#include <net/if.h>
#include <sys/types.h>
#include <net/if.h>
#include <linux/netlink.h>
#include <dirent.h>

#if CFG_CURL_SUPPORT
#include <curl/curl.h>
#endif


#include <arpa/inet.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/un.h>
#include <errno.h>

#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <arpa/inet.h>

#define BUFSIZE 8192

#include "osldbg.h"

typedef struct route_info {
	u_int dstAddr;
	u_int srcAddr;
	u_int gateWay;
	char ifName[IF_NAMESIZE];
} ROUTE_INFO;

static unsigned int RequestAddr(int sock);
static unsigned int parseRoutes(struct nlmsghdr *nlHdr,
		struct route_info *rtInfo);
static int readNlSock(int sock, char *buf, int seqNo);

u_int32_t osl_get_gateway_addr(void) {

	unsigned int addr = 0;
	int sock = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE);
	if (sock < 0) {
		fprintf(stderr, "failed to socket\n");
		return -1;
	}

	addr = RequestAddr(sock);

	close(sock);
	return addr;
}

unsigned int RequestAddr(int sock) {

	char msgBuf[BUFSIZE];
	memset(msgBuf, 0, BUFSIZE);

	struct nlmsghdr *nlMsg = (struct nlmsghdr *) msgBuf;

	int msgSeq = 0;
	unsigned int len = 0;

	nlMsg->nlmsg_len = NLMSG_LENGTH(sizeof(struct rtmsg)); // Length of message.
	nlMsg->nlmsg_type = RTM_GETROUTE; // Get the routes from kernel routing table .

	nlMsg->nlmsg_flags = NLM_F_DUMP | NLM_F_REQUEST; // The message is a request for dump.
	nlMsg->nlmsg_seq = msgSeq++; // Sequence of the message packet.
	nlMsg->nlmsg_pid = getpid(); // PID of process sending the request.

	if (send(sock, nlMsg, nlMsg->nlmsg_len, 0) < 0) {
		printf("Write To Socket Failed...\n");
		return -1;
	}

	if ((len = readNlSock(sock, msgBuf, msgSeq)) < 0) {
		printf("Read From Socket Failed...\n");
		return -1;
	}

	struct route_info* rtInfo = (struct route_info *) malloc(
			sizeof(struct route_info));

	unsigned int gateway;
	for (; NLMSG_OK(nlMsg,len); nlMsg = NLMSG_NEXT(nlMsg,len)) {
		memset(rtInfo, 0, sizeof(struct route_info));
		gateway = parseRoutes(nlMsg, rtInfo);
		if (gateway)
			break;
	}
	free(rtInfo);
	return gateway;
}

int readNlSock(int sock, char *buf, int seqNo) {
	struct nlmsghdr *nlHdr;
	unsigned int readLen = 0, msgLen = 0;

	do {
		if ((readLen = recv(sock, buf, BUFSIZE - msgLen, 0)) < 0) {
			perror("SOCK READ: ");
			return -1;
		}

		nlHdr = (struct nlmsghdr *) buf;

		if ((NLMSG_OK(nlHdr, readLen) == 0)
				|| (nlHdr->nlmsg_type == NLMSG_ERROR)) {
			perror("Error in recieved packet");
			return -1;
		}

		if (nlHdr->nlmsg_type == NLMSG_DONE) {
			break;
		} else {
			buf += readLen;
			msgLen += readLen;
		}

		if ((nlHdr->nlmsg_flags & NLM_F_MULTI) == 0) {
			break;
		}
	} while ((nlHdr->nlmsg_seq != (unsigned int) seqNo)
			|| (nlHdr->nlmsg_pid != (unsigned int) getpid()));
	return msgLen;
}

unsigned int parseRoutes(struct nlmsghdr *nlHdr, struct route_info *rtInfo) {
	struct rtmsg *rtMsg;
	struct rtattr *rtAttr;
	int rtLen;
	unsigned int gateway;

	rtMsg = (struct rtmsg *) NLMSG_DATA(nlHdr);

	if ((rtMsg->rtm_family != AF_INET) || (rtMsg->rtm_table != RT_TABLE_MAIN))
		return 0;

	rtAttr = (struct rtattr *) RTM_RTA(rtMsg);
	rtLen = RTM_PAYLOAD(nlHdr);
	for (; RTA_OK(rtAttr,rtLen); rtAttr = RTA_NEXT(rtAttr,rtLen)) {
		switch (rtAttr->rta_type) {
		case RTA_OIF:
			if_indextoname(*(int *) RTA_DATA(rtAttr), rtInfo->ifName);
			break;
		case RTA_GATEWAY:
			rtInfo->gateWay = *(u_int *) RTA_DATA(rtAttr);
			break;
		case RTA_PREFSRC:
			rtInfo->srcAddr = *(u_int *) RTA_DATA(rtAttr);
			break;
		case RTA_DST:
			rtInfo->dstAddr = *(u_int *) RTA_DATA(rtAttr);
			break;
		}
	}

	gateway = rtInfo->gateWay;

	return gateway;
}



u_int32_t osl_get_hwaddr(u_int8_t *addr) {
	struct ifreq ifr;
    struct ifreq *IFR;
    struct ifconf ifc;
    char buf[1024];
    int s, i;
    int ok = 0;

    s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s==-1) {
        return -1;
    }

    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = buf;
    ioctl(s, SIOCGIFCONF, &ifc);

    IFR = ifc.ifc_req;
    for (i = ifc.ifc_len / sizeof(struct ifreq); --i >= 0; IFR++) {
        strcpy(ifr.ifr_name, IFR->ifr_name);
        if (ioctl(s, SIOCGIFFLAGS, &ifr) == 0) {
            if (! (ifr.ifr_flags & IFF_LOOPBACK)) {
                if (ioctl(s, SIOCGIFHWADDR, &ifr) == 0) {
                    ok = 1;
                    break;
                }
            }
        }
    }

    close(s);
    if (ok) {
        bcopy( ifr.ifr_hwaddr.sa_data, addr, ETH_ALEN);
    }
    else {
        return -1;
    }
    return 0;
}

const char* osl_get_default_wlan_ifname(void)
{
#ifndef CONFIG_CTRL_IFACE_DIR
#define CONFIG_CTRL_IFACE_DIR "/var/run/wpa_supplicant"
#endif
	char *ifname = NULL;

	struct dirent *dent;
	DIR *dir = opendir(CONFIG_CTRL_IFACE_DIR);
	if (!dir) {
		return NULL;
	}
	while ((dent = readdir(dir))) {
#ifdef _DIRENT_HAVE_D_TYPE
		/*
		 * Skip the file if it is not a socket. Also accept
		 * DT_UNKNOWN (0) in case the C library or underlying
		 * file system does not support d_type.
		 */
		if (dent->d_type != DT_SOCK && dent->d_type != DT_UNKNOWN)
			continue;
#endif /* _DIRENT_HAVE_D_TYPE */
		if (strcmp(dent->d_name, ".") == 0 ||
		    strcmp(dent->d_name, "..") == 0)
			continue;
		printf("Selected interface '%s'\n", dent->d_name);
		ifname = strdup(dent->d_name);
		break;
	}
	closedir(dir);

	return ifname;
}

u_int32_t osl_get_ipaddr(const char* ifname)
{
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0) {
		return 0;
	}
	unsigned int addr;
	struct ifreq        ifr;
	struct sockaddr_in  *p = (struct sockaddr_in *)&(ifr.ifr_addr);
	memset(&ifr, 0, sizeof(ifr));
	strncpy(ifr.ifr_name, ifname, sizeof(ifr.ifr_name)-1);
	p->sin_family = AF_INET;

	int err = ioctl(sock, SIOCGIFADDR, &ifr);
	if(err < 0) {
		close(sock);
		return 0;
	}
	close(sock);

	if(err >= 0) {
		return p->sin_addr.s_addr;
	}
	return 0;
}

int osl_get_ip(const char* ifname, char* buf, size_t len) {
	int addr = 0;
	addr = osl_get_ipaddr(ifname);
	if(addr == 0) {
		return -1;
	}

	struct in_addr _ip;
	_ip.s_addr = addr;
	inet_ntop(AF_INET, &_ip, buf, len);
	return 0;
}

int osl_http_content_type(const char* uri, char* content_type) {


#if CFG_CURL_SUPPORT
	CURL* curl = curl_easy_init();
	if(!curl) {
		fprintf(stderr, "failed to <curl_easy_init()>\n");
		return -1;
	}

	curl_easy_setopt(curl , CURLOPT_URL,  uri) ;
	curl_easy_setopt(curl , CURLOPT_NOPROGRESS , 1) ;
	curl_easy_setopt(curl , CURLOPT_VERBOSE , 1) ;
	curl_easy_setopt(curl , CURLOPT_WRITEHEADER , stderr ) ;

	const CURLcode rc = curl_easy_perform(curl) ;
	if(CURLE_OK != rc) {
		fprintf(stderr, "Error from cURL : %s" ,curl_easy_strerror( rc ));
		curl_easy_cleanup(curl);
		return -1;
	}
	else {
		char* statString = NULL ;

		// Get Content-Type
		if( CURLE_OK == curl_easy_getinfo( curl , CURLINFO_CONTENT_TYPE , &statString ) ){
			M_("Content type: %s" , statString);
			strcpy(content_type, statString);
		}
	}

	if(curl) {
		curl_easy_cleanup(curl);
	}
#endif
	return 0;
}

int osl_http_download(const char* url, const char* dst) {

	S_;
	int r = 0;
#if CFG_CURL_SUPPORT
	fprintf(stderr, "download url: %s", url);
	FILE* fp=fopen(dst, "wt+");
	if(fp == NULL) {
		fprintf(stderr, "cannot create %s\n", dst);
		r = -1;
		goto error;
	}

	CURL* curl = curl_easy_init();
	if(!curl) {
		fprintf(stderr, "failed to <curl_easy_init()>\n");
		r = -1;
		goto error;
	}

	curl_easy_setopt(curl , CURLOPT_URL,  url) ;
	curl_easy_setopt(curl , CURLOPT_NOPROGRESS , 1) ;
	curl_easy_setopt(curl , CURLOPT_VERBOSE , 1) ;
	curl_easy_setopt(curl , CURLOPT_WRITEHEADER , stderr ) ;
	curl_easy_setopt(curl , CURLOPT_WRITEDATA , fp) ;

	const CURLcode rc = curl_easy_perform(curl) ;
	if(CURLE_OK != rc) {
		fprintf(stderr, "Error from cURL : %s" ,curl_easy_strerror( rc ));
		r = -1;
		goto error;
	}
	else {
		double statDouble ;
		long statLong ;
		char* statString = NULL ;
		char contentType[64] = {0,};

		// Get HTTP Response Code
		if( CURLE_OK == curl_easy_getinfo(curl , CURLINFO_HTTP_CODE , &statLong ) ){
			M_("Response code:  %d",  statLong);
		}

#if 0
		// Get Content-Type
		if( CURLE_OK == curl_easy_getinfo( curl , CURLINFO_CONTENT_TYPE , &statString ) ){
			M_("Content type: %s" , statString);
			strcpy(contentType, statString);
		}
#endif

#if 0
		// Get size of Download object
		if( CURLE_OK == curl_easy_getinfo( curl , CURLINFO_SIZE_DOWNLOAD , &statDouble ) ){
			M_("Download size: %d bytes ", statDouble);
		}
		//
		if( CURLE_OK == curl_easy_getinfo( curl , CURLINFO_SPEED_DOWNLOAD , &statDouble ) ){
			M_("Download speed: %f bytes/sec", statDouble);
		}
#endif
	}

error:

	if(curl) {
		curl_easy_cleanup(curl);
	}

	if(fp) {
		fclose(fp);
	}
	fp = NULL;

#endif
	return r;
}

int osl_get_rx(const char* alias) {
	char tmp[1024] = {0};
	sprintf(tmp, "/sys/class/net/%s/statistics/rx_bytes", alias);

	FILE* fp = fopen(tmp, "r");
	if(fp == NULL) {
		ERR("cannot open %s", tmp);
		return -1;
	}

	int bytes;
	fscanf(fp, "%d\n", &bytes);
	bytes *= 8;

	fclose(fp);
	return bytes;
}

int osl_get_tx(const char* alias) {
	char tmp[1024] = {0};
	sprintf(tmp, "/sys/class/net/%s/statistics/tx_bytes", alias);

	FILE* fp = fopen(tmp, "r");
	if(fp == NULL) {
		ERR("cannot open %s", tmp);
		return -1;
	}

	int bytes;
	fscanf(fp, "%d\n", &bytes);
	bytes *= 8;

	fclose(fp);
	return bytes;
}




int osl_get_host(char* host, _out_ char* ip) {
	int r = 0;
	struct hostent* h = NULL;
	h = gethostbyname(host);
	if(!h)
		return -1;

	struct in_addr** addr_list = (struct in_addr**)h->h_addr_list;
	if(addr_list[0]) {
		strcpy(ip, inet_ntoa(*addr_list[0]));
	}
#if fixme
	for(int i = 0; addr_list[i] != NULL; i++) {
		strcpy(ip, inet_ntoa(*addr_list[i]));
		break;
	}
#endif
	return 0;
}

static int CONNECT_TIMEOUT = 0;
static int RECV_TIMEOUT = 0;
static int _connect(char* ip, uint16_t port, int type) {
	int sock = -1;
    if ((sock = socket(AF_INET, type, 0)) < 0) {
    	return -1;
    }

#if 0
    int st, tmp;
    tmp = st  = fcntl(sock, F_GETFL, NULL);
    if(tmp < 0) {
    	m_("failed to get fcntl");
    	close(sock);
    	return -1;
    }

    tmp |= O_NONBLOCK;
    if(fcntl(sock, F_SETFL, tmp) < 0) {
    	m_("failed to set to non-block");
    	close(sock);
    	return -1;
    }

    // @author: buttonfly
    // issue no.286
    char tmp2[512];
    if(osl_get_host(ip, tmp2) == -1) {
    	// TODO
    	fprintf(stderr, "cannot resolve host name: %s\n", ip);
    }

    struct sockaddr_in serveraddr;
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(tmp2);
    serveraddr.sin_port = htons(port);


    int client_len = sizeof(serveraddr);
    connect(sock, (struct sockaddr *)&serveraddr, client_len); // NON-BLOCK

    struct timeval tval = {0};
    struct timeval* tv = NULL;
    if(CONNECT_TIMEOUT > 0) {
        tval.tv_sec = CONNECT_TIMEOUT ;
        tval.tv_usec = 0;
        tv = &tval;
    }

    fd_set                readfds, writefds;
    FD_ZERO(&readfds);
    FD_SET(sock, &readfds);
    int  fd_max = sock;

    writefds = readfds;
    int  ret = select(fd_max + 1, &readfds, &writefds, 0, tv);
    if(ret > 0) {
        fcntl(sock, F_SETFL, st);
    }
    else {
    	m_("failed to connect : %s:%d", ip, port);
		fcntl(sock, F_SETFL, st);
		close(sock);
		sock = -1;
	}

    if (FD_ISSET(sock, &readfds) || FD_ISSET(sock, &writefds) ) {
    	// TODO
    }

#else
    struct sockaddr_in serveraddr;
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(ip);
    serveraddr.sin_port = htons(port);

    int client_len = sizeof(serveraddr);
	if (connect(sock, (struct sockaddr *)&serveraddr, client_len) < 0) {
		m_("failed to connect: %s:%d", ip, port);
		close(sock);
		return -1;
	}
#endif

    return sock;
}

int osl_connect(char* ip, uint16_t port) {
	m_("connect to: %s", ip);
	return _connect(ip, port, SOCK_STREAM);
}

int osl_udpconnect(char* ip, short port)  {
	m_("connect to: %s", ip);
	return _connect(ip, port, SOCK_DGRAM);
}

int osl_send(int sck, char* buf, int len) {
	int flags = 0;
	return send(sck, buf, len, flags);
}

int osl_recv(int sck, char* buf, int len, int timeout){
	int ret = -1;
	int rc = -1;
	fd_set readfds;
	struct timeval to = { 0 };
	struct timeval *p_to = NULL;
	int flags = 0; // TODO

	FD_ZERO(&readfds);
	FD_SET(sck, &readfds);

	if (0 != timeout) {
		to.tv_sec = timeout / 1000;
		to.tv_usec = (timeout % 1000) * 1000;
		p_to = &to;
	}

	do
		rc = select(sck + 1, &readfds, NULL, NULL, p_to);
	while (EINTR == errno);
	if (-1 == rc) {
//		PERROR("select");
		ERR("select error");
	}

#if 1
	else if (0 < rc) {
		rc = recv(sck, buf, len, flags);
		if (-1 == rc)
			ERR("recv");
		ret = rc;
	}
#else
	else if (0 < rc)
	{
		/*Bug #264 : abnormal vod page */
		rc = recv(sd + 1, buf, size, flags);
		if (-1 == rc)
			PERROR("recv");
		ret = rc;
	}
#endif
	/* Else it timed out */
	//    printf("\nExiting : %s\n", __func__);
//	H_(buf, size);
	return ret;
}

int osl_disconnect(int sck) {
	close(sck);
	return 0;
}

static bool is_multicast(const struct sockaddr_storage *saddr, socklen_t len) {
    const struct sockaddr *addr = (const struct sockaddr *) saddr;

    switch(addr->sa_family) {
#if defined(IN_MULTICAST)
        case AF_INET: {
            const struct sockaddr_in *ip = (const struct sockaddr_in *)saddr;
            if ((size_t)len < sizeof (*ip))
                return false;
            return IN_MULTICAST(ntohl(ip->sin_addr.s_addr)) != 0;
        }
#endif
#if defined(IN6_IS_ADDR_MULTICAST)
        case AF_INET6: {
            const struct sockaddr_in6 *ip6 = (const struct sockaddr_in6 *)saddr;
            if ((size_t)len < sizeof (*ip6))
                return false;
            return IN6_IS_ADDR_MULTICAST(&ip6->sin6_addr) != 0;
        }
#endif
    }
    return false;
}


#if MULTICAST_SUPPORT
typedef struct {
	int sockfd;
	int client_addr_len;
	struct ip_mreq mreq;
	struct sockaddr_in groupaddr;
	struct in_addr iaddr;
} tagconn;


conn* osl_multicast_server_open(const char *alias, const char* ip, short port)
{
	tagconn* con = (tagconn*) malloc(sizeof(tagconn));
	KASSERT(con);
	memset(con, 0, sizeof(conn));
	unsigned char ttl = 3;
	unsigned char one = 1;

	con->sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	KASSERT(con->sockfd > 0);
	if(con->sockfd < 0) {
		goto error;
	}

	memset(&con->groupaddr, 0, sizeof(con->groupaddr));
	con->groupaddr.sin_family = PF_INET;
	con->groupaddr.sin_port = htons(port); // Use the first free port
	con->groupaddr.sin_addr.s_addr =  inet_addr(ip); // bind socket to any interface
	con->client_addr_len = sizeof(con->groupaddr);
	bind(con->sockfd, (struct sockaddr *)&(con->groupaddr), sizeof(struct sockaddr_in));
	if(alias == NULL) alias = "eth0";
	con->iaddr.s_addr = osl_get_ipaddr(alias);
	setsockopt(con->sockfd, IPPROTO_IP, IP_MULTICAST_IF, &con->iaddr, sizeof(struct in_addr));
	setsockopt(con->sockfd, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(unsigned char));
	setsockopt(con->sockfd, IPPROTO_IP, IP_MULTICAST_LOOP,&one, sizeof(unsigned char));

	return (conn *) con;

error:
	if(con != NULL) {
		free(con);
		con = NULL;
	}
	return (conn *) NULL;
}

void osl_multicast_server_send(conn* con, const char *data, int len)
{

	tagconn *conn = (tagconn *) con;
	sendto(conn->sockfd, data, len, 0,(struct sockaddr *)&conn->groupaddr, conn->client_addr_len);
}

void osl_multicast_server_close (conn* con)
{
	tagconn *c = (conn *) con;
	close(c->sockfd);
	free(c);
}
#endif

