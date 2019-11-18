#ifndef SERVICE_H_INCLUDED
#define SERVICE_H_INCLUDED
#include "BasicController.h"
#include <vector>
using std::string;
using std::vector;

class Service : public BasicController {
    public:
        Service(const std::string& address, const std::string& port, std::vector<string> add, int _sim_start = 0, int _speed_to_set = 0)
            : BasicController(address,port), sim_start(_sim_start), speed_to_set(_speed_to_set), module_address(add) {}
        ~Service() {}
        void handleGet(http_request message);
        void handlePut(http_request message);
        void initRestOpHandlers() override;
        void send_time_info(int step);
        int getStart(){return sim_start;}
    private:
        int sim_start;
        int speed;
        string startTime;
        string symTime;
        int speed_to_set;
        std::vector<string> module_address;
        int symTimeYear;
        int symTimeMonth;
        int symTimeDay;
        int symTimeHour;
        int symTimeMinutes;
        int symTimeSeconds;
};
#endif // SERVICE_H_INCLUDED
