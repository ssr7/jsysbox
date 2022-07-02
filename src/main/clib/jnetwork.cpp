/*
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <jni.h>
#include "jnetwork.h"

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <list>
#include <errno.h>

#include <net/if.h>
#include <net/if.h>
#include <net/ethernet.h>
#include <net/route.h>

#include <netinet/in.h>
#include <netinet/ip.h>

#include <arpa/inet.h>

#include <sys/ioctl.h>
#include <sys/socket.h>

#include <linux/if_link.h>
#include <linux/types.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>

#include <sys/types.h>

#include <bits/ioctls.h>
#include <string>

using namespace std ;

class Network {
private:
    // char host[NI_MAXHOST];
    bool is_ipv4_address(const string& str,struct sockaddr_in& sa)
    {
        return inet_pton(AF_INET, str.c_str(), &(sa.sin_addr))!=0;
    }

    bool is_ipv6_address(const string& str,struct sockaddr_in6& sa)
    {
        return inet_pton(AF_INET6, str.c_str(), &(sa.sin6_addr))!=0;
    }

public:

    int validate_ip(const string& ipstr,int af) {
        struct sockaddr_in sa;
        struct sockaddr_in6 sa6;

        if (af == AF_INET && is_ipv4_address(ipstr,sa)) return 0 ;
        if (af == AF_INET6 && is_ipv6_address(ipstr,sa6)) return 0;

        fprintf(stderr,"Invalid ip address.\n");
        return 1;
    }

    struct in_addr get_network_addr(struct in_addr ip_addr, struct in_addr netmask_addr) {
        struct in_addr network_addr;
        memset(&network_addr, 0, sizeof(network_addr));
        network_addr.s_addr  = ip_addr.s_addr & netmask_addr.s_addr ;
        return network_addr;
    }

    list<string> interfaces() {
        struct ifaddrs *ifaddrs ;
        if (getifaddrs(&ifaddrs) == -1) {
            perror("getifaddrs");
            exit(EXIT_FAILURE);
        }

        list<string> names ;
        for (struct  ifaddrs *ifa = ifaddrs ; ifa != NULL ; ifa = ifa->ifa_next)
        {
            if (ifa->ifa_addr == NULL) continue ;
            if (ifa->ifa_addr->sa_family == AF_PACKET) {
                names.push_back(ifa->ifa_name);
            }
        }
        freeifaddrs(ifaddrs);
        return names ;
    }

    int mac_addr(const string name,string& mac_addr) {
        int s;
        struct ifreq buffer;

        s = socket(PF_INET, SOCK_DGRAM, 0);
        memset(&buffer, 0x00, sizeof(buffer));
        strcpy(buffer.ifr_name, name.data());
        ioctl(s, SIOCGIFHWADDR, &buffer);
        close(s);

        char *data = buffer.ifr_hwaddr.sa_data;

        char* m = new char ;
        sprintf(m,
                "%02x:%02x:%02x:%02x:%02x:%02x",
                (unsigned char)data[0],
                (unsigned char)data[1],
                (unsigned char)data[2],
                (unsigned char)data[3],
                (unsigned char)data[4],
                (unsigned char)data[5]);
        mac_addr.append(m);
        return 0 ;
    }

    int get_ipaddr(string name, bool ipv4,bool netmask,string& ip) {
        void * tmpAddrPtr;
        struct ifaddrs *ifaddrs ;
        if (getifaddrs(&ifaddrs) == -1) {
            perror("getifaddrs");
            exit(EXIT_FAILURE);
        }


        for (struct  ifaddrs *ifa = ifaddrs ; ifa != NULL ; ifa = ifa->ifa_next)
        {
            if (ifa->ifa_addr == NULL) continue ;
            int family = ifa->ifa_addr->sa_family ;

            if (family == AF_INET || family == AF_INET6) {
                tmpAddrPtr = netmask ?
                             &((struct sockaddr_in *)(ifa->ifa_netmask))->sin_addr :
                             &((struct sockaddr_in *)(ifa->ifa_addr))->sin_addr ;
            }

            if (strcmp(ifa->ifa_name,name.data()) == 0 && family == AF_INET && ipv4) {
                ip.resize(INET_ADDRSTRLEN);
                inet_ntop(AF_INET, tmpAddrPtr, (char*) ip.data(), INET_ADDRSTRLEN);
                return 0 ;
            } else if (strcmp(ifa->ifa_name,name.data()) == 0 && family == AF_INET6 && !ipv4) {
                ip.resize(INET6_ADDRSTRLEN);
                inet_ntop(AF_INET6, tmpAddrPtr, (char*) ip.data(), INET6_ADDRSTRLEN);
                return 0 ;
            }
        }
        freeifaddrs(ifaddrs) ;
        return 1 ;
    }


    int link_data(string name,struct rtnl_link_stats *&stats) {
        struct ifaddrs *ifaddrs ;
        if (getifaddrs(&ifaddrs) == -1) {
            perror("getifaddrs");
            exit(EXIT_FAILURE);
        }


        for (struct  ifaddrs *ifa = ifaddrs ; ifa != NULL ; ifa = ifa->ifa_next)
        {
            if (ifa->ifa_addr == NULL) continue ;
            int family = ifa->ifa_addr->sa_family ;

            if (strcmp(ifa->ifa_name,name.data()) == 0 && family == AF_PACKET && ifa->ifa_data != NULL) {
                stats = (struct rtnl_link_stats *) ifa->ifa_data;
                return 0 ;
            }
        }
        freeifaddrs(ifaddrs);
        return 1 ;
    }


    /**
    * Set ip
    */
    int set_ip(string& iface_name, string& ip_addr, string& netmask)
    {
	int r ;
        int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
        if(sockfd == -1) {
            fprintf(stderr, "Could not get socket.\n");
            return -1;
        }
        if(!iface_name.data()) return -1;

        struct ifreq ifr;
        struct sockaddr_in* addr = (struct sockaddr_in*)&ifr.ifr_addr;

        ifr.ifr_addr.sa_family = AF_INET;

        /* get interface name */
        strncpy(ifr.ifr_name, iface_name.data(), IFNAMSIZ);

        /* Set ip address */
        ifr.ifr_addr.sa_family = AF_INET;
        inet_pton(AF_INET, ip_addr.data(), &addr->sin_addr);
        r = ioctl(sockfd, SIOCSIFADDR, &ifr);

        /* Set netmask */
        inet_pton(AF_INET, netmask.data(), &addr->sin_addr);
        r = ioctl(sockfd, SIOCSIFNETMASK, &ifr);

	if (r < 0) return -1 ;
        /* Bring up interface */
        ioctl(sockfd, SIOCGIFFLAGS, &ifr);
        strncpy(ifr.ifr_name, iface_name.data(), IFNAMSIZ);
        ifr.ifr_flags |= (IFF_UP | IFF_RUNNING);

        close(sockfd) ;
        return 0;
    }

    int update_route(string& addr_str,string& netmask_str, string& gateway_str, string& ethernet,int metrics,bool isHost,bool del) {
        struct in_addr addr  ;
        struct in_addr netmask  ;
        struct in_addr gateway  ;
        struct in_addr network  ;

        if (!netmask_str.empty() && inet_aton((char*) netmask_str.c_str(), &netmask) == 1) {
            if (isHost) {
                inet_aton("255.255.255.255",&netmask) ;
            }
        } else {
            fprintf(stderr, "Invalid address\n");
            return 1;
        }

        if (!addr_str.empty() && inet_aton((char*) addr_str.c_str(), &addr) == 1) {
            if (!isHost) {
                network = get_network_addr(addr,netmask) ;
            }
        } else {
            fprintf(stderr, "Invalid address\n");
            return 1;
        }

        if (gateway_str.empty()) {
            inet_aton("0.0.0.0", &gateway) ;
        } else  if (inet_aton((char*) gateway_str.c_str(), &gateway) != 1) {
            fprintf(stderr, "Invalid gateway\n");
            return 1;
        }

        struct ifreq ifr;
        struct sockaddr_in* saddr = (struct sockaddr_in*)&ifr.ifr_addr;

        struct rtentry route;
        int err = 0;
        memset(&route, 0, sizeof(route));

        /*
        set gateway
        */
        saddr = (struct sockaddr_in*) &route.rt_gateway;
        saddr->sin_family = AF_INET;
        saddr->sin_addr.s_addr = gateway.s_addr;

        /*
        set netmask
        */
        saddr = (struct sockaddr_in*) &route.rt_genmask;
        saddr->sin_family = AF_INET;
        saddr->sin_addr.s_addr = netmask.s_addr ;

        /*
        set target
        */
        saddr = (struct sockaddr_in*) &route.rt_dst;
        saddr->sin_family = AF_INET;
        if (isHost) {
            saddr->sin_addr.s_addr = addr.s_addr;
        } else {
            saddr->sin_addr.s_addr = network.s_addr;
        }

        int flag = RTF_UP ;
        if (isHost) {
            flag = flag | RTF_HOST;
        }
        if (!gateway_str.empty()) {
            flag = flag | RTF_GATEWAY ;
        }
        if (del) {
            flag = flag & RTF_REJECT ;
            route.rt_metric = 0;
        }

        route.rt_flags = flag;
        if (!ethernet.empty()) {
            route.rt_dev = (char*) ethernet.c_str();
        }

        route.rt_metric = metrics;
        int sockfd = socket( PF_INET, SOCK_DGRAM, IPPROTO_IP );
        if (del) {
            err = ioctl(sockfd, SIOCDELRT, &route )  ;
        } else {
            err = ioctl(sockfd, SIOCADDRT, &route);
        }
        if ((err) < 0) {
            fprintf(stderr, "ioctl: %s\n",  "Route Error");
            return -1;
        }

        close(sockfd) ;
        return 1;
    }
} ;


