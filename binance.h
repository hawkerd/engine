#ifndef BINANCE_API_H
#define BINANCE_API_H

#include <string>
#include <functional>
#include <websocketpp/websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/websocketpp/client.hpp>
#include <boost/asio/ssl/context.hpp>
#include <spdlog/spdlog.h>

using WebSocketClient = websocketpp::client<websocketpp::config::asio_client>;
using ConnectionHandle = websocketpp::connection_hdl;

class BinanceAPI {
public:
    BinanceAPI(const std::string& symbol) {
        connected = false;
        websocketURL = "wss://stream.binance.com:9443/ws/" + symbol + "@trade";
        client.init_asio();
        client.set_tls_init_handler([](ConnectionHandle) {
            return websocketpp::lib::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::tlsv12);
        });

        // Register event handlers
        client.set_open_handler([this](ConnectionHandle hdl) { onOpen(hdl); });
        client.set_message_handler([this](ConnectionHandle hdl, WebSocketClient::message_ptr msg) {
            onMessage(hdl, msg);
        });
        client.set_close_handler([this](ConnectionHandle hdl) { onClose(hdl); });
        client.set_fail_handler([this](ConnectionHandle hdl) { onFail(hdl); });
    }

    ~BinanceAPI() {
        disconnect();
    }

    void connect() {
        websocketpp::lib::error_code ec;
        WebSocketClient::connection_ptr con = client.get_connection(websocketURL, ec);
        if (ec) {
            spdlog::error("Failed to connect to Binance WebSocket: {}", ec.message());
            return;
        }
        client.connect(con);
        spdlog::info("Connecting to Binance WebSocket: {}", websocketURL);

        websocketpp::lib::thread asioThread(&WebSocketClient::run, &client);
        asioThread.detach();
    }

    void disconnect() {
        if (connected) {
            websocketpp::lib::error_code ec;
            client.close(hdl, websocketpp::close::status::normal, "", ec);
            if (ec) {
                spdlog::error("Failed to disconnect from Binance WebSocket: {}", ec.message());
            }
            connected = false;
        }
    }

    void setMessageHandler(std::function<void(std::string)> handler) {
        messageHandler = handler;
    }

private:
    bool connected;
    std::string websocketURL;
    WebSocketClient client;
    ConnectionHandle hdl;

    // WebSocket event handlers
    void onOpen(ConnectionHandle hdl) {
        spdlog::info("Connected to Binance WebSocket");
        connected = true;
        this->hdl = hdl;
    }
    void onMessage(ConnectionHandle hdl, websocketpp::config::asio_client::message_type::ptr msg) {
        std::string payload = msg->get_payload();
        spdlog::info("Received WebSocket message: {}", payload);

        if (messageHandler) {
            messageHandler(payload);
        }
    }
    void onClose(ConnectionHandle hdl) {
        spdlog::info("Disconnected from Binance WebSocket");
        connected = false;
    }
    void onFail(ConnectionHandle hdl) {
        spdlog::error("Failed to connect to Binance WebSocket");
        connected = false;
    }

    std::function<void(std::string)> messageHandler;
};

#endif
