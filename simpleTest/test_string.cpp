// #define printf(x, ...)

#include <stdio.h>
#include <string>
#include <string.h>

// #define TTXOSD_DEBUG
// #define printf(x, ...)
// #define printf(...)

int main() {
/*
    int temp = 49;
    printf("%c\n", static_cast<char>(0+temp));
*/
/*
    unsigned char zoneId[6];
    memset(zoneId, 0x00, sizeof(zoneId));
    zoneId[5] = 0x15;
    for(int i = 0; i < 6; i++) {
        printf("uchar zoneId[%d] = 0x%02X\n", i, zoneId[i]);
    }
    printf("zoneId[5] = %c\n", zoneId[5]);

    std::string ZoneId("15");
    printf("string ZoneId(%s)\n", ZoneId.c_str());
*/

    char cTitle[50] = "";
    std::string strTitle = "Testing";

    printf("cTitle(%s), strTitle(%s)\n", cTitle, strTitle.c_str());

    strcpy(cTitle, strTitle.c_str());
    
    printf("cTitle(%s), strTitle(%s)\n", cTitle, strTitle.c_str());
    
    return 0;
}
