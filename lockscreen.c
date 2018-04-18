// lockscreen.c

#include <ApplicationServices/ApplicationServices.h>
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOMessage.h>

// #include <unistd.h>


extern void SACLockScreenImmediate ();

int main (
    int argc,
    const char * argv[]
)
{
  io_service_t displayWrangler;
  CFDictionaryRef sessionInfoDict;
  CFBooleanRef sessionIsLocked;
  bool result = false;

  displayWrangler = IOServiceGetMatchingService (kIOMasterPortDefault,
                                                 IOServiceNameMatching ("IODisplayWrangler"));

  SACLockScreenImmediate ();

  while (!result)
  {
    sessionInfoDict = CGSessionCopyCurrentDictionary();
    sessionIsLocked = CFDictionaryGetValue (sessionInfoDict,
                                            CFSTR("CGSSessionScreenIsLocked"));
    if (sessionIsLocked) {
      result = CFBooleanGetValue (sessionIsLocked);
    }
    CFRelease (sessionInfoDict);
    if (!result)
      usleep(100000);
  }

  IORegistryEntrySetCFProperty (displayWrangler, CFSTR("IORequestIdle"), kCFBooleanTrue);
  IOObjectRelease (displayWrangler);

  return 0;
}
