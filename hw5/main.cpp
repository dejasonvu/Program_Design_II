#include <iostream>
#include <vector>
#include <set>
#include <cctype>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <unordered_map>
using namespace std;
class EnglishString{
public:
    long long id;
    string str;
};
class Search{
public:
    string str;
    vector<string> words;
    set<long long> output;
};
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
    for(auto &i : cor){
        removePunctuationAndNumber(i.str);
        smallLetter(i.str);
    }

    unordered_map<string, set<long long>> directionary;

    for(auto &i : cor){
        stringstream ss1(i.str);
        string item1;
        while(getline(ss1, item1, ' ')){
            if(!item1.empty()){
                directionary[item1].insert(i.id);
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
        if(k.words.size() == 1){
            k.output = directionary[k.words[0]];
        }else if(k.words.size() == 2){
            set_intersection(directionary[k.words[0]].begin(), directionary[k.words[0]].end(), directionary[k.words[1]].begin(), directionary[k.words[1]].end(), inserter(k.output, k.output.begin()));
        }else if(k.words.size() == 3){
            set<long long> mid;
            set_intersection(directionary[k.words[0]].begin(), directionary[k.words[0]].end(), directionary[k.words[1]].begin(), directionary[k.words[1]].end(), inserter(mid, mid.begin()));
            set_intersection(mid.begin(), mid.end(), directionary[k.words[2]].begin(), directionary[k.words[2]].end(), inserter(k.output, k.output.begin()));
        }
    }
    

    //outputting
    for(auto &i : que){
        if(i.output.size() == 0){
            cout << "-1" << endl;
        }else if(i.output.size() == 1){
            cout << *(i.output.begin()) << endl;
        }else{
            set<long long> :: iterator it;
            it = i.output.begin();
            cout << *it;

            advance(it, 1);
            for(; it != i.output.end(); it++){
                cout << " " << *it;
            }
            cout << endl;
        }
    }
    return 0;
}