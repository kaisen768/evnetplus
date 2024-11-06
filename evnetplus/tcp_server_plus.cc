#include "tcp_server_plus.h"

namespace evnetplus
{

TCPServerPlus::TCPServerPlus(evnetplus::EventLoop* loop, const std::string& listen_addr,
                const std::string& name, uint32_t thread_num)
    : _loop(loop)
    , _server(new evnetplus::TCPServer(_loop, listen_addr, name, thread_num))
{
    _server->SetConnectionCallback([this](const evnetplus::TCPConnPtr& conn){
        if (conn->IsConnected()) {
            onConnected(conn);

            TCPSession::Ptr session = nullptr;
            if (_session_alloc) {
                session = _session_alloc(conn);
                _session_map.emplace(conn->id(), session);

                session->onConnected();
            }

            /* 启动定时任务 */
            TimeTickerHelper::Ptr helper(new TimeTickerHelper());
            helper->timer = evnetplus::InvokeTimer::Create(_loop, evnetplus::Duration(1.0F), [this, conn, helper, session]{
                onTimerHandle(conn);

                if (session) {
                    session->onTimerHandle();
                }
            }, true);

            helper->timer->Start();
            _ticker_map.emplace(conn->id(), helper);
        } else {
            onDisconnected(conn);

            /* 删除定时任务 */
            auto pair = _ticker_map.find(conn->id());
            if (pair != _ticker_map.end()) {
                TimeTickerHelper::Ptr helper = pair->second;
                helper->timer->Cancel();

                _ticker_map.erase(pair->first);
            }

            auto sessionpair = _session_map.find(conn->id());
            if (sessionpair != _session_map.end()) {
                auto session = sessionpair->second;

                session->onDisconnected();

                _session_map.erase(sessionpair->first);
            }
        }
    });

    _server->SetMessageCallback([this](const evnetplus::TCPConnPtr& conn, evnetplus::Buffer* buf){
        std::string data = buf->NextAllString();
        onReceive(conn, data);

        auto sessionpair = _session_map.find(conn->id());
        if (sessionpair != _session_map.end()) {
            auto session = sessionpair->second;
            session->onReceive(data);
        }
    });
}

TCPServerPlus::~TCPServerPlus()
{
}

bool
TCPServerPlus::start()
{
    bool ret;

    if (_server->IsRunning()) {
        return false;
    }

    ret = _server->Init();
    if (ret == false) {
        return false;
    }

    ret = _server->Start();
    if (ret == false) {
        return false;
    }

    return true;
}

void
TCPServerPlus::stop()
{
    _server->Stop([this]{
        fprintf(stderr, "all connection is release.\n");

        _condvar.notify_all();
    });

    std::unique_lock<std::mutex> lock(_mutex);
    _condvar.wait(lock);
}

Ticker&
TCPServerPlus::ticker(const evnetplus::TCPConnPtr& session)
{
    auto pair = _ticker_map.find(session->id());
    return pair->second->ticker;
}

}
