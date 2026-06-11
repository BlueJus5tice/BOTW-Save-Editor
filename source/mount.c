#include <string.h>
#include <stdio.h>
#include <switch.h>

static AccountUid g_userID = {0};

void unmountSaveData(){
    fsdevCommitDevice("save");
    fsdevUnmountDevice("save");
}

int mountSaveData()
{
    u64 titleID = 0x01007ef00011e000;

    accountInitialize();

    Result rc = accountGetPreselectedUser(&g_userID);
    if (R_FAILED(rc)) {
        s32 userCount = 0;
        accountGetUserCount(&userCount);
        if (userCount > 0) {
            AccountUid users[userCount];
            s32 actual = 0;
            accountListAllUsers(users, userCount, &actual);
            if (actual > 0) {
                memcpy(&g_userID, &users[0], sizeof(AccountUid));
                rc = 0;
            }
        }
    }

    if (R_SUCCEEDED(rc)) {
        rc = fsdevMountSaveData("save", titleID, g_userID);
    }

    accountExit();
    return R_SUCCEEDED(rc) ? 1 : 0;
}
