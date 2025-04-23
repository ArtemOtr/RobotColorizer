#ifndef ROBOT_CONTROLLER_H
#define ROBOT_CONTROLLER_H

#include <vector>
#include "server.h"
#include "colorizer.h"

class Robot_Controller {
public:
    Robot_Controller();
    virtual ~Robot_Controller();

    virtual std::vector<int> receive_graffiti_location();
    virtual bool move_to_graffiti(const std::vector<int>& coordinates);
    virtual bool paint_over_graffiti();
    virtual bool return_to_robot_base();

protected:
    Server server;        // Просто объекты, не указатели
    Colorizer colorizer;
};

class Test_Controller : public Robot_Controller {
public:
    Test_Controller();
    ~Test_Controller();

    std::vector<int> test_receive_graffiti_location();
    bool test_move_to_graffiti(const std::vector<int>& coordinates);
    bool test_paint_over_graffiti();
    bool test_return_to_robot_base();
};

#endif // ROBOT_CONTROLLER_H