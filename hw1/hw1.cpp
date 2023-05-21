#include <iostream>
#include <string>
using namespace std;

int main(int argc, char** argv){
   int max = 0, test = 0; 
   int arg = stoi(argv[1]); 
   for(int i = 0; i <= arg; i++){
      test = i * (arg - i);
      if(test > max)
         max = test;
      
      cout << "(" << i << "," << arg - i << ")\n";         
   }
   
   cout << max << endl;
   return 0;
}
