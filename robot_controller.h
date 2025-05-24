#ifndef ROBOT_CONTROLLER_H
#define ROBOT_CONTROLLER_H

#include <vector>
#include "server.h"
#include "colorizer.h"

class Robot_Controller : public Colorizer {
public:
    explicit Robot_Controller(const std::string& camera_url = "0");
    virtual ~Robot_Controller();

    virtual std::vector<double> receive_graffiti_location();  // Изменено на double
    virtual bool move_to_graffiti(const std::vector<double>& coordinates);  // Изменено на double
    virtual bool paint_over_graffiti();
    virtual bool return_to_robot_base();
    virtual bool auto_approach_graffiti(bool show_debug = true);

protected:
    Server server;
    Colorizer colorizer;
};

class Test_Controller : public Robot_Controller {
public:
    Test_Controller();
    ~Test_Controller();

    std::vector<double> test_receive_graffiti_location();  // Изменено на double
    bool test_move_to_graffiti(const std::vector<double>& coordinates);  // Изменено на double
    bool test_paint_over_graffiti();
    bool test_return_to_robot_base();
    bool test_auto_approach_graffiti(bool show_debug = true);
};

#endif // ROBOT_CONTROLLER_H