#ifndef _TCP_SERVER_PLUS_H_
#define _TCP_SERVER_PLUS_H_

#include <memory>
#include <unordered_map>
#include <string>
#include <thread>
#include <condition_variable>
#include <mutex>

#include "tcp_server.h"
#include "tcp_conn.h"
#include "timer_ticker.h"

namespace evnetplus
{


#define TCPSERVER_THREAD_NUM 4

class TCPSession: public std::enable_shared_from_this<TCPSession> {
public:
    using Ptr = std::shared_ptr<TCPSession>;

    explicit TCPSession(const evnetplus::TCPConnPtr& conn) {
        _conn = conn;
    }
    ~TCPSession() {}

    virtual void onReceive(const std::string &buf) {}
    virtual void onConnected() {}
    virtual void onDisconnected() {}
    virtual void onTimerHandle() {}

    void send(const std::string &data) {
        _conn->Send(data);
    }

    const std::string& remoteAddr() { return _conn->remote_addr(); }
    const std::string addrToString() { return _conn->AddrToString(); }
    uint64_t id() { return _conn->id(); }
    void close() { _conn->Close(); }

private:
    evnetplus::TCPConnPtr _conn;
};

class TCPServerPlus
{
public:
    using Ptr = std::shared_ptr<TCPServerPlus>;

    /**
     * @brief TCPServer的构造函数
     * @param loop
     * @param listen_addr 监听的地址，格式为 "ip:port"
     * @param name TCP服务器名字
     * @param thread_num 服务器工作的最大线程数量，默认为 TCPSERVER_THREAD_NUM
     */
    explicit TCPServerPlus(evnetplus::EventLoop* loop, const std::string& listen_addr,
                const std::string& name, uint32_t thread_num = TCPSERVER_THREAD_NUM);

    virtual ~TCPServerPlus();

    bool start();

    template <typename SessionType>
    bool start(const std::function<void(std::shared_ptr<SessionType>&)> &cb = nullptr) {
        _session_alloc = [cb](const evnetplus::TCPConnPtr& conn) -> TCPSession::Ptr {
            auto session = std::shared_ptr<SessionType>(new SessionType(conn));
            if (cb) {
                cb (session);
            }
            return session;
        };

        return start();
    }

    void stop();

    virtual void onReceive(const evnetplus::TCPConnPtr& conn, const std::string& buf) {}
    virtual void onConnected(const evnetplus::TCPConnPtr& conn) {}
    virtual void onDisconnected(const evnetplus::TCPConnPtr& conn) {}
    virtual void onTimerHandle(const evnetplus::TCPConnPtr& conn) {}

    Ticker& ticker(const evnetplus::TCPConnPtr& conn);

private:
    evnetplus::EventLoop* _loop;
    std::shared_ptr<evnetplus::TCPServer> _server;
    std::condition_variable _condvar;
    std::mutex _mutex;

    struct TimeTickerHelper {
        using Ptr = std::shared_ptr<TimeTickerHelper>;

        Ticker ticker;
        evnetplus::InvokeTimerPtr timer;

        TimeTickerHelper(){}
        ~TimeTickerHelper(){}
    };
    std::unordered_map<uint64_t, TimeTickerHelper::Ptr> _ticker_map;

    std::function<TCPSession::Ptr(const evnetplus::TCPConnPtr&)> _session_alloc;
    std::unordered_map<uint64_t, TCPSession::Ptr> _session_map;
};

} // namespace evnetplus

#endif // _TCP_SERVER_PLUS_H_
