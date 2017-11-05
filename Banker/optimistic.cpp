//  Created by Steve DengZishi on 11/4/17.
//  Copyright © 2017 Steve DengZishi. All rights reserved.
//

#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <cmath> // To use sqrt()
#include <list> // To use STL linked-list in dfs topological sort
#include <stack> // To use STL stack in topological sort
#include <queue> // To use STL queue in bfs, dijkstra's, maxflow
#include <cstring> // To use memset()
#include <cstdlib> // To use atoi(), abs(), malloc()
#include <algorithm> // To use sort(), next_permutation(), min(), max() etc.
#include <unordered_map> // To allow O(1) mapping access using key->value
#include <set> // To sort and remove duplicate when inserted
#include <unordered_set> //To remove duplicates and count size
#include <fstream> //To use c++ style read stream from file

//macro here
#define FOR(i,a,b) for(size_t i=a;i<b;i++)
#define DE(x) cout << #x << ":" << x << endl
#define DEIF cout<<"DEBUG: in if"<<endl
#define DEELSE cout<<"DEBUG: in else"<<endl
#define DEIFIF cout<<"DEBUG: in if if"<<endl
#define DEIFELSE cout<<"DEBUG: in if else"<<endl
#define DEELSEIF cout<<"DEBUG: in else if"<<endl
#define DEELSEELSE cout<<"DEBUG: in else else"<<endl
#define ALL(a) a.begin(),a.end()
#define CINLINE(a) getline(cin,a)
#define FILL(a,b) memset(a, b , sizeof(a)) //fill array a with all bs
#define INIT(a) FILL(a,0) //initialize array a with all 0s

//name space here
using namespace std;

//typedef here
typedef vector<int> VI;
typedef vector<pair<int,pair<int,int>>> Edges;
typedef pair<int,int> PII;

//debugging functions here
template<class T>
void DEVEC(vector<T> vec){
    FOR(i,0,vec.size()){
        DE(vec[i]);
    }
    cout<<endl;
}

template<class T>
void DEARRAY(T array[],int num=20){
    FOR(i,0,num+1){
        cout<<array[i]<<" ";
    }
    cout<<endl;
}

//functions, global variables, comparators & Non-STL Data Structures definition here
string fileName;
fstream inFile;
int cycle;
int waitTime;
int taskNum;
int typeNum;
int* resources;
vector<string>* instructions;
int* termination;

struct result{
    int first,second;
    double third;
    
    result(){}
    result(int a, int b, double c):first(a),second(b),third(c){}
};

result* recordTable;

bool checkEnd(){
    bool end=true;
    for(int i=0;i<taskNum;i++){
        if(termination[i]==0) end=false;
    }
    return end;
}

string fetchEffectiveInstruction(int i){
    if(termination[i]!=1) return instructions[i][cycle];
    else return "void";
}

//start of main()
int main(int argc, const char * argv[]) {
    //Taking command line argument
    if(argc==2){
        fileName=argv[1];
    }
    else{
        cerr<<"illegal input, you should provide only one command line arguments with the fileName"<<endl;
        exit(0);
    }
    
    //open input file
    inFile.open(fileName,ios::in);
    inFile>>taskNum>>typeNum;
    
    //dynamic allocation of array of resources in system
    resources = new int[typeNum];
    for(int i=0;i<typeNum;i++){
        inFile>>resources[i];
    }
    //dynamic allocation of array of string instructions and termination status for all tasks
    instructions = new vector<string>[taskNum];
    termination = new int[taskNum]{};
    recordTable = new result[taskNum];
    
    //divide and store instruction lines according to their taskNum
    string instruction;
    getline(inFile,instruction);
    while(getline(inFile,instruction)){
        //cout<<instruction<<endl;
        instructions[(instruction[10]-'0')-1].push_back(instruction);
    }
    //initialized and start cycle 0
    cycle=0;
    while(true){
        //if all tasks have terminated, break out of the infinite loop
        if(checkEnd()==true) break;
        //run one instruction for each task in every round
        for(int i=0;i<taskNum;i++){
            instruction = fetchEffectiveInstruction(i);
            if(instruction!="void"){
                //execute this instruction
                string order=instruction.substr(0,instruction.find(' '));
                int taskId=instruction[10]-'0';
                int resourceType=instruction[12]-'0';
                int resourceQuantity=instruction[14]-'0';
                
                //check what is the instruction
                if(order=="initiate"){
                    
                }
                else if(order=="request"){
                    
                }
                else if(order=="release"){
                    
                }
                else if(order=="compute"){
                    
                }
                else if(order=="terminate"){
                    //terminate task
                    termination[taskId]=1;
                    //record results
                    
                }
            }
        }
        //increment cycle
        cycle++;
    }
    //after execution, print output in the given format
    
    return 0;
}
