//
//  fcfs.cpp
//  First Come First Serve cpu scheduling
//
//  Created by Steve DengZishi on 10/8/17.
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
fstream infileProcess;
fstream infileRandom;

int randomOS(int U){
    int randint;
    infileRandom.open("random-numbers.txt",ios::in);
    infileRandom>>randint;
    return 1+(randint%U);
}

struct process{
    int A,B,C,M;
    process(int first, int second, int third, int fourth):A(first),B(second),C(third),M(fourth){}
};

struct result{
    int finishTime,turnTime,ioTime,waitTime;
    result(int first, int second, int third, int fourth):finishTime(first),turnTime(second),ioTime(third),waitTime(fourth){}
};

//start of main()
int main(int argc, const char * argv[]) {
    //optimize iostream
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    //reading from the file
    string fileName = argv[1];
    infileProcess.open(fileName,ios::in);
    int procNum,A,B,C,M;
    infileProcess>>procNum;
    vector<process> procVec;
    unordered_map<int, result> procResult;
    
    FOR(i,0,procNum){
        infileProcess>>A>>B>>C>>M;
        procVec.emplace_back(A,B,C,M);
    }
    
    
    
    
    //output headers
    cout<<"The original input was: "<<procNum;
    FOR(i,0,procNum){
        printf(" (%d %d %d %d) ",procVec[i].A,procVec[i].B,procVec[i].C,procVec[i].M);
    }
    cout<<endl;
    
    sort(procVec.begin(),procVec.end());
    
    cout<<"The (sorted) input is:  "<<procNum;
    FOR(i, 0, procNum){
        printf(" (%d %d %d %d) ",procVec[i].A,procVec[i].B,procVec[i].C,procVec[i].M);
    }
    cout<<endl;
    cout<<endl;
    
    cout<<"The scheduling algorithm used was First Come First Served"<<endl;
    cout<<endl;
    
    //calculations will be different based on different algorithms used
    int finish=0;
    int turnAround=0;
    int io=0;
    int waiting=0;
    float cpuUtil,ioUtil,throughPut,aveTurn,aveWait;
    
    //output all process indentifications
    FOR(i,0,procNum){
        printf("Process %d:",(int)i);
        printf("\t(A,B,C,M) = (%d,%d,%d,%d)",procVec[i].A,procVec[i].B,procVec[i].C,procVec[i].M);
        printf("\tFinishing time: %d",procResult[(int)i].finishTime);
        printf("\tTurnaround time: %d",procResult[(int)i].turnTime);
        printf("\tI/O time: %d",procResult[(int)i].ioTime);
        printf("\tWaiting time: %d",procResult[(int)i].waitTime);
    }
    //output summary data
    
    return 0;
}
