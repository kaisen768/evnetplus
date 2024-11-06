#include <iostream>
#include "evnetplus/tcp_client_plus.h"
#include "evnetplus/event_watcher.h"

using namespace evnetplus;
using namespace std;

class ClientExample : public TCPClientPlus
{
public:
    ClientExample(EventLoopThread* loop_thread,
                  const std::string& remote_addr,
                  const std::string& name);
    ~ClientExample();

    virtual void onReceive(const TCPConnPtr& conn, const std::string& buf) override {
        cout << "received bytes: " << buf << ", from: " << conn->remote_addr() << endl;
    }

    virtual void onConnected(const TCPConnPtr& conn) override {
        cout << conn->remote_addr() << " is connected." << endl;
    }

    virtual void onDisconnected(const TCPConnPtr& conn) override {
        cout << conn->remote_addr() << " is disconnected." << endl;
    }

    virtual void onTimerHandle(const TCPConnPtr& conn) override {
        cout << "timer occur" << endl;
    }
};

ClientExample::ClientExample(EventLoopThread* loop_thread,
                             const std::string& remote_addr,
                             const std::string& name)
    : TCPClientPlus(loop_thread, remote_addr, name)
{
}

ClientExample::~ClientExample()
{
}

int main(int argc, char const *argv[])
{
    bool bEnableStop = false;

    EventLoopThread loopthread;
    loopthread.Start(true);

    ClientExample client(&loopthread, "192.168.0.100:9600", "ClientExample");
    client.connect();

    SignalEventWatcher signalWatcher(SIGINT, loopthread.loop(), [&loopthread, &client, &bEnableStop](){
        std::cout << "atched signal" << std::endl;
        bEnableStop = true;
    });
    signalWatcher.Init();
    signalWatcher.AsyncWait();

    while (!bEnableStop) {
        this_thread::sleep_for(chrono::milliseconds(500));
    }

    client.disconnect();
    loopthread.Stop(true);

    return 0;
}

