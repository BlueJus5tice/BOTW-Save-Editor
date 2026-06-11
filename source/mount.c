#include <string.h>
#include <stdio.h>
#include <switch.h>
#include "mount.h"

static AccountUid uid;

void unmount_save() {
    fsdevCommitDevice("save");
    fsdevUnmountDevice("save");
}

int mount_save() {
    accountInitialize(AccountServiceType_Application);
    Result rc = accountGetPreselectedUser(&uid);
    if (R_FAILED(rc)) {
        s32 count = 0;
        accountGetUserCount(&count);
        if (count > 0) {
            AccountUid users[8];
            s32 actual = 0;
            accountListAllUsers(users, count, &actual);
            if (actual > 0) {
                memcpy(&uid, &users[0], sizeof uid);
                rc = 0;
            }
        }
    }
    if (R_SUCCEEDED(rc))
        rc = fsdevMountSaveData("save", 0x01007ef00011e000, uid);
    accountExit();
    return R_SUCCEEDED(rc) ? 1 : 0;
}
