#include "robot.h"
using namespace std;
using namespace chrono;

namespace RoboComm {


    class PinManager {
    protected:
        vector<int> pin_numbers = {12, 13, 6, 20, 21, 26};
        int device_handler;

        void setPinState(int pin, uint8_t state) {
            struct gpiohandle_request config;
            struct gpiohandle_data pin_state;

            strcpy(config.consumer_label, "robo_driver");
            config.lineoffsets[0] = pin;
            config.lines = 1;
            config.flags = GPIOHANDLE_REQUEST_OUTPUT;

            if (ioctl(device_handler, GPIO_GET_LINEHANDLE_IOCTL, &config) < 0) {
                cerr << "Pin access error: " << strerror(errno) << endl;
                return;
            }

            pin_state.values[0] = state;
            if (ioctl(config.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &pin_state) < 0) {
                cerr << "State change error: " << strerror(errno) << endl;
            }

            close(config.fd);
        }

    public:
        PinManager() {
            device_handler = open("/dev/gpiochip0", O_RDWR);
            if (device_handler < 0) {
                throw runtime_error("GPIO init failure: " + string(strerror(errno)));
            }

            for (int p : pin_numbers) {
                setPinState(p, 0);
            }
            halt();
        }

        void halt() {
            setPinState(6, 1);
            setPinState(26, 1);
            for (int p : {12, 13, 20, 21}) {
                setPinState(p, 0);
            }
        }

        ~PinManager() {
            halt();
            close(device_handler);
        }
    };

    class MotionController : public PinManager {
    public:
        void moveForward(int duration) {
            activateMotors();
            setMovementPins(0, 1, 1, 0);
            delay(duration);
            halt();
        }

        void turnRight(int duration) {
            activateMotors();
            setMovementPins(0, 0, 1, 0);
            delay(duration);
            halt();
        }

        void turnLeft(int duration) {
            activateMotors();
            setMovementPins(0, 1, 0, 0);
            delay(duration);
            halt();
        }

    private:
        void activateMotors() {
            setPinState(6, 1);
            setPinState(26, 1);
        }

        void setMovementPins(int in1, int in2, int in3, int in4) {
            setPinState(12, in1);
            setPinState(13, in2);
            setPinState(20, in3);
            setPinState(21, in4);
        }

        void delay(int ms) {
            this_thread::sleep_for(milliseconds(ms));
        }
    };

    class NetworkHandler {
        struct mosquitto* mqtt;
        string current_topic;
        string last_message;
        mutex msg_lock;
        thread worker;
        bool active;

        static void onMessage(struct mosquitto* m, void* ctx, const struct mosquitto_message* msg) {
            NetworkHandler* self = static_cast<NetworkHandler*>(ctx);
            lock_guard<mutex> guard(self->msg_lock);
            self->last_message = string((char*)msg->payload, msg->payloadlen);
        }

        void processEvents() {
            while (active) {
                mosquitto_loop(mqtt, 100, 100);
                this_thread::sleep_for(milliseconds(10));
            }
        }

    public:
        NetworkHandler(const char* server, int port, const char* topic)
            : current_topic(topic), active(false) {
            mosquitto_lib_init();
            mqtt = mosquitto_new("robo_network", true, this);

            if (!mqtt) throw runtime_error("MQTT init failed");

            mosquitto_message_callback_set(mqtt, onMessage);
            mosquitto_connect(mqtt, server, port, 60);
            
            int err = mosquitto_loop_start(mqtt);
            if(err) cerr << "Network error: " << mosquitto_strerror(err);

            if (mosquitto_subscribe(mqtt, nullptr, current_topic.c_str(), 1) != MOSQ_ERR_SUCCESS) {
                throw runtime_error("Subscription failed");
            }

            active = true;
            worker = thread(&NetworkHandler::processEvents, this);
        }

        ~NetworkHandler() {
            active = false;
            if (worker.joinable()) worker.join();
            mosquitto_destroy(mqtt);
            mosquitto_lib_cleanup();
        }

        string getCommand() {
            lock_guard<mutex> lock(msg_lock);
            if (!last_message.empty()) {
                string cmd = last_message;
                last_message.clear();
                return cmd;
            }
            return "";
        }
    };

    class CommandProcessor {
        MotionController& controller;
        NetworkHandler& network;

    public:
        CommandProcessor(MotionController& ctrl, NetworkHandler& net)
            : controller(ctrl), network(net) {}

        void execute() {
            cout << "Awaiting commands..." << endl;
            while (true) {
                string cmd = network.getCommand();

                if (cmd.empty()) {
                    this_thread::sleep_for(milliseconds(100));
                    continue;
                }

                cout << "Command received: " << cmd << endl;
                
                if (cmd == "forward") controller.moveForward(200);
                else if (cmd == "stop") controller.halt();
                else if (cmd == "left") controller.turnLeft(100);
                else if (cmd == "right") controller.turnRight(100);
                else cerr << "Invalid command: " << cmd << endl;
            }
        }
    };
}

int main() {
    try {
        RoboComm::MotionController ctrl;
        //char host_input;
        //cin >> host_input;
        //const char* server_addr = &host_input;
        RoboComm::NetworkHandler net("192.168.1.100", 1883, "robot/gpio");
        RoboComm::CommandProcessor processor(ctrl, net);
        processor.execute();
    }
    catch (const exception& e) {
        cerr << "Fatal error: " << e.what() << endl;
        return 1;
    }
    return 0;
}