jobject mapToEthernet(JNIEnv *env, const string iface_name,Network& network) {
    jclass ethernetClazz = env->FindClass("ir/moke/jsysbox/network/Ethernet");
    jmethodID ethernetConstructor = env->GetMethodID(ethernetClazz,
                                    "<init>",
                                    "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;IIIIII)V");
    string mac_addr;
    string ip ;
    string netmask;
    network.mac_addr(iface_name,mac_addr) ;
    network.get_ipaddr(iface_name,true,false,ip);
    network.get_ipaddr(iface_name,true,true,netmask);

    struct rtnl_link_stats* link_data;
    network.link_data(iface_name,link_data);
    int rx_pkt = link_data->rx_packets ;
    int rx_bytes = link_data->rx_bytes ;
    int rx_err = link_data->rx_errors ;
    int tx_pkt = link_data->tx_packets ;
    int tx_bytes = link_data->tx_bytes ;
    int tx_err = link_data->tx_errors ;

    jvalue args[10];
    args[0].l = env->NewStringUTF(iface_name.c_str());
    args[1].l = env->NewStringUTF(mac_addr.c_str());
    args[2].l = env->NewStringUTF(ip.c_str());
    args[3].l = env->NewStringUTF(netmask.c_str());
    args[4].i = rx_pkt ;
    args[5].i = rx_bytes ;
    args[6].i = rx_err ;
    args[7].i = tx_pkt ;
    args[8].i = tx_bytes ;
    args[9].i = tx_err ;

    return env->NewObjectA(ethernetClazz,ethernetConstructor,args);
}

