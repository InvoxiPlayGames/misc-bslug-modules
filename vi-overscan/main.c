#include <bslug.h>
#include <stdio.h>
#include <rvl/vi.h>

BSLUG_MODULE_GAME("????");
BSLUG_MODULE_NAME("Enable Overscan");
BSLUG_MODULE_VERSION("1.0");
BSLUG_MODULE_AUTHOR("InvoxiPlayGames");
BSLUG_MODULE_LICENSE("MIT");

void VIConfigureHook(GXRenderModeObj *mode) {
	// print original settings for reference
	printf("fbWidth:%i efbHeight:%i xfbHeight:%i viXOrigin:%i viYOrigin:%i viWidth:%i viHeight:%i\n",
		mode->fbWidth, mode->efbHeight, mode->xfbHeight, mode->viXOrigin, mode->viYOrigin, mode->viWidth, mode->viHeight);
	// set framebuffer to 640x480
	mode->fbWidth = 640;
	mode->efbHeight = mode->xfbHeight = 480;
	// set X/Y origin to 0
	mode->viXOrigin = 0;
	mode->viYOrigin = 0;
	// set VI output resolution to 720x480
	mode->viWidth = 720;
	mode->viHeight = 480;
	// call the original function
	VIConfigure(mode);
	return;
}

BSLUG_MUST_REPLACE(VIConfigure, VIConfigureHook);