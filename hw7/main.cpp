#include <iostream>
#include <vector>
#include <cmath>
#include <cctype>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <unordered_set>
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
    vector<long long> output;
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

    unordered_map<string, unordered_set<long long>> directionary;
    unordered_map<string, unordered_map<long long, long long>> times;
    unordered_map<long long, long long> num_word;

    for(auto &i : cor){
        stringstream ss1(i.str);
        string item1;
        while(getline(ss1, item1, ' ')){
            if(!item1.empty()){
                directionary[item1].insert(i.id);
                times[item1][i.id]++;
                num_word[i.id]++;
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
    
    //calculating and preparing the data for output
    int k = stoi(argv[3]);
    double epsilon = 1e-12;
    for(auto &i : que){
        unordered_map<long long, unordered_map<string, double>>rank;
        unordered_map<long long, double> rank_sum;
        for(auto &j : i.words){
            if(directionary[j].size() == 0){
                continue;
            }
            double idf = log10((double)cor.size() / directionary[j].size());
            if(idf == 0){
                continue;
            }
            for(auto &z : directionary[j]){
                rank[z][j] += (idf * (double)times[j][z] / num_word[z]);
            }
        }
        for(auto &j : rank){
            unordered_map<string, double>copy_rank = j.second;

            if(copy_rank.size() <= 3){
                for(auto &q : copy_rank){
                    rank_sum[j.first] += q.second;
                }
            }else{
                for(int c=0; c<3; c++){
                    string rec = copy_rank.begin()->first;
                    for(auto &it : copy_rank){
                        if(directionary[it.first].size() < directionary[rec].size()){
                            rec = it.first;
                        }else if(directionary[it.first].size() == directionary[rec].size()){
                            if(it.second > copy_rank[rec]){
                                rec = it.first;
                            }
                        }
                    }
                    rank_sum[j.first] += copy_rank[rec];
                    copy_rank.erase(rec);
                }
            }
        }
        for(int t=0; t<k; t++){
            if(rank_sum.empty()){
                break;
            }
            long long rec = rank_sum.begin()->first;
            for(auto &it : rank_sum){
                if(abs(it.second - rank_sum[rec]) < epsilon){
                    if(it.first < rec){
                        rec = it.first;
                    }
                }else{
                    if(it.second > rank_sum[rec]){
                        rec = it.first;
                    }
                }
            }
            i.output.push_back(rec);
            rank_sum.erase(rec);
        }
        if(i.output.size() < k){
            int out_size = i.output.size();
            for(int q=0; q < k - out_size; q++){
                i.output.push_back(-1);
            }
        }
    }
    
    //outputting
    for(auto &i : que){
        if(i.output.size() == 0){
            continue;
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
    return 0;
}