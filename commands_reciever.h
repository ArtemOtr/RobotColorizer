#ifndef COMMANDS_RECIEVER_H
#define COMMANDS_RECIEVER_H

#include <vector>
#include "server.h"
#include "colorizer.h"

class Commands_Reciever {
public:
    Commands_Reciever();
    virtual ~Commands_Reciever();
    
    virtual std::vector<int> receive_graffiti_location();
    virtual bool move_to_graffiti(const std::vector<int>& coordinates);
    virtual bool paint_over_graffiti();
    virtual bool return_to_robot_base();

protected:
    Server server;        // Просто объекты, не указатели
    Colorizer colorizer;
};

class Test_Reciever : public Commands_Reciever {
public:
    Test_Reciever();
    ~Test_Reciever();
    
    std::vector<int> test_receive_graffiti_location();
    bool test_move_to_graffiti(const std::vector<int>& coordinates);
    bool test_paint_over_graffiti();
    bool test_return_to_robot_base();
};

#endif // COMMANDS_RECIEVER_H