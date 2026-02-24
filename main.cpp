#include "Listener.hpp"
#include "Connection.hpp"
#include <iostream>

int main() {
    try {
        Listener l(8080);
        std::cout << "Serveur prêt sur le port 8080" << std::endl;

        while (true) {
            int client_fd = l.accept();
            std::cout << "Nouvelle connexion acceptée : fd=" << client_fd << std::endl;
            Connection c(client_fd);
            c.handle_request();
        }
    } catch (const std::exception& e) {
        std::cerr << "Erreur : " << e.what() << std::endl;
    }
}