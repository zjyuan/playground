#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <iostream>

using namespace boost::asio;
using namespace boost::asio::ip;

void resolve_handler(const boost::system::error_code &ec,
  tcp::resolver::iterator it)
{
  if (!ec)
    std::cout << "resolved done" << std::endl;
}

int main()
{
	io_service srv;
	tcp::resolver resolver{srv};

	tcp::resolver::query q{"www.baidu.com", "80"};
	resolver.async_resolve(q, resolve_handler);

	srv.run();
}