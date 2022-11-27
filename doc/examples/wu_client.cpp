//
//  Weather update client in C++
//  Connects SUB socket to tcp://localhost:5556
//  Collects weather updates and finds avg temp in zipcode
//
#include <azmq/socket.hpp>
#include <boost/asio.hpp>

#include <array>
#include <iostream>
#include <sstream>
// c headers
#include <cstring>

namespace asio = boost::asio;

int main(int argc, char* argv[]) {
    asio::io_service ios;
    azmq::sub_socket subscriber(ios);

    // Socket to talk to server
    std::cout << "Collecting updates from weather server...\n" << std::endl;
    subscriber.connect("tcp://localhost:5556");

    // Subscribe to zipcode, default is NYC, 10001
    const char* filter = (argc > 1) ? argv[1] : "10001 ";
    subscriber.set_option(azmq::socket::subscribe(filter));

    // Process 100 updates
    int update_nbr{};
    long total_temp{0};
    for (update_nbr = 0; update_nbr < 100; update_nbr++) {
        std::array<char, 20> update;

        auto size = subscriber.receive(asio::buffer(update));
        if (size >= 20) {
            int zipcode{}, temperature{}, relhumidity{};
            std::istringstream iss(static_cast<char*>(update.data()));
            iss >> zipcode >> temperature >> relhumidity;

            total_temp += temperature;
            std::cout << temperature << std::endl;
        }
    }
    std::cout << "Average temperature for zipcode '" << filter << "' was " << static_cast<int>(total_temp / update_nbr)
              << "F" << std::endl;
    return 0;
}
