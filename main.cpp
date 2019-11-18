#include <iostream>
#include "Service.h"
using namespace std;
#define STEP 5
int main()
{
    int i=1;
    std::vector<string> add;
    // TODO -> add module addresses
    //add.push_back("http://127.0.0.1:8080/time");
    Service serv("127.0.0.1","8080",add);
    serv.setEndpoint("/time");
    serv.accept().wait();
    while(1==1)
    {
        if (serv.getStart()){
            sleep(STEP);
            std::cout<<"krok "<<i<<std::endl;
            serv.send_time_info(STEP);
            i++;
        }
        else sleep(1);
    }
    return 0;
}
