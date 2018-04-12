// lockscreen.c

#include <unistd.h>
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOKitLib.h>


extern void SACLockScreenImmediate ();

int main(int argc, const char * argv[])
{

    SACLockScreenImmediate();

	io_registry_entry_t entry = IORegistryEntryFromPath(
			kIOMasterPortDefault,
			"IOService:/IOResources/IODisplayWrangler"
		);
    IORegistryEntrySetCFProperty(entry, CFSTR("IORequestIdle"), kCFBooleanTrue);
    IOObjectRelease(entry);

    return 0;
}
