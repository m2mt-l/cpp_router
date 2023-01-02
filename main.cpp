# include <cstdint>
# include <fcntl.h>
# include <ifaddrs.h>
# include <iostream>
# include <unistd.h>
# include <arpa/inet.h>
# include <linux/if_ether.h>
# include <net/if.h>
# include <netpacket/packet.h>
# include <sys/ioctl.h>
# include <sys/socket.h>
# include <cstring>
// # include "log.h"
# include "net.h"

#define IGNORE_INTERFACES {"lo", "bond0", "dummy0", "tunl0", "sit0"}

bool is_ignore_interface(const char *ifname){
    char ignore_interfaces[][IF_NAMESIZE] = IGNORE_INTERFACES;
    for (int i = 0; i < sizeof(ignore_interfaces) / IF_NAMESIZE; i++){
        if(strcmp(ignore_interfaces[i], ifname) == 0){
            return true;
        }
    }
    return false;
}

net_device *get_net_device_by_name(const char *name){
    net_device *dev;
    for (dev = net_dev_list; dev; dev = dev->next){
        if(strcmp(dev->name, name) == 0)
            return dev;
    }
}


int main(){
    int sock = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if(sock == -1){
        perror("Failed to open socket");
        return 1;
    }
    unsigned char buf[1550];
    while(true){
        ssize_t n = recv(sock, buf, sizeof(buf), 0);
        if(n == -1){
            perror("Failed to receive");
            return 1;
        }
        if(n != 0){
            printf("Received %lu bites: ", n);
            for (int i = 0; i < n; i++){
                printf("%02x", buf[i]);
            }
            printf("\n");
        }
    }
};