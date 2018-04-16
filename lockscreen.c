// lockscreen.c

#include <ApplicationServices/ApplicationServices.h>
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOMessage.h>

// #include <unistd.h>


extern void SACLockScreenImmediate ();

void displayPowerNotificationsCallback (
    void *refcon,
    io_service_t service,
    natural_t messageType,
    void *messageArgument
)
{
  static bool powerOffSeen = false;
  switch (messageType) {
    case kIOMessageDeviceWillPowerOff:
      if (powerOffSeen)
        break;
      powerOffSeen = true;
      CFRunLoopStop(CFRunLoopGetCurrent());
      break;
  }
}

int main (
    int argc,
    const char * argv[]
)
{
  io_service_t displayWrangler;
  io_object_t notification;
  IONotificationPortRef notificationPort;
  CFDictionaryRef sessionInfoDict;
  CFBooleanRef sessionIsLocked;
  bool result = false;

  displayWrangler = IOServiceGetMatchingService (kIOMasterPortDefault,
                                                 IOServiceNameMatching ("IODisplayWrangler"));

  notificationPort = IONotificationPortCreate (kIOMasterPortDefault);

  IOServiceAddInterestNotification (notificationPort, displayWrangler, kIOGeneralInterest,
                                    displayPowerNotificationsCallback, NULL, &notification);

  CFRunLoopAddSource (CFRunLoopGetCurrent (),
                      IONotificationPortGetRunLoopSource (notificationPort),
                      kCFRunLoopDefaultMode);

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
    usleep(100000);
  }

  IORegistryEntrySetCFProperty (displayWrangler, CFSTR("IORequestIdle"), kCFBooleanTrue);
  IOObjectRelease (displayWrangler);
  CFRunLoopRun ();

  return 0;
}
