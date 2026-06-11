#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <switch.h>

void unmountSaveData(){
    fsdevCommitDevice("save");
    fsdevUnmountDevice("save");
}

int mountSaveData()
{
    AccountUid userID = {0};
    u64 titleID = 0x01007ef00011e000;

    Result rc = accountGetLastOpenedUser(&userID);
    if (R_FAILED(rc)) {
        printf("accountGetLastOpenedUser() failed: 0x%x\n", rc);
        return 0;
    }

    rc = fsdevMountSaveData("save", titleID, userID);
    if (R_FAILED(rc)) {
        printf("fsdevMountSaveData() failed: 0x%x\n", rc);
        return 0;
    }

    return 1;
}
