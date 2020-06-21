//
//  main.c
//  lab2003
//
//  Created by Hoàng Minh Trị on 6/14/20.
//  Copyright © 2020 Hoang Minh Tri. All rights reserved.
//

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
    int main(void) {
    int x = 266;
    char s[] = "test";
    FILE *f;
    f = fopen("out1", "w");         // Open in translating Mode
    fprintf(f, "%i\n%s", x, s);     // 266\r\ntest -> (9byte), \r -> 0D, \n -> 0A
    fclose(f);
        
    f = fopen("out2", "wb");        // Open in non translating Mode
    fprintf(f, "%i\n%s", x, s);     // 266\ntest -> 8byte
    fclose(f);
        
    f = fopen("out3", "w");         // Open in translating Mode
    fwrite(&x, sizeof x, 1, f);     // 266 -> 0x0000010A -> 0D 0A 01 00 00
    fwrite(&s, sizeof s, 1, f);     // 5byte -> test 00
    fclose(f);                      // 10 byte
        
    f = fopen("out4", "wb");        // Open in non translating Mode
    fwrite(&x, sizeof x, 1, f);     // 0A 01 00 00
    fwrite(&s, sizeof s, 1, f);     // 5byte -> test 00
    fclose(f);                      // 9 byte
        
    return 0;
    
}
