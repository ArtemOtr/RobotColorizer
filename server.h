#include <string>
#include <vector>

class Server{
    public:
        Server();
        ~Server();
    
        bool detect_graffiti();
    
        std::vector<int> get_graffiti_location();
    
        bool move_to_graffiti_command();
    
    
};
    