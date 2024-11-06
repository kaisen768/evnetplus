#pragma once

#include <assert.h>
#include <stdint.h>

#ifdef __cplusplus
#include <iostream>
#include <memory>
#include <functional>
#endif // end of define __cplusplus

#include "sys_addrinfo.h"
#include "sys_sockets.h"
#include "sockets.h"
#include "logging.h"

struct event;
namespace evnetplus {
    int EventAdd(struct event* ev, const struct timeval* timeout);
    int EventDel(struct event*);
    int GetActiveEventCount();
}
