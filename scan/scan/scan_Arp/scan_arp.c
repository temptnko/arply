#include "if_setup.h"
#include "sock_setup_Arp"
#include "sock_setup_ioctl.h"

#include <stdint.h>

int main(){

    int fdArp = settupSocket();
    char ssid[64] = {0};
    
    if(getCommandOutput(command, &ssid, sizeof(ssid)) == -1){ 
        return -1;
    }
    
    if(fdArp == -1){
        return -1;
    }

    return 0;
}
