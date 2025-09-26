#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <fstream>

// Console colors
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define BLUE    "\033[34m"
#define RESET   "\033[0m"

struct Message {
    std::string sender;
    std::string text;
    std::string timestamp;
};

class ChatRoom {
    std::vector<Message> chatHistory;
public:
    void sendMessage(const std::string &sender, const std::string &text, const std::string &color) {
        // Get current IST time
        std::time_t t = std::time(nullptr);
        t += 5 * 3600 + 30 * 60; // UTC+5:30 for IST
        char buffer[20];
        std::strftime(buffer, sizeof(buffer), "%H:%M:%S", std::gmtime(&t));

        chatHistory.push_back({sender, text, buffer});
        std::cout << color << "[" << buffer << "] " << sender << ": " << text << RESET << "\n";
    }

    void showHistory() {
        std::cout << "\n--- Full Chat History ---\n";
        for (auto &msg : chatHistory) {
            std::cout << "[" << msg.timestamp << "] "
                      << msg.sender << ": " << msg.text << "\n";
        }
    }

    void saveHistoryToFile(const std::string &filename) {
        std::ofstream out(filename, std::ios::app);
        for (auto &msg : chatHistory) {
            out << "[" << msg.timestamp << "] "
                << msg.sender << ": " << msg.text << "\n";
        }
        out.close();
        std::cout << "Chat history saved to " << filename << "\n";
    }

    void loadHistoryFromFile(const std::string &filename) {
        std::ifstream in(filename);
        if (!in.is_open()) return;
        std::string line;
        std::cout << "\n--- Previous Chat History ---\n";
        while (std::getline(in, line)) {
            std::cout << line << "\n";
        }
        in.close();
    }
};

int main() {
    ChatRoom room;
    std::string filename = "chat_history.txt";

    // Load previous chat if exists
    room.loadHistoryFromFile(filename);

    std::vector<std::string> users = {"Alice", "Bob", "Charlie"};
    std::vector<std::string> colors = {RED, GREEN, BLUE};

    std::string text;
    int choice;

    std::cout << "\nType /exit to quit\n";

    while (true) {
        std::cout << "\nSelect user to send message:\n";
        for (size_t i = 0; i < users.size(); i++)
            std::cout << i+1 << ". " << users[i] << "\n";
        std::cout << users.size()+1 << ". Show history\n";
        std::cout << users.size()+2 << ". Save history\n";
        std::cout << users.size()+3 << ". Exit\n";
        std::cin >> choice;
        std::cin.ignore(); // flush newline

        if (choice >= 1 && choice <= (int)users.size()) {
            std::cout << "Enter message: ";
            std::getline(std::cin, text);
            if (text == "/exit") continue;
            room.sendMessage(users[choice-1], text, colors[choice-1]);
        } 
        else if (choice == (int)users.size()+1) {
            room.showHistory();
        } 
        else if (choice == (int)users.size()+2) {
            room.saveHistoryToFile(filename);
        } 
        else break;
    }

    std::cout << "\nChat ended. Full history:\n";
    room.showHistory();
    room.saveHistoryToFile(filename);

    return 0;
}
