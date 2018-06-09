//
// Created by carljay on 17-8-29.
//

#include <ipc.h>
#include "usr.h"
int u_send_message(MESSAGE* message){
    _sys_call(SEND,&message,0,0);
}