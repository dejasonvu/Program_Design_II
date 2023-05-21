#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <algorithm>
using namespace std;
class Data{
public:
    long long id;
    string sign;
    long long time;
    long long yyyymmdd;
    long long diff_day;
};
class Output{
public:
    long long id;
    long long continuous_days;
    long long from_day;
    long long end_day;
};
bool cmp1(Data a, Data b){
    if(a.id < b.id){
        return true;
    }else if(a.id > b.id){
        return false;
    }else{
        if(a.time < b.time){
            return true;
        }else if(a.time > b.time){
            return false;
        }else{
            if(a.sign == "sign-in"){
                return true;
            }else{
                return false;
            }
        }
    }
}
bool cmp2(Output a, Output b){
    if(a.continuous_days > b.continuous_days){
        return true;
    }else if(a.continuous_days < b.continuous_days){
        return false;
    }else{
        if(a.id < b.id){
            return true;
        }else{
            return false;
        }
    }
}
void processDiffDay(vector<Data>& data){
    for(auto &i:data){
        int year = (int)(i.yyyymmdd / 10000);
        int month = (int)((i.yyyymmdd % 10000) / 100);
        int day = (int)(i.yyyymmdd % 100);
        
        tm date = {};
        date.tm_year = year - 1900;
        date.tm_mon = month - 1;
        date.tm_mday = day;
        time_t stamp = mktime(&date);
        i.diff_day = stamp / (60 * 60 * 24);
    }
}
vector<Output> processOutputData(vector<Data>& data){
    vector<Output> output;
    Output reading;
    reading.id = data[0].id;
    reading.continuous_days = 1;
    reading.from_day = data[0].yyyymmdd;
    reading.end_day = data[0].yyyymmdd;
    
    long long rec_id = data[0].id, rec_continuous = 1, rec_from = data[0].yyyymmdd, rec_end = data[0].yyyymmdd, rec_day = data[0].diff_day;
    for(auto &i:data){
        if(rec_id == i.id){
            if(rec_day == i.diff_day){
                continue;
            }else if((rec_day + 1) == i.diff_day){
                rec_continuous++;
                rec_day++;
                rec_end = i.yyyymmdd;
            }else{
                if(reading.continuous_days <= rec_continuous){
                    reading.id = rec_id;
                    reading.continuous_days = rec_continuous;
                    reading.from_day = rec_from;
                    reading.end_day = rec_end;
                }

                //initializing records
                rec_id = i.id;
                rec_continuous = 1;
                rec_from = i.yyyymmdd;
                rec_end = i.yyyymmdd;
                rec_day = i.diff_day;
            }
        }else{
            if(reading.continuous_days <= rec_continuous){
                reading.id = rec_id;
                reading.continuous_days = rec_continuous;
                reading.from_day = rec_from;
                reading.end_day = rec_end;
            }

            //adding this object in the vector
            output.push_back(reading);

            //initializing records
            rec_id = i.id;
            rec_continuous = 1;
            rec_from = i.yyyymmdd;
            rec_end = i.yyyymmdd;
            rec_day = i.diff_day;

            //initializing reading
            reading.id = i.id;
            reading.continuous_days = 1;
            reading.from_day = i.yyyymmdd;
            reading.end_day = i.yyyymmdd;
        }
    }
    if(reading.continuous_days <= rec_continuous){
        reading.id = rec_id;
        reading.continuous_days = rec_continuous;
        reading.from_day = rec_from;
        reading.end_day = rec_end;
    }
    output.push_back(reading);
    return output;
}
int main(int argc, char** argv){
    //reading information
    string filename = argv[1];

    ifstream file(filename);
    vector<vector<string>> inf;

    string line;
    while(getline(file, line)){
        stringstream cut(line);
        string item;
        vector<string> row;
        while(getline(cut, item, ',')){
            row.push_back(item);
        }
        inf.push_back(row);
    }
    file.close();

    vector<Data> data;
    for(auto &i : inf){
        Data reading;
        reading.id = stoll(i[0]);
        reading.sign = i[1];
        reading.time = stoll(i[2]);
        reading.yyyymmdd = reading.time / 10000;

        data.push_back(reading);
    }

    //processing data
    sort(data.begin(), data.end(), cmp1);
    
    vector<Output> output;
    processDiffDay(data);
    output = processOutputData(data);
    
    sort(output.begin(), output.end(),cmp2);

    //outputting
    cout << output[0].id << "," << output[0].continuous_days << "," << output[0].from_day << "," << output[0].end_day << endl;
    cout << output[1].id << "," << output[1].continuous_days << "," << output[1].from_day << "," << output[1].end_day << endl;
    cout << output[2].id << "," << output[2].continuous_days << "," << output[2].from_day << "," << output[2].end_day << endl;
    return 0;
}