#include <boost/asio/io_service.hpp>
#include <boost/asio/steady_timer.hpp>
#include <chrono>
#include <iostream>

int main()
{
	using namespace boost::asio;
	io_service service;
	steady_timer timer{service, std::chrono::seconds{5}};
	timer.async_wait([](const boost::system::error_code &ec){
		std::cout << "timeout" << std::endl;
	});

	service.run();
}

//g++ asio_steady_timer.cpp -o asio_steady_timer -std=c++11