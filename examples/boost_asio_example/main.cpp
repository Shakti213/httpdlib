#include "httpdlib/filesystem_response_generator.h"
#include "httpdlib/interface/response.h"
#include "httpdlib/memory_response.h"
#include "httpdlib/request.h"
#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

static httpdlib::filesystem_response_generator fs_response(PUB_HTML);

template <typename ConnectionHandler>
class asio_generic_server
{
public:
    asio_generic_server() : m_acceptor(m_io_service) {
    }
    void start_server(std::uint16_t port) {
        using namespace boost::asio::ip;
        tcp::endpoint ep(tcp::v4(), port);
        m_acceptor.open(tcp::v4());
        m_acceptor.bind(ep);
        m_acceptor.listen(0);
        std::cout << "Listening on " << port << std::endl;
        auto new_connection = std::make_shared<ConnectionHandler>(m_io_service);
        m_acceptor.async_accept(new_connection->socket(), [=](auto ec) {
            new_connection->connected(ec);
            handle_new_connection();
        });

        for (int i = 0; i < 10; i++) {
            m_thread_pool.emplace_back([&]() { m_io_service.run(); });
        }

        for (auto &t : m_thread_pool) {
            t.join();
        }
    }

    void handle_new_connection() {
        std::cout << "New connection..." << std::endl;
        auto next_connection =
            std::make_shared<ConnectionHandler>(m_io_service);
        m_acceptor.async_accept(next_connection->socket(), [=](auto ec) {
            next_connection->connected(ec);
            handle_new_connection();
        });
    }

private:
    boost::asio::io_service m_io_service;
    boost::asio::ip::tcp::acceptor m_acceptor;
    boost::asio::ip::tcp::endpoint m_endpoint;

    std::vector<std::thread> m_thread_pool;
};

class http_handler : public std::enable_shared_from_this<http_handler>
{
public:
    http_handler(boost::asio::io_service &service)
        : m_strand(service), m_socket(service), m_conn_timeout_timer(service) {
    }

    boost::asio::ip::tcp::socket &socket() {
        return m_socket;
    }
    void connected(const boost::system::error_code &ec) {
        if (!ec) {
            read_data();
        }
    }

    void data_read(std::size_t bytes_read) {
        m_request.add_data(m_recv_data.data(), bytes_read);
        if (m_request.finished()) {
            std::cout << "Request for " << m_request.uri() << " finished.";
            for (const auto &h : m_request.headers()) {
                std::cout << "\t" << h.first << ": " << h.second << std::endl;
            }
            m_response = fs_response.get_response(m_request);
            write_data();
            m_conn_timeout_timer.cancel();
        }
        else if (m_request.error()) {
            m_response.reset(new httpdlib::memory_response(
                httpdlib::memory_response::default_for_code(
                    m_request.parse_result())));
            write_data();
        }
        else {
            m_response.reset();
            read_data();
        }
    }

    void data_sent(std::size_t bytes_sent) {
        m_response->async_bytes_written(bytes_sent);
        std::cout << "Data written: " << bytes_sent << std::endl;
        if (m_response->done()) {
            m_request.reset();
            read_data();
        }
        else {
            write_data();
        }
    }

    void idle_timeout() {
        std::cout << "Connection timeout..." << std::endl;
        m_socket.close();
    }

private:
    void update_timeout() {
        m_conn_timeout_timer.expires_from_now(boost::posix_time::seconds(5));
        m_conn_timeout_timer.async_wait([me = shared_from_this()](auto ec) {
            if (ec) {
                return;
            }
            me->idle_timeout();
        });
    }

    void read_data() {
        m_socket.async_read_some(
            boost::asio::buffer(m_recv_data),
            m_strand.wrap([me = shared_from_this()](auto ec, auto sz) {
                if (ec) {
                    std::cout << "Error..." << std::endl;
                    return;
                }
                me->data_read(sz);
            }));
        update_timeout();
    }

    void write_data() {
        auto write_end_handler =
            m_strand.wrap([me = shared_from_this()](auto ec, auto cnt) {
                if (ec) {
                    std::cout << "Error..." << std::endl;
                    return;
                }
                me->data_sent(cnt);
            });

        auto writer = [&s = m_socket, write_end_handler ](
            const char *p, std::size_t max_size) {
            s.async_write_some(boost::asio::buffer(p, max_size),
                               write_end_handler);
            return std::size_t(0);
        };

        m_response->write_next(writer);
    }

    std::array<char, 2048> m_recv_data;
    boost::asio::io_service::strand m_strand;
    boost::asio::ip::tcp::socket m_socket;
    boost::asio::deadline_timer m_conn_timeout_timer;
    httpdlib::request m_request;
    std::unique_ptr<httpdlib::interface::response> m_response;
};

int main() {
    asio_generic_server<http_handler> server;
    server.start_server(8889);
    return 0;
}
