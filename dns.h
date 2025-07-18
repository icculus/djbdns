#ifndef DNS_H
#define DNS_H

#include "stralloc.h"
#include "iopause.h"
#include "taia.h"
#include "clients.h"

#define DNS_C_IN "\0\1"
#define DNS_C_ANY "\0\377"

#define DNS_T_A "\0\1"
#define DNS_T_NS "\0\2"
#define DNS_T_CNAME "\0\5"
#define DNS_T_SOA "\0\6"
#define DNS_T_PTR "\0\14"
#define DNS_T_HINFO "\0\15"
#define DNS_T_MX "\0\17"
#define DNS_T_TXT "\0\20"
#define DNS_T_RP "\0\21"
#define DNS_T_SIG "\0\30"
#define DNS_T_KEY "\0\31"
#define DNS_T_AAAA "\0\34"
#define DNS_T_SRV "\0\41"
#define DNS_T_AXFR "\0\374"
#define DNS_T_ANY "\0\377"

struct dns_transmit {
  char *query; /* 0, or dynamically allocated */
  unsigned int querylen;
  char *packet; /* 0, or dynamically allocated */
  unsigned int packetlen;
  int s1; /* 0, or 1 + an open file descriptor */
  int tcpstate;
  unsigned int udploop;
  unsigned int curserver;
  struct taia deadline;
  unsigned int pos;
  unsigned const char *servers;
  unsigned char localip[16];
  unsigned int scope_id;
  char qtype[2];
  struct dns_transmit *master;
  struct dns_transmit *slaves[MAXUDP];
  int nslaves;
} ;

extern void dns_enable_merge(void (*logger)(const char *, const char *,
      const char *));

extern void dns_random_init(const char data[128]);
extern unsigned int dns_random(unsigned int);

extern void dns_sortip(unsigned char *,unsigned int);
extern void dns_sortip6(unsigned char *,unsigned int);

extern void dns_domain_free(char **);
extern int dns_domain_copy(char **,const char *);
extern unsigned int dns_domain_length(const char *);
extern int dns_domain_equal(const char *,const char *);
extern int dns_domain_suffix(const char *,const char *);
extern unsigned int dns_domain_suffixpos(const char *,const char *);
extern int dns_domain_fromdot(char **,const char *,unsigned int);
extern int dns_domain_todot_cat(stralloc *,const char *);

extern unsigned int dns_packet_copy(const char *,unsigned int,unsigned int,void *,unsigned int);
extern unsigned int dns_packet_getname(const char *,unsigned int,unsigned int,char **);
extern unsigned int dns_packet_skipname(const char *,unsigned int,unsigned int);

extern int dns_transmit_start(struct dns_transmit *d,const unsigned char servers[256],int flagrecursive,const char *q,const char qtype[2],const unsigned char localip[16]);
extern void dns_transmit_free(struct dns_transmit *);
extern void dns_transmit_io(struct dns_transmit *,iopause_fd *,struct taia *);
extern int dns_transmit_get(struct dns_transmit *,const iopause_fd *,const struct taia *);

extern int dns_resolvconfip(unsigned char s[256]);
extern int dns_resolve(const char *q,const char qtype[2]);
extern struct dns_transmit dns_resolve_tx;

extern int dns_ip4_packet(stralloc *,const char *,unsigned int);
extern int dns_ip4(stralloc *,const stralloc *);
extern int dns_ip6_packet(stralloc *,char *,unsigned int);
extern int dns_ip6(stralloc *,stralloc *);
extern int dns_name_packet(stralloc *,const char *,unsigned int);
#define DNS_NAME4_DOMAIN 31
extern void dns_name4_domain(char name[DNS_NAME4_DOMAIN],const unsigned char ip[4]);
extern int dns_name4(stralloc *out,const unsigned char ip[4]);
extern int dns_name6(stralloc *out,const unsigned char ip[16]);
extern int dns_txt_packet(stralloc *,const char *,unsigned int);
extern int dns_txt(stralloc *,const stralloc *);
extern int dns_mx_packet(stralloc *,const char *,unsigned int);
extern int dns_mx(stralloc *,const stralloc *);

extern int dns_resolvconfrewrite(stralloc *);
extern int dns_ip4_qualify_rules(stralloc *,stralloc *,const stralloc *,const stralloc *);
extern int dns_ip4_qualify(stralloc *,stralloc *,const stralloc *);
extern int dns_ip6_qualify_rules(stralloc *,stralloc *,const stralloc *,const stralloc *);
extern int dns_ip6_qualify(stralloc *,stralloc *,const stralloc *);

#define DNS_IP6_INT 0
#define DNS_IP6_ARPA 1

#define DNS_NAME6_DOMAIN (4*16+11)
extern int dns_name6_domain(char name[DNS_NAME6_DOMAIN],const unsigned char ip[16],int t);

#endif
