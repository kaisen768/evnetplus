#include <iostream>
#include <signal.h>
#include "evnetplus/tcp_server_plus.h"

using namespace std;
using namespace evnetplus;

EventLoop _loop;
TCPServerPlus::Ptr _server;

class TransmitTCPSession : public TCPSession
{
public:
    TransmitTCPSession(const TCPConnPtr& conn)
        : TCPSession(conn)
    {

    }
    ~TransmitTCPSession() {}

    virtual void onReceive(const std::string &buf) override {
        std::cout << "received bytes : " << buf.length() << std::endl;
        send(buf);
    }

    virtual void onConnected() override {
        // fprintf(stderr, "new connection : %s\n", remoteAddr().c_str());
        std::cout << "new connection : " << remoteAddr() << std::endl;
    }

    virtual void onDisconnected() override {

    }

    virtual void onTimerHandle() override {

    }
};

void EventloopThreadHandle()
{
    _loop.Run();
}

int main(int argc, char const *argv[])
{
    _server = make_shared<TCPServerPlus>(&_loop, "0.0.0.0:9600", "PusherTcpServer");
    if (_server == nullptr) {
        fprintf(stderr, "pusher tcp server create failed.\n");
        return -1;
    }
    bool ret = _server->start<TransmitTCPSession>();

    std::cout << "server start result: " << ret << std::endl;

    std::thread loopthread(EventloopThreadHandle);
    loopthread.detach();

    // Just block till user tells us to quit.
    std::cout << "please enter the charater 'q' to exit." << std::endl;
    while (std::tolower(std::cin.get()) != 'q')
        ;

    _server->stop();
    _loop.Stop();

    return 0;
}
