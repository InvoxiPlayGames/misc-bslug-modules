#include <bslug.h>
#include <stdio.h>
#include <rvl/ipc.h>
#include <rvl/ssl.h>

BSLUG_MODULE_GAME("????");
BSLUG_MODULE_NAME("Disable SSL Validation");
BSLUG_MODULE_VERSION("1.0");
BSLUG_MODULE_AUTHOR("InvoxiPlayGames");
BSLUG_MODULE_LICENSE("MIT");

// certain games include this, others don't, so don't use the same name
int Impl_SSLDisableVerifyOptionForDebug(ssl_ctx_t context, int unk_r4) {
	// local variables - for some reason all the integer values are 8...
	ioctlv vector[3];
	int response[8];
	int ssl_context[8];
	int unknown[8];
	// open SSL descriptor
	ios_fd_t fd = IOS_Open("/dev/net/ssl", IPC_OPEN_NONE);
	// if we didn't get an fd, fail
	if (fd < 0)
		return -1;
	// set variables
	ssl_context[0] = context;
	unknown[0] = unk_r4;
	// set up vectors (response is first because N did an oops!)
	vector[0].data = response;
	vector[0].len = sizeof(response);
	vector[1].data = ssl_context;
	vector[1].len = sizeof(ssl_context);
	vector[2].data = unknown;
	vector[2].len = sizeof(unknown);
	// send ioctlv
	IOS_Ioctlv(fd, 0xF, 1, 2, vector);
	// return a response;
	return response[0];
}

ssl_ctx_t SSLNewHook(int options, char *hostname) {
	ssl_ctx_t r = SSLNew(options, hostname);
	printf("Disabling verification for host %s\n", hostname);
	Impl_SSLDisableVerifyOptionForDebug(r, 0xB /*seen as 0xb in Rock Band*/);
	return r;
}

BSLUG_MUST_REPLACE(SSLNew, SSLNewHook);