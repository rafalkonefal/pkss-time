#include "Service.h"
#include <string>
#include <sstream>
#include "cpprest/http_client.h"

using std::string;
using std::to_string;
void Service::initRestOpHandlers() {
    _listener.support(methods::GET,std::bind(&Service::handleGet,this,std::placeholders::_1));
    _listener.support(methods::PUT,std::bind(&Service::handlePut,this,std::placeholders::_1));
}


void Service::handlePut(http_request message) {
    vector<string> path = requestPath(message);
    if (!path.empty()){
        if (path[0] == "start"){
            message.extract_json().then([=](pplx::task<json::value> task)
            {
                try
                {
                    web::http::client::http_client worldTimeAPI(U("http://worldtimeapi.org/api/timezone/Europe/Warsaw"));
                    worldTimeAPI.request(methods::GET).then([=](http_response response)
                    {
                        if(response.status_code() == status_codes::OK)
                        {
                            json::value jsonFromAPI = response.extract_json().get();
                            std::cout << jsonFromAPI<<std::endl;
                            string dateTime = jsonFromAPI[U("datetime")].as_string();
                            startTime=dateTime.substr(0,19);
                            startTime[10]='+';
                            symTime = startTime;
                            symTimeYear = stoi(symTime.substr(0,4));
                            symTimeMonth = stoi(symTime.substr(5,6));
                            symTimeDay = stoi(symTime.substr(8,9));
                            symTimeHour = stoi(symTime.substr(11,12));
                            symTimeMinutes = stoi(symTime.substr(14,15));
                            symTimeSeconds = stoi(symTime.substr(17,18));
                        }
                        else {
                            std::cout<<"worldTimeAPI not available"<<std::endl;
                            return;
                        }
                    });
                    json::value val = task.get();
                    speed = val[U("speed")].as_number().to_int32();
                    std::cout<<speed<<std::endl;
                    sim_start = 1;
                    message.reply(status_codes::OK);
                }
                catch(std::exception& e)
                {
                    message.reply(status_codes::NotFound);
                }
            });
            json::value put_start;
            put_start["speed"] = json::value::number(speed);
            put_start["startTime"] = json::value::string(startTime);
            for (auto a : module_address){
                string path = a + "/start";
                web::http::client::http_client client("path");
                client.request(web::http::methods::PUT, U("/"), put_start).then([](const web::http::http_response& response) {
                    return response.extract_json();
                })
                .then([=](const pplx::task<web::json::value>& task) {
                    try {
                        json::value json_return = task.get();
                    }
                    catch (const web::http::http_exception& e) {
                        std::cout << "error "  << std::endl;
                    }
                });
            }

        }
        if (path[0] == "speed"){
            message.extract_json().then([=](pplx::task<json::value> task)
            {
                try
                {
                    json::value val = task.get();
                    speed_to_set = val[U("speed")].as_number().to_int32();
                    std::cout<<speed_to_set<<std::endl;
                    message.reply(status_codes::OK);
                }
                catch(std::exception& e)
                {
                    message.reply(status_codes::NotFound);
                }
            });
        }
    }
}
void Service::handleGet(http_request message) {
    vector<string> path = requestPath(message);
    if (!path.empty()){
        if (path[0] == "time_details"){
            json::value actual_data;
            actual_data["speed"] = json::value::number(speed);
            actual_data["speed_to_set"] = json::value::number(speed_to_set);
            actual_data["startime"] = json::value::string(startTime);
            actual_data["symTime"] = json::value::string(symTime);
            actual_data["sim_start"] = json::value::number(sim_start);
            message.reply(status_codes::OK,actual_data);
        }
    }

}
void Service::send_time_info(int step){
    int symSecTmp = step * speed;
    symTimeYear = symTimeYear + symSecTmp/315360000;
    symSecTmp %= 31536000;
    symTimeMonth = symTimeMonth + symSecTmp/2678400;
    symSecTmp %= 2678400;
    symTimeDay = symTimeDay + symSecTmp/86400;
    symSecTmp %= 86400;
    symTimeHour = symTimeHour + symSecTmp/3600;
    symSecTmp %= 3600;
    symTimeMinutes = symTimeMinutes + symSecTmp/60;
    symTimeSeconds = symTimeSeconds + symSecTmp%60;
    if (symTimeSeconds >= 60){
        symTimeMinutes += symTimeSeconds/60;
        symTimeSeconds %= 60;
    }
    if (symTimeMinutes >= 60){
        symTimeHour += symTimeMinutes/60;
        symTimeMinutes %= 60;
    }
    if (symTimeHour >= 24){
        symTimeDay += symTimeHour/24;
        symTimeHour %= 24;
    }
    if (symTimeDay > 31){
        symTimeMonth += symTimeDay/31;
        symTimeDay %= 31;
    }
    if (symTimeMonth > 12){
        symTimeYear+= symTimeMonth/12;
        symTimeMonth %= 12;
    }
    symTime = to_string(symTimeYear) + '-' + to_string(symTimeMonth) + '-' + to_string(symTimeDay) + '+' + to_string(symTimeHour) + ':' + to_string(symTimeMinutes) + ':' + to_string(symTimeSeconds);
    if (symTimeMonth < 10)  symTime.insert(5,"0");
    if (symTimeDay < 10)  symTime.insert(8,"0");
    if (symTimeHour < 10)  symTime.insert(11,"0");
    if (symTimeMinutes < 10)  symTime.insert(14,"0");
    if (symTimeSeconds < 10)  symTime.insert(17,"0");
    if (speed_to_set){
        speed = speed_to_set;
    }
    json::value put_time;
    put_time["speed"] = json::value::number(speed);
    put_time["symTime"] = json::value::string(symTime);
    for (auto a : module_address){
        string path = a + "/time";
        web::http::client::http_client client("path");
        client.request(web::http::methods::PUT, U("/"), put_time).then([](const web::http::http_response& response) {
            return response.extract_json();
        })
        .then([=](const pplx::task<web::json::value>& task) {
            try {
                json::value json_return = task.get();
            }
            catch (const web::http::http_exception& e) {
                std::cout << "error " << std::endl;
            }
        });
    }
}

