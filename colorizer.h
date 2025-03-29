
#include <string>
#include <vector>
#include <exception>


class Colorizer {
public:
    
    Colorizer();
    ~Colorizer();

    virtual void set_speed(double new_speed);

    virtual void move_forward(unsigned int time);

    virtual void move_backward(unsigned int time);

    virtual void turn_right(double usr_angle);

    virtual void turn_left(double usr_angle);
    
    virtual bool activate_painting();

private:
    double x;
    double y;
    double angle;
    double speed;
};

class Test_Colorizer: public Colorizer{
public:
    Test_Colorizer();
    ~Test_Colorizer();

    void test_set_speed(double new_speed);

    void test_move_forward(unsigned int time);

    void test_move_backward(unsigned int time);

    void test_turn_right(double usr_angle);

    void test_turn_left(double usr_angle);

    bool test_activate_painting();
    
};




