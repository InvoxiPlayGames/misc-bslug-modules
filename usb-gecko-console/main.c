#include <bslug.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <rvl/dwc.h>
#include <rvl/OSThread.h>
#include "usb.h"

BSLUG_MODULE_GAME("????");
BSLUG_MODULE_NAME("USB Gecko Console");
BSLUG_MODULE_VERSION("1.0");
BSLUG_MODULE_AUTHOR("IPG, Chadderz, Nuke");
BSLUG_MODULE_LICENSE("MIT");

/* Replacement for DWC_SetLogMask.
 *  always enables all logging. Calls DWC_SetLogMask with 0xffffffff. */
static void Console_DWC_SetLogMask(DWC_LogType_t log_mask);
/* Replacement for fwrite.
 *  writes messages to the the sd card. File 1 is stdout and file 2 is stderr.
 *  All other calls passed through to the real fwrite. */
static size_t Console_fwrite(
    const void *ptr, size_t size, size_t nmemb, FILE *stream);

/* this method isn't in offline games; don't care if not patched. */
BSLUG_REPLACE(DWC_SetLogMask, Console_DWC_SetLogMask);
BSLUG_MUST_REPLACE(fwrite, Console_fwrite);

static void Console_DWC_SetLogMask(DWC_LogType_t log_mask) {
    /* call down to the real DWC_SetLogMask enabling all logging. */
    DWC_SetLogMask(0xffffffff);
}
static size_t Console_fwrite(
        const void *ptr, size_t size, size_t nmemb, FILE *stream) {    
    if (stream && ptr && (stream->fd == 1 || stream->fd == 2) && OSGetCurrentThread() != NULL) {
        /* stdout && stderr */
        if (!usb_checkgecko())
            goto skip;
        
        usb_sendbuffer(ptr, size * nmemb);
skip:
        fwrite(ptr, size, nmemb, stream);
        return nmemb;
    } else
        /* call down to real fwrite. */
        return fwrite(ptr, size, nmemb, stream);
}
