#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

class Employee{
    public:
    int id;
    int age;
    string gender;
    int salary;
};
bool cmp(Employee a, Employee b){
    if(a.salary < b.salary){
        return true;
    }else if(a.salary > b.salary){
        return false;
    }else{
        if(a.age < b.age){
            return true;
        }else if(a.age > b.age){
            return false;
        }else{
            if(a.gender == "f" && b.gender == "m"){
                return true;
            }else if(a.gender == "m" && b.gender == "f"){
                return false;
            }else{
                if(a.id < b.id){
                    return true;
                }else{
                    return false;
                }
            }
        }
    }
}
int main(int argc, char** argv){
    //getting data
    string filename = argv[1];

    ifstream file(filename);
    vector<vector<string>> data;

    string line;
    while(getline(file, line)){
        vector<string> num;
        stringstream cut(line);
        string item;
        while(getline(cut, item, ',')){ 
            num.push_back(item);
        }
        data.push_back(num);
    }
    file.close();

    vector<Employee> vec_emp;
    
    for(int i=0;i < data.size(); i++){
        Employee emp;
        emp.id = stoi(data[i][0]);
        emp.age = stoi(data[i][1]);
        emp.gender = data[i][2];
        emp.salary = stoi(data[i][3]);
        
        vec_emp.push_back(emp);
    }
    // sorting data 
    sort(vec_emp.begin(),vec_emp.end(),cmp);

    // outputting data
    int rec = vec_emp[0].salary;
    cout << rec;
    for(int i=0;i < data.size(); i++){
        if(rec < vec_emp[i].salary){
            rec = vec_emp[i].salary;
            cout << "\n" << rec;
        }
        cout << "," << vec_emp[i].id;
    }
    cout << endl;
    return 0;
}