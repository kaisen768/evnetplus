#ifndef _TCP_CLIENT_PLUS_H_
#define _TCP_CLIENT_PLUS_H_

#include "event_loop.h"
#include "event_loop_thread.h"
#include "tcp_client.h"
#include "timer_ticker.h"
#include "tcp_conn.h"

namespace evnetplus
{

class TCPClientPlus 
{
public:
    using Ptr = std::shared_ptr<TCPClientPlus>;

    explicit TCPClientPlus(EventLoopThread* loop_thread,
                           const std::string& remote_addr/*host:port*/,
                           const std::string& name);
    virtual ~TCPClientPlus();

    void bind(const std::string& local_addr/*host:port*/);

    void connect();

    void disconnect();

    void send(const Slice& message);

    virtual void onReceive(const TCPConnPtr& conn, const std::string& buf) {}
    virtual void onConnected(const TCPConnPtr& conn) {}
    virtual void onDisconnected(const TCPConnPtr& conn) {}
    virtual void onTimerHandle(const TCPConnPtr& conn) {}

private:
    EventLoopThread* _loop_thread;
    std::shared_ptr<TCPClient> _client;

    struct TimeTickerHelper {
        using Ptr = std::shared_ptr<TimeTickerHelper>;

        Ticker ticker;
        InvokeTimerPtr timer;

        TimeTickerHelper(){}
        ~TimeTickerHelper(){}
    };
    TimeTickerHelper::Ptr _ticker_helper;
    TCPConnPtr _connection;
    std::mutex _mutex;
};

}

#endif // _TCP_CLIENT_PLUS_H_
