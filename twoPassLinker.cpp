//  2.cpp
//  HackerRankQ
//
//  Created by Steve DengZishi on 5/30/17.
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
//#include <unordered_map> // To allow O(1) mapping access using key->value
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
fstream inFile;
vector<pair<string,int>> symTable;
vector<pair<string,int>> usageTable;
vector<int> baseTable;
vector<string> instructionTable;
int moduleNum;
int ND, NU, NT, addressDef, addressUse, absoluteAddress, baseAddress=0;
string sym, instruction, line;

//start of main()
int main(int argc, const char * argv[]) {
    //optimize iostream
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    //read input fileName from std in
    string fileName;
    cin>>fileName;
    //open input file as fstream (1st pass)
    inFile.open(fileName,ios::in);
    if(!inFile){
        cerr<<"Could not open the file."<<endl;
        exit(0);
    }
    //read module Name
    inFile>>moduleNum;
    
    while(moduleNum--){
        baseTable.push_back(baseAddress);
        //reading definition list
        inFile>>ND;
        while(ND--){
            inFile>>sym>>addressDef;
            absoluteAddress=baseAddress+addressDef;
            symTable.emplace_back(sym,absoluteAddress);
        }
        //reading usage list
        inFile>>NU;
        while(NU--){
            inFile>>sym>>addressUse;
            absoluteAddress=baseAddress+addressUse;
            usageTable.emplace_back(sym,absoluteAddress);
        }
        //reading instruction list
        inFile>>NT;
        baseAddress+=NT; //increment base address
        while(NT--){
            inFile>>instruction;
        }
    }
    //print Symbol Table and close file after first pass
    cout<<"Symbol Table"<<endl;
    FOR(i,0,symTable.size()){
        cout<<symTable[i].first<<"="<<symTable[i].second<<endl;
    }
    inFile.close();
    
    //2nd pass reading, we already have base addresses, definition and usage in absolute addresses.
    inFile.open(fileName,ios::in);
    if(!inFile){
        cerr<<"Could not open the file."<<endl;
        exit(0);
    }
    
    inFile>>moduleNum;
    while(moduleNum--){
        //reading definition list
        getline(inFile,line);
        
        //reading usage list
        getline(inFile,line);
        
        //reading instruction list
        inFile>>NT;
        while(NT--){
            inFile>>instruction;
            instructionTable.push_back(instruction);
        }
    }
    
    //error checking
    //print the actual output with relocated and resolved addresses
    
    
    return 0;
}
