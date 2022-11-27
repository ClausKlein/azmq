//
//  Weather update server in C++
//  Binds PUB socket to tcp://*:5556
//  Publishes random weather updates
//
#include <azmq/socket.hpp>
#include <boost/asio.hpp>

#if defined(WIN32)
// XXX #include <zhelpers.hpp>
#endif

#include <array>
#include <chrono>
#include <thread>

// c headers
#include <cstdio>
#include <cstdlib>
#include <ctime>

inline int within(int num) { return static_cast<int>(static_cast<double>(num * random()) / (RAND_MAX + 1.0)); }

namespace asio = boost::asio;

int main() {
    using namespace std::chrono_literals;

    // Prepare our context and publisher
    asio::io_service ios;
    azmq::pub_socket publisher(ios);
    publisher.bind("tcp://*:5556");

#if !defined(WIN32)
    publisher.bind("ipc://weather.ipc");  // Not usable on Windows.
#endif

    //  Initialize random number generator
    srandom(static_cast<unsigned>(time(nullptr)));
    while (true) {
        // Get values that will fool the boss
        int zipcode = 10000 + within(100);
        int temperature = within(215) - 80;
        int relhumidity = within(50) + 10;

        // Send message to all subscribers
        std::array<char, 20> message;

        snprintf(static_cast<char*>(message.data()), message.size(), "%05d %d %d\n", zipcode, temperature, relhumidity);

        publisher.send(asio::buffer(message));

        std::this_thread::sleep_for(1ms);
    }

    return 0;
}
