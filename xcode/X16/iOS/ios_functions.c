//
//  ios_functions.c
//  CommanderX16
//
//  Created by Pearce, Matthew (Senior Developer) on 04/01/2020.
//

#include "ios_functions.h"
#if __APPLE__
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE
// iOS device
#define OBJC_OLD_DISPATCH_PROTOTYPES 1

#include <objc/objc.h>
#include <objc/runtime.h>
#include <objc/message.h>
#elif TARGET_OS_MAC
// Other kinds of Mac OS
#else
#   error "Unknown Apple platform"
#endif
#endif
#include "utf8_encode.h"
#include <string.h>
#include <stdlib.h>

void createIosMessageObserver(void) {
    SEL alloc = sel_registerName("alloc");
    SEL init = sel_registerName("init");
    id MessageObserver = (id)objc_getClass("MessageObserver");
    id tmp = objc_msgSend(MessageObserver, alloc);
    objc_msgSend(tmp, init);
}

void sendNotification(const char *notification_name) {
    
    SEL alloc = sel_registerName("alloc");
    id NSString = (id)objc_getClass("NSString");
    SEL initWithCString_encoding = sel_registerName("initWithCString:encoding:");
    int NSUTF8StringEncoding = 4;
    id tmp2 = objc_msgSend(NSString, alloc);
    id notificationName = objc_msgSend(tmp2, initWithCString_encoding,
                                       notification_name, NSUTF8StringEncoding);
    id NSNotificationCenter = (id)objc_getClass("NSNotificationCenter");
    
    SEL defaultCenter = sel_registerName("defaultCenter");
    SEL postNotificationName = sel_registerName("postNotificationName:object:");
    
    id tmp = objc_msgSend(NSNotificationCenter, defaultCenter);
    
    objc_msgSend(tmp, postNotificationName, notificationName, NULL);
    
}

int loadBasFile(const char *bas_path) {
    bool pasting_bas = false;
    char paste_text_data[65536];
    char *paste_text = NULL;
    
    FILE *bas_file = fopen(bas_path, "r");
    if (!bas_file) {
        printf("Cannot open %s!\n", bas_path);
        exit(1);
    }
    paste_text = paste_text_data;
    size_t paste_size = fread(paste_text, 1, sizeof(paste_text_data) - 1, bas_file);
    paste_text[paste_size] = 0;

    fclose(bas_file);
    
    if (paste_text) {
        // ...paste BASIC code into the keyboard buffer
        pasting_bas = true;
    }
    
    while (pasting_bas) {
        uint32_t c;
        int e = 0;

    while (pasting_bas && RAM[NDX] < 10) {

        if (paste_text[0] == '\\' && paste_text[1] == 'X' && paste_text[2] && paste_text[3]) {
            uint8_t hi = strtol((char[]){paste_text[2], 0}, NULL, 16);
            uint8_t lo = strtol((char[]){paste_text[3], 0}, NULL, 16);
            c = hi << 4 | lo;
            paste_text += 4;
        } else {
            paste_text = utf8_decode(paste_text, &c, &e);
            
            c = iso8859_15_from_unicode(c);
        }
        if (c && !e) {
            RAM[KEYD + RAM[NDX]] = c;
            RAM[NDX]++;
        } else {
            pasting_bas = false;
            paste_text = NULL;
        }
        }
    }
    
    return 0;
}

int loadPrgFile(const char *prg_path) {
    bool pasting_bas = false;
    char paste_text_data[65536];
    char *paste_text = NULL;
    
    if (!prg_path) {
        return 1;
    }
    
    FILE *prg_file ;
    int prg_override_start = -1;
    
    char *comma = strchr(prg_path, ',');
    if (comma) {
        prg_override_start = (uint16_t)strtol(comma + 1, NULL, 16);
        *comma = 0;
    }

    prg_file = fopen(prg_path, "rb");
    if (!prg_file) {
        printf("Cannot open %s!\n", prg_path);
        return 1;
    }
    
    uint8_t start_lo = fgetc(prg_file);
    uint8_t start_hi = fgetc(prg_file);
    uint16_t start;
    if (prg_override_start >= 0) {
        start = prg_override_start;
    } else {
        start = start_hi << 8 | start_lo;
    }
    uint16_t end = start + fread(RAM + start, 1, 65536-start, prg_file);
    fclose(prg_file);
    prg_file = NULL;
    if (start == 0x0801) {
        // set start of variables
        
        RAM[VARTAB] = end & 0xff;
        RAM[VARTAB + 1] = end >> 8;
        
        paste_text = "RUN";
    } else {
        paste_text = paste_text_data;
        snprintf(paste_text, sizeof(paste_text_data), "SYS$%04X", start);
    }

    if (paste_text) {
        // ...paste BASIC code into the keyboard buffer
        pasting_bas = true;
    }
    
    while (pasting_bas && RAM[NDX] < 10) {
        uint32_t c;
        int e = 0;

        if (paste_text[0] == '\\' && paste_text[1] == 'X' && paste_text[2] && paste_text[3]) {
            uint8_t hi = strtol((char[]){paste_text[2], 0}, NULL, 16);
            uint8_t lo = strtol((char[]){paste_text[3], 0}, NULL, 16);
            c = hi << 4 | lo;
            paste_text += 4;
        } else {
            paste_text = utf8_decode(paste_text, &c, &e);
            c = iso8859_15_from_unicode(c);
        }
        if (c && !e) {
            RAM[KEYD + RAM[NDX]] = c;
            RAM[NDX]++;
        } else {
            pasting_bas = false;
            paste_text = NULL;
        }
    }
    
    return 0;
}