JNIEXPORT jobject JNICALL Java_ir_moke_jsysbox_network_JNetwork_ethernet (JNIEnv *env, jclass, jstring jiface_name) {
    Network network ;
    string iface_name = env->GetStringUTFChars(jiface_name,0);
    return mapToEthernet(env,iface_name,network);
}

JNIEXPORT jobjectArray JNICALL Java_ir_moke_jsysbox_network_JNetwork_ethernetList (JNIEnv *env, jclass) {
    jclass ethernetClazz = env->FindClass("ir/moke/jsysbox/network/Ethernet");
    Network network ;
    int index = 0 ;
    list<string> interfaces = network.interfaces() ;
    list<string>::iterator it ;

    jobjectArray result = env->NewObjectArray(interfaces.size(),ethernetClazz,NULL);
    for (it=interfaces.begin() ; it != interfaces.end() ; ++it) {

        string iface_name = it->c_str() ;
        jobject o = mapToEthernet(env,iface_name,network);
        env->SetObjectArrayElement(result,index,o);
        env->DeleteLocalRef(o);

        ++index ;
    }
    return result;
}

JNIEXPORT jobject JNICALL Java_ir_moke_jsysbox_network_JNetwork_setIp (JNIEnv *env, jclass clazz, jstring jiface, jstring jaddr, jstring jnetmask) {
    jclass jexception = env->FindClass("ir/moke/jsysbox/JNetworkException");
    Network network ;
    string iface = env->GetStringUTFChars(jiface,0);
    string addr = env->GetStringUTFChars(jaddr,0);
    string netmask = env->GetStringUTFChars(jnetmask,0);
    int r = network.set_ip(iface,addr,netmask) ;

    if (r != 0) {
        const char *err = "Failed to set ip address" ;
        env->ThrowNew(jexception,err);
    }

    return mapToEthernet(env,iface,network);
}

JNIEXPORT void JNICALL Java_ir_moke_jsysbox_network_JNetwork_updateRoute 
(JNIEnv *env, jclass, jstring jdestination, jstring jnetmask, jstring jgateway, jstring jiface, jint jmetrics, jboolean jIsHost,jboolean jdel) {

	string iface = ""; 
	string gateway = "";
	if (jiface != NULL) iface = env->GetStringUTFChars(jiface,0) ;
	if (jgateway != NULL) gateway = env->GetStringUTFChars(jgateway,0) ;

	string destination = env->GetStringUTFChars(jdestination,0) ;
	string netmask = env->GetStringUTFChars(jnetmask,0) ;
	bool isHost = bool(jIsHost) ;
	bool del = bool(jdel) ;
	int metrics = (int) jmetrics;

	Network network ;
	network.update_route(destination,
			netmask,
			gateway ,
			iface,
			metrics,
			isHost,
			del) ;
}
