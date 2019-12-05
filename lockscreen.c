/*
 * Copyright (C) 2016-2018 Claudio Luck <cluck@posteo.ch>
 * 
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <ApplicationServices/ApplicationServices.h>
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOMessage.h>


extern void SACLockScreenImmediate ();
#define BLINK 127000

int main (
    int argc,
    const char * argv[]
)
{
  io_service_t displayWrangler;
  CFDictionaryRef sessionInfoDict;
  CFBooleanRef sessionIsLocked;
  bool result = false;
  int retry_scrlock;

  displayWrangler = IOServiceGetMatchingService (kIOMasterPortDefault,
                                                 IOServiceNameMatching ("IODisplayWrangler"));

  SACLockScreenImmediate ();

  for (
    retry_scrlock = 0;
    retry_scrlock < 20 && !result;
    retry_scrlock++
       )
  {
    sessionInfoDict = CGSessionCopyCurrentDictionary();
    sessionIsLocked = CFDictionaryGetValue (sessionInfoDict,
                                            CFSTR("CGSSessionScreenIsLocked"));
    if (sessionIsLocked) {
      result = CFBooleanGetValue (sessionIsLocked);
    }
    CFRelease (sessionInfoDict);
    if (!result)
      usleep(BLINK);
  }

  if (result) {
    usleep(2 * BLINK);
    IORegistryEntrySetCFProperty (displayWrangler, CFSTR("IORequestIdle"), kCFBooleanTrue);
  }

  IOObjectRelease (displayWrangler);

  return !result ? 0 : 1;
}
