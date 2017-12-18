#include <cstring>
#include <iostream>
#include "SFML/Network.hpp"
#include "JSON.h"

void MakePacket(const wchar_t* action, const wchar_t* name, const float score, sf::Packet& packet) {
    JSONObject data;
    data[L"action"] = new JSONValue(action);
    data[L"name"] = new JSONValue(name);
    data[L"score"] = new JSONValue(score);
    
    JSONValue* val = new JSONValue(data);
    data.clear();
    
    std::wstring data_string = val->Stringify();
    delete val;
    
    std::string wide;
    wide.assign(data_string.begin(), data_string.end());
    packet.append(wide.c_str(), wide.length());    
}

int main(int argc, char* argv[]) {
    sf::IpAddress ip("127.0.0.1");
    sf::UdpSocket socket;
    sf::Packet packet;
    unsigned short port = 9000;
    unsigned short response_port = 1002;

    // No arguments retrieve scores
    if (argc == 1) {
        MakePacket(L"GetScores",L"",0.0f,packet);
        socket.bind(response_port);
        socket.send(packet, ip, port);

        char buffer[512];
        sf::IpAddress response_ip;
        std::size_t responde_size;
        socket.receive(buffer, 512, responde_size, response_ip, port);
        std::string response_string(buffer, responde_size);

        JSONValue* json_highscore = JSON::Parse(response_string.c_str());
        if (!json_highscore->IsObject()) {
            std::cout << "Error" << "\n";
            return -1;
        }

        JSONObject root = json_highscore->AsObject();
        if (root.find(L"Scores") != root.end() && root[L"Scores"]->IsArray()) {
            JSONArray scores = root[L"Scores"]->AsArray();
            std::cout << "Current high scores: " << "\n";
            for (int i = 0; i < scores.size(); ++i) {
                JSONObject current_object = scores[i]->AsObject();
                std::wcout << "Name: " << current_object[L"Name"]->AsString();
                std::cout << " // Highscore: " << current_object[L"Score"]->AsNumber() << "\n";
            }
        }
        delete json_highscore;
    } else if (argc == 3) {
        MakePacket(L"AddScore", std::wstring(argv[1], argv[1] + strlen(argv[1])).c_str(), std::atof(argv[2]), packet);
        if (socket.send(packet, ip, port) != sf::Socket::Done) {
            std::cout << "An error occurred while sending the packet" << "\n";
        }
    } else {
        std::cout << "Invalid usage" << "\n";
        return -1;
    }
}
