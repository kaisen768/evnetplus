#pragma once

#include "inner_pre.h"
#include "timestamp.h"

namespace evnetplus {
class EventLoop;
class FdChannel;

class Listener {
public:
    typedef std::function <
    void(evpp_socket_t sockfd,
         const std::string& /*remote address with format "ip:port"*/,
         const struct sockaddr_in* /*remote address*/) >
    NewConnectionCallback;
    Listener(EventLoop* loop, const std::string& addr/*local listening address : ip:port*/);
    ~Listener();

    // socket listen
    void Listen();

    // nonblocking accept
    void Accept();

    void Stop();

    void SetNewConnectionCallback(NewConnectionCallback cb) {
        new_conn_fn_ = cb;
    }

private:
    void HandleAccept();

private:
    evpp_socket_t fd_ = -1;// The listening socket fd
    EventLoop* loop_;
    std::string addr_;
    std::unique_ptr<FdChannel> chan_;
    NewConnectionCallback new_conn_fn_;
};
}


