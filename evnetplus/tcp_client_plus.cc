#include "tcp_client_plus.h"

namespace evnetplus
{

TCPClientPlus::TCPClientPlus(EventLoopThread* loop_thread,
                             const std::string& remote_addr/*host:port*/,
                             const std::string& name)
    : _loop_thread(loop_thread)
    , _client(new TCPClient(loop_thread->loop(), remote_addr, name))
    , _ticker_helper(TimeTickerHelper::Ptr(new TimeTickerHelper()))
    , _connection(nullptr)
{
    _client->SetConnectionCallback([this](const TCPConnPtr& conn){
        std::lock_guard<std::mutex> lock(_mutex);

        if (conn->IsConnected()) {
            _connection = conn;
            onConnected(_connection);

            _ticker_helper->timer = InvokeTimer::Create(_loop_thread->loop(), evnetplus::Duration(1.0F), [this, conn]{
                onTimerHandle(conn);
            }, true);
            _ticker_helper->timer->Start();
        } else {
            if (_connection) {
                onDisconnected(_connection);
                _connection.reset();
            }

            if (_ticker_helper->timer) {
                _ticker_helper->timer->Cancel();
            }
        }
    });

    _client->SetMessageCallback([this](const TCPConnPtr& conn, Buffer* buffer){
        std::string message(buffer->NextAllString());

        onReceive(conn, message);
    });
}

TCPClientPlus::~TCPClientPlus()
{
}

void
TCPClientPlus::bind(const std::string& local_addr)
{
    _client->Bind(local_addr);
}

void
TCPClientPlus::connect()
{
    _client->Connect();
}

void
TCPClientPlus::disconnect()
{
    _client->Disconnect();
}

void
TCPClientPlus::send(const Slice& message)
{
    std::lock_guard<std::mutex> lock(_mutex);
    if (_connection) {
        _connection->Send(message);
    }
}

}
