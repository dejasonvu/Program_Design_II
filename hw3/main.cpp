#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;
class Data{
public:
    long long id;
    string sign;
    long long time;
};
bool cmp(Data a, Data b){
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
    
    // avoiding the case that there is no data.
    if(inf.size() == 0){
        return 0;
    }

    vector<Data> data;
    for(int i=0; i < inf.size(); i++){
        Data reading;
        reading.id = stoll(inf[i][0]);
        reading.sign = inf[i][1];
        reading.time = stoll(inf[i][2]);

        data.push_back(reading);
    }

    //sorting data
    sort(data.begin(), data.end(), cmp);

    // outputting data
    long long rec_id = data[0].id, rec_overloading_day = 0, rec_sign_forget_days = 0;
    for(int i=0; i < data.size(); i++){
        if(rec_id != data[i].id){
            cout << rec_id << "," << rec_overloading_day << "," << rec_sign_forget_days << "\n";
            rec_id = data[i].id; 
            rec_overloading_day = 0;
            rec_sign_forget_days = 0;
        }
        if(data[i].sign == "sign-in"){
            rec_sign_forget_days++;
        }else{
            if(i==0){
                rec_sign_forget_days++;
            }else if(data[i-1].sign == "sign-in" && (data[i-1].time / 10000) == (data[i].time / 10000) && data[i-1].id == data[i].id){
                rec_sign_forget_days--;
                long long dif_hour = (data[i].time / 100) - (data[i-1].time / 100);
                long long dif = dif_hour * 60 + (data[i].time % 100) - (data[i-1].time % 100);
                if(dif > 480){
                    rec_overloading_day++;
                }
            }else{
                rec_sign_forget_days++;
            }
        }
    }
    cout << rec_id << "," << rec_overloading_day << "," << rec_sign_forget_days << endl;
    return 0;
}