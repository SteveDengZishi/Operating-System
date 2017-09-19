//  Created by Steve DengZishi on 9/18/17.
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
fstream inFile;
//unordered_map to ensure O(1) access
unordered_map<string, int> symTable;
vector<int> baseTable;
vector<int> baseMap;
vector<pair<string,int>>* usageTable;
vector<string> instructionTable;
int moduleNum;
int ND, NU, NT, addressDef, addressUse, absoluteAddress, baseAddress=0;
string sym, instruction, line;

string convertToThreeDigit(int num){
    string string_i=to_string(num);
    size_t i_bit=string_i.length();
    
    for(int l=0;l<(3-i_bit);l++){
        string_i="0"+string_i;
    }
    return string_i;
}

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
    //read module Number
    inFile>>moduleNum;
    //dynamic allocate usage table
    usageTable=new vector<pair<string,int>>[moduleNum];
    
    FOR(i,0,moduleNum){
        //reading definition list
        inFile>>ND;
        while(ND--){
            inFile>>sym>>addressDef;
            absoluteAddress=baseAddress+addressDef;
            symTable[sym]=absoluteAddress;
        }
        //reading usage list
        inFile>>NU;
        while(NU--){
            inFile>>sym>>addressUse;
            absoluteAddress=baseAddress+addressUse;
            usageTable[i].emplace_back(sym,absoluteAddress);
        }
        //reading instruction list
        inFile>>NT;
        //only push baseAddress once per module in baseTable
        baseTable.push_back(baseAddress);
        FOR(i,0,NT){
            inFile>>instruction;
            instructionTable.push_back(instruction);
            //each instruction associate with one baseAddress in baseMap
            baseMap.push_back(baseAddress);
        }
        baseAddress+=NT; //increment base address
    }
    
    //print Symbol Table and close file after first pass
    cout<<"Symbol Table"<<endl;
    unordered_map<string, int>::iterator itr;
    for(itr=symTable.begin();itr!=symTable.end();itr++){
        cout<<itr->first<<"="<<itr->second<<endl;
    }
    inFile.close();
    
    //2nd pass reading, we already have base addresses, definition and usage in absolute addresses.
    inFile.open(fileName,ios::in);
    if(!inFile){
        cerr<<"Could not open the file."<<endl;
        exit(0);
    }
    
    inFile>>moduleNum;
    FOR(i,0,moduleNum){
        //reading definition list
        getline(inFile,line);
        
        //reading usage list
        getline(inFile,line);
        
        //reading instruction list
        getline(inFile,line);
    }
    
    //    DEVEC(instructionTable);
    
    FOR(i,0,moduleNum){
        FOR(j,0,usageTable[i].size()){
            sym=usageTable[i][j].first;
            addressUse=usageTable[i][j].second;
            
            while(instructionTable[addressUse].substr(1,3)!="777"){
                instruction=instructionTable[addressUse];
                int prevAddress=addressUse;
                addressUse=stoi(instruction.substr(1,3))+baseTable[i];
                //changing value entry in the table, the middle part need to convert to three digits
                instructionTable[prevAddress]=instruction.substr(0,1)+convertToThreeDigit(symTable[sym])+instruction.substr(4,1);
            }
            instruction=instructionTable[addressUse];
            instructionTable[addressUse]=instruction.substr(0,1)+convertToThreeDigit(symTable[sym])+instruction.substr(4,1);
        }
    }
    //error checking
    //print the actual output with relocated and resolved addresses
    cout<<endl;
    cout<<"Memory Map"<<endl;
    string finalAddress;
    FOR(i,0,instructionTable.size()){
        //if address is relative, take the first 4 digits and add to base
        if(instructionTable[i].substr(4)=="3") finalAddress=to_string(stoi(instructionTable[i].substr(0,4))+baseMap[i]);
        else if(instructionTable[i].substr(4)=="4") finalAddress=instructionTable[i].substr(0,4);
        else finalAddress=instructionTable[i].substr(0,4);
        cout<<i<<":"<<"\t"<<finalAddress<<endl;
    }
    
    return 0;
}

