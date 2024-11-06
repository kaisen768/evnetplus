#include "inner_pre.h"

#include "libevent.h"

#include <signal.h>

#include <map>
#include <thread>
#include <mutex>

namespace evnetplus {

namespace {
struct OnStartup {
    OnStartup() {

        if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
            LOG_ERROR << "SIGPIPE set failed.";
            exit(-1);
        }
        LOG_INFO << "ignore SIGPIPE";
    }
    ~OnStartup() {
    }
} __s_onstartup;
}


int EventAdd(struct event* ev, const struct timeval* timeout) {
    return event_add(ev, timeout);
}

int EventDel(struct event* ev) {
    return event_del(ev);
}

int GetActiveEventCount() {
    return 0;
}

}
