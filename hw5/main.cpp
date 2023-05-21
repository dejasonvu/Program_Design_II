#include <iostream>
#include <vector>
#include <cctype>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>
using namespace std;
class EnglishString{
public:
    long long id;
    string str;
    vector<string> words;
};
class Search{
public:
    string str;
    vector<string> words;
    vector<long long> output;
};
bool cmp(EnglishString &a, EnglishString &b){
    if(a.id <= b.id){
        return true;
    }else{
        return false;
    }
}
bool pred(char c){
    return (!isalpha(c)) && (!isspace(c)) ;
}
unsigned char lowerCase(unsigned char c){
    return tolower(c);
}
void removePunctuationAndNumber(string& str){
    //removing any punctuation
    str.erase(remove_if(str.begin(), str.end(), pred), str.end());
}
void smallLetter(string& str){
    //turnnig a string into small letters
    transform(str.begin(), str.end(), str.begin(), lowerCase);
}
int main(int argc, char** argv){
    //reading corpus.txt 
    string filename1 = argv[1];

    ifstream file1(filename1);
    vector<EnglishString> cor;

    string cor_line;

    while(getline(file1, cor_line)){
        stringstream cor_cut(cor_line);
        string cor_item;
        EnglishString cor_element;
        
        getline(cor_cut, cor_item, ',');
        cor_element.id = stoll(cor_item);
        getline(cor_cut, cor_item);
        cor_element.str = cor_item;

        cor.push_back(cor_element);
    }
    file1.close();

    //processing corpus.txt
    sort(cor.begin(), cor.end(), cmp);
    for(auto &i : cor){
        removePunctuationAndNumber(i.str);
        smallLetter(i.str);
    }
    for(auto &i : cor){
        stringstream ss1(i.str);
        string item1;
        while(getline(ss1, item1, ' ')){
            if(!item1.empty()){
                i.words.push_back(item1);
            }
        }
    }
    
    //reading query.txt
    string filename2 = argv[2];

    ifstream file2(filename2);
    vector<Search> que;

    string que_line;
    while(getline(file2, que_line)){
        Search que_element;
        que_element.str = que_line;

        que.push_back(que_element);
    }
    file2.close();

    //processing query.txt
    for(auto &i : que){
        smallLetter(i.str);
    }
    for(auto &i : que){
        stringstream ss2(i.str);
        string item2;
        while(getline(ss2, item2, ' ')){
            if(!item2.empty()){
                i.words.push_back(item2);
            }
        }
    }

    //searching
    for(auto &k : que){
        for(auto &i : cor){
            bool flag = true;
            //宣告並初始化book，這個book紀錄各個Search中搜尋字詞是否有在字串中搜到
            map<string, bool> book;
            for(auto &j : k.words){
                book[j] = false;
            }

            for(auto &j : i.words){
                for(auto &z : k.words){
                    if(z == j){
                        book[z] = true;
                        continue;
                    }
                }
            }
            for(auto &r : book){
                if(r.second != true){
                    flag = false;
                    break;
                }
            }
            //如果各個Search中搜尋字詞都搜得到，就加入output
            if(flag == true){
                k.output.push_back(i.id);
            }
        }
    }

    //outputting
    for(auto &i : que){
        if(i.output.size() == 0){
            cout << "-1" << endl;
        }else if(i.output.size() == 1){
            cout << i.output[0] << endl;
        }else{
            cout << i.output[0];
            for(long long j = 1; j < i.output.size(); j++){
                cout << " " << i.output[j];
            }
            cout << endl;
        }
    }
    for(auto &i : cor){
        cout << i.id;
        for(auto &j : i.words){
            cout << " " << j;
        }
        cout << endl;
    }
    return 0;
}
/*
    for(auto &i : que){
        if(i.output.size() == 0){
            cout << "-1" << endl;
        }else if(i.output.size() == 1){
            cout << i.output[0] << endl;
        }else{
            cout << i.output[0];
            for(int j = 1; j < i.output.size(); j++){
                cout << " " << i.output[j];
            }
            cout << endl;
        }
    }

    for(auto &i : cor){
        cout << i.id;
        for(auto &j : i.words){
            cout << " " << j;
        }
        cout << endl;
    }

    for(auto &i : que){
        for(auto &j : i.words){
            cout << j << endl;
        }
        cout << endl;
    }

    for(auto &i : cor){
        cout << i.id << "," << i.str << endl;
    }
*/