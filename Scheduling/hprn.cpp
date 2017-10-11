//
//  hprn.cpp
//  Highest Penalty Ratio Next cpu scheduling
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
string fileName;
string flag;
int* status;//-1 is unstarted, 1 is ready, 2 is running, 3 is blocked, 4 is terminated

int randomOS(int U){
    int randint;
    infileRandom>>randint;
    //cout<<"Using random int: "<<randint<<endl;
    return 1+(randint%U);
}

struct process{
    int A,B,C,M;
    process(int first, int second, int third, int fourth):A(first),B(second),C(third),M(fourth){}
};

struct progress{
    int remainT,ioT,arriveT;
    progress(int arrive,int remain):arriveT(arrive),remainT(remain),ioT(0){}
};

struct result{
    int finishTime,turnTime,ioTime,waitTime;
    result(){}
    result(int first, int second, int third, int fourth):finishTime(first),turnTime(second),ioTime(third),waitTime(fourth){}
};

bool compy(process a, process b){
    return a.A<b.A;
}

bool compShortest(pair<int,progress> a, pair<int,progress> b){
    return a.second.remainT<b.second.remainT;
}

//start of main()
int main(int argc, const char * argv[]) {
    //optimize iostream
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    //reading from the file
    if(argc==2){
        fileName=argv[1];
    }
    else if(argc==3){
        flag=argv[1];
        fileName=argv[2];
    }
    else{
        cerr<<"illegal input, you should provide one or two command line arguments"<<endl;
        exit(0);
    }
    
    infileRandom.open("random-numbers.txt",ios::in);
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
    
    stable_sort(procVec.begin(),procVec.end(),compy);
    
    cout<<"The (sorted) input is:  "<<procNum;
    FOR(i, 0, procNum){
        printf(" (%d %d %d %d) ",procVec[i].A,procVec[i].B,procVec[i].C,procVec[i].M);
    }
    cout<<endl;
    cout<<endl;
    
    cout<<"The scheduling algorithm used was Higest Penalty Ratio Next"<<endl;
    
    //calculations will be different based on different algorithms used
    int finish=0;
    int turnAround=0;
    int io=0;
    int waiting=0;
    float cpuUtil,ioUtil,throughPut,aveTurn,aveWait;
    
    //queue contains all process in ready state, start counting waiting time
    queue<int> inputProg;
    queue<int> scheduler;
    vector<progress> progVec;
    
    FOR(i,0,procNum) {
        //initialize scheduler
        inputProg.push((int)i);
        //initialize progressRecorder with arrival times and total ramaining time
        progVec.emplace_back(procVec[i].A,procVec[i].C);
    }
    
    //set time Elapse to be the first arrival job
    int timeElapse=procVec[inputProg.front()].A;
    while(true){
        //if no job is currently in scheduler, check whether need to input from inputProg or all jobs are done
        if(scheduler.empty()){
            if(inputProg.empty()) break;
            else{
                while(!inputProg.empty()){
                    if(timeElapse>=procVec[inputProg.front()].A){
                        scheduler.push(inputProg.front());
                        inputProg.pop();
                    }
                    else break;
                }
            }
        }
        //pick the next job
        int job=scheduler.front();
        scheduler.pop();
        //check whether next job is ready
        if(timeElapse<progVec[job].arriveT){
            timeElapse=progVec[job].arriveT;
        }
        
        int cpuBurst=randomOS(procVec[job].B);
        int ioBurst=cpuBurst*procVec[job].M;
        //start burst and save state until block
        if(progVec[job].remainT>cpuBurst){
            //start cpu burst
            timeElapse+=cpuBurst;
            progVec[job].remainT-=cpuBurst;
            //add ioT but timeElapse doesn't change
            progVec[job].ioT+=ioBurst;
            //before push back to wait, check whether new task arrives while cpuBursting
            while(!inputProg.empty()){
                if(procVec[inputProg.front()].A<timeElapse+ioBurst){
                    //if process arrives before finishing cpu burst + io burst, push it to scheduler before the unfinished job
                    scheduler.push(inputProg.front());
                    inputProg.pop();
                }
                else break;
            }
            //push back to wait
            scheduler.push(job);
            //update arrival time in progress
            progVec[job].arriveT=timeElapse+ioBurst;
        }
        else{
            timeElapse+=progVec[job].remainT;
            progVec[job].remainT=0;
            //check whether new task arrives while cpuBursting
            while(!inputProg.empty()){
                if(procVec[inputProg.front()].A<timeElapse){
                    //if process arrives before finishing cpu burst, push it to scheduler
                    scheduler.push(inputProg.front());
                    inputProg.pop();
                }
                else break;
            }
            //finished process and update result
            /*
             usedT=cpuBurstT+ioT+waitT
             finishT=arriveT+usedT
             turnT=finishT-arriveT
             remainT=process.C-cpuBurstT
             timeElapse-arrivalT=turnaroundT
             turnaroundT-ioT-C=waitT
             */
            finish=timeElapse;
            //use the initial arrival time in procVec
            turnAround=finish-procVec[job].A;
            io=progVec[job].ioT;
            waiting=turnAround-io-procVec[job].C;
            result done(finish,turnAround,io,waiting);
            procResult[job]=done;
        }
    }
    
    //output all process indentifications
    FOR(i,0,procNum){
        printf("\nProcess %d:\n",(int)i);
        printf("\t(A,B,C,M) = (%d,%d,%d,%d)\n",procVec[i].A,procVec[i].B,procVec[i].C,procVec[i].M);
        printf("\tFinishing time: %d\n",procResult[(int)i].finishTime);
        printf("\tTurnaround time: %d\n",procResult[(int)i].turnTime);
        printf("\tI/O time: %d\n",procResult[(int)i].ioTime);
        printf("\tWaiting time: %d\n",procResult[(int)i].waitTime);
    }
    //output summary data
    int totalCPU=0;
    int totalIO=0;
    int totalTurn=0;
    int totalWait=0;
    FOR(i,0,procNum){
        totalCPU+=procVec[i].C;
        totalIO+=progVec[i].ioT;
        totalTurn+=procResult[i].turnTime;
        totalWait+=procResult[i].waitTime;
    }
    cpuUtil=(float)totalCPU/(float)finish;
    ioUtil=(float)totalIO/(float)finish;
    throughPut=100.0/(float)finish*(float)procNum;
    aveTurn=(float)totalTurn/(float)procNum;
    aveWait=(float)totalWait/(float)procNum;
    
    printf("\nSummary Data: \n");
    printf("\tFinishing Time: %d\n",finish);
    printf("\tCPU utilization: %f\n",cpuUtil);
    printf("\tI/O utilization: %f\n",ioUtil);
    printf("\tThroughPut: %f processes per hundred cycles\n",throughPut);
    printf("\tAverage turnaround time: %f\n",aveTurn);
    printf("\tAverage waiting time: %f\n",aveWait);
    return 0;
}
