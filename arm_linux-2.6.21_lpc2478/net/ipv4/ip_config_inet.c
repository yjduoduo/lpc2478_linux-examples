/*
 *net/ipv4/ip_config_inet.c
 *take from include/net/ip.h file 
 */
#include <net/ip.h>
#include <net/dst.h>

#if defined(CONFIG_IPV6) || defined(CONFIG_IPV6_MODULE)
#include <linux/ipv6.h>
#endif


/* The function in 2.2 was invalid, producing wrong result for
* check=0xFEFF. It was noticed by Arthur Skawina _year_ ago. --ANK(000625) */
inline
int ip_decrease_ttl(struct iphdr *iph)
{
	u32 check = (__force u32)iph->check;
	check += (__force u32)htons(0x0100);
	iph->check = (__force __sum16)(check + (check>=0xFFFF));
	return --iph->ttl;
}

inline
int ip_dont_fragment(struct sock *sk, struct dst_entry *dst)
{
	return (inet_sk(sk)->pmtudisc == IP_PMTUDISC_DO ||
		(inet_sk(sk)->pmtudisc == IP_PMTUDISC_WANT &&
		!(dst_metric(dst, RTAX_LOCK)&(1<<RTAX_MTU))));
}


inline void ip_select_ident(struct iphdr *iph, struct dst_entry *dst, struct sock *sk)
{
	if (iph->frag_off & htons(IP_DF)) {
		/* This is only to work around buggy Windows95/2000
		* VJ compression implementations.  If the ID field
		* does not change, they drop every other packet in
		* a TCP stream using header compression.
		*/
		iph->id = (sk && inet_sk(sk)->daddr) ?
			htons(inet_sk(sk)->id++) : 0;
	} else
		__ip_select_ident(iph, dst, 0);
}

inline void ip_select_ident_more(struct iphdr *iph, struct dst_entry *dst, struct sock *sk, int more)
{
	if (iph->frag_off & htons(IP_DF)) {
		if (sk && inet_sk(sk)->daddr) {
			iph->id = htons(inet_sk(sk)->id);
			inet_sk(sk)->id += 1 + more;
		} else
			iph->id = 0;
	} else
		__ip_select_ident(iph, dst, more);
}

/*
*	Map a multicast IP onto multicast MAC for type ethernet.
*/

inline void ip_eth_mc_map(__be32 naddr, char *buf)
{
	__u32 addr=ntohl(naddr);
	buf[0]=0x01;
	buf[1]=0x00;
	buf[2]=0x5e;
	buf[5]=addr&0xFF;
	addr>>=8;
	buf[4]=addr&0xFF;
	addr>>=8;
	buf[3]=addr&0x7F;
}

/*
*	Map a multicast IP onto multicast MAC for type IP-over-InfiniBand.
*	Leave P_Key as 0 to be filled in by driver.
*/

inline void ip_ib_mc_map(__be32 naddr, char *buf)
{
	__u32 addr;
	buf[0]  = 0;		/* Reserved */
	buf[1]  = 0xff;		/* Multicast QPN */
	buf[2]  = 0xff;
	buf[3]  = 0xff;
	addr    = ntohl(naddr);
	buf[4]  = 0xff;
	buf[5]  = 0x12;		/* link local scope */
	buf[6]  = 0x40;		/* IPv4 signature */
	buf[7]  = 0x1b;
	buf[8]  = 0;		/* P_Key */
	buf[9]  = 0;
	buf[10] = 0;
	buf[11] = 0;
	buf[12] = 0;
	buf[13] = 0;
	buf[14] = 0;
	buf[15] = 0;
	buf[19] = addr & 0xff;
	addr  >>= 8;
	buf[18] = addr & 0xff;
	addr  >>= 8;
	buf[17] = addr & 0xff;
	addr  >>= 8;
	buf[16] = addr & 0x0f;
}


__inline__ void inet_reset_saddr(struct sock *sk)
{
	inet_sk(sk)->rcv_saddr = inet_sk(sk)->saddr = 0;
#if defined(CONFIG_IPV6) || defined(CONFIG_IPV6_MODULE)
	if (sk->sk_family == PF_INET6) {
		struct ipv6_pinfo *np = inet6_sk(sk);

		memset(&np->saddr, 0, sizeof(np->saddr));
		memset(&np->rcv_saddr, 0, sizeof(np->rcv_saddr));
	}
#endif
}