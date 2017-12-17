#include <cstring>
#include "SFML/Network.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        return -1;
    }

    sf::IpAddress ip("127.0.0.1");
    sf::UdpSocket socket;
    sf::Packet packet;

    packet.append(argv[1], strlen(argv[1]));
    socket.send(packet, ip, 9000);
}
