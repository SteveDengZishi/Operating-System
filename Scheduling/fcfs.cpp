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
int cpuStatus;
int cycle;
int procNum;
string table[5];

int randomOS(int U){
    int randint;
    infileRandom>>randint;
    if(flag=="--verbose") cout<<"Find burst when choosing ready process to run: "<<randint<<endl;
    return 1+(randint%U);
}

struct process{
    int A,B,C,M;
    process(int first, int second, int third, int fourth):A(first),B(second),C(third),M(fourth){}
};

struct progress{
    int status; //0 is unstarted, 1 is ready, 2 is running, 3 is blocked, 4 is terminated
    int remainT,ioT,arriveT,remainingBurst,remainingIO,waitT;
    progress(int arrive,int remain):arriveT(arrive),remainT(remain),ioT(0),status(0),remainingBurst(0),remainingIO(0),waitT(0){}
};

struct result{
    int finishTime,turnTime,ioTime,waitTime;
    result(){}
    result(int first, int second, int third, int fourth):finishTime(first),turnTime(second),ioTime(third),waitTime(fourth){}
};

vector<process> procVec;
unordered_map<int, result> procResult;
//queue contains all process in ready state, start counting waiting time
queue<int> inputProg;
queue<int> scheduler;
vector<progress> progVec;

bool compy(process a, process b){
    return a.A<b.A;
}

bool compShortest(pair<int,progress> a, pair<int,progress> b){
    return a.second.remainT<b.second.remainT;
}

void printStatus(int cycle){
    printf("Before cycle    %d:",cycle);
    FOR(i,0,procNum){
        cout<<" "<<table[progVec[i].status];
        if(progVec[i].status==3){
            cout<<" "<<progVec[i].remainingIO;
        }
        else{
            cout<<" "<<progVec[i].remainingBurst;
        }
    }
    cout<<endl;
}

//start of main()
int main(int argc, const char * argv[]) {
    
    //reading ternimal argument
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
    
    
    //parsing input
    infileRandom.open("random-numbers.txt",ios::in);
    infileProcess.open(fileName,ios::in);
    int A,B,C,M;
    infileProcess>>procNum;
    
    FOR(i,0,procNum){
        infileProcess>>A>>B>>C>>M;
        process tmp=process(A, B, C, M);
        procVec.push_back(tmp);
    }
    
    //output headers
    cout<<"The original input was: "<<procNum;
    FOR(i,0,procNum){
        printf(" (%d %d %d %d) ",procVec[i].A,procVec[i].B,procVec[i].C,procVec[i].M);
    }
    cout<<endl;
    
    //procVect is currently sorted
    stable_sort(procVec.begin(),procVec.end(),compy);
    
    cout<<"The (sorted) input is:  "<<procNum;
    FOR(i, 0, procNum){
        printf(" (%d %d %d %d) ",procVec[i].A,procVec[i].B,procVec[i].C,procVec[i].M);
    }
    cout<<endl;
    cout<<endl;
    
    cout<<"The scheduling algorithm used was First Come First Served"<<endl;
    
    //initialize variables
    cycle=0;
    cpuStatus=-1; //-1 means no process is running otherwise will be index of process
    float cpuUtil,ioUtil,throughPut,aveTurn,aveWait;
    
    //initialize procTable to be all unstarted
    //0 is unstarted, 1 is ready, 2 is running, 3 is blocked, 4 is terminated
    table[0]="unstarted";
    table[1]="ready";
    table[2]="running";
    table[3]="blocked";
    table[4]="terminated";
    
    FOR(i,0,procNum) {
        //initialize scheduler
        inputProg.push((int)i);
        //initialize progressRecorder with arrival times and total ramaining time
        progress tmp=progress(procVec[i].A, procVec[i].C);
        progVec.push_back(tmp);
    }
    
    //printing cycle 0, all unstarted
    if(flag=="--verbose") printStatus(cycle);
    
    //cycle loop
    int blocked=0;
    while(true){
    
        //check whether blocking progress is ready
        FOR(i,0,procNum){
            if(progVec[i].status==3){
                if(progVec[i].remainingIO==0){
                    //change status to ready
                    progVec[i].status=1;
                    scheduler.push((int)i);
                }
            }
        }
        
        //count how many process were in blocked status
        int blockingCnt=0;
        FOR(i,0,procNum){
            if(progVec[i].status==3){
                blockingCnt++;
            }
        }
        
        if(blockingCnt>0) blocked++;
        
        //breaking condition: if no coming input && no process in ready status && no blocked process
        if(inputProg.size()==0 && scheduler.size()==0 && blockingCnt==0 && cpuStatus==-1) break;
        

        //check whether new task is coming in
        while(progVec[inputProg.front()].arriveT==cycle && inputProg.size()!=0){
            //if yes, push to scheduler and change status to ready
            progVec[inputProg.front()].status=1;
            scheduler.push(inputProg.front());
            inputProg.pop();
        }
        
        //pick jobs to execute
        //if current cpuStatus is running, continue the execution
        //else pick a new job
        if(cpuStatus==-1){
            if(!scheduler.empty()) {
                cpuStatus=scheduler.front();
                scheduler.pop();
                //random number is generated with new job
                int cpuBurst=randomOS(procVec[cpuStatus].B);
                int ioBurst=cpuBurst*procVec[cpuStatus].M;
                //initialize current burst and remaining time
                //selected process start running status
                progVec[cpuStatus].status=2;
                progVec[cpuStatus].remainingBurst=cpuBurst;
                progVec[cpuStatus].remainingIO=ioBurst;
            }
        }
        
        //increment cycles
        cycle++;
        
        //printing status before round starts
        if(flag=="--verbose") printStatus(cycle);
        
        //run down ioBurst & increase IOT for all other process and add wait time is they are ready
        FOR(i,0,procNum){
            if(progVec[i].status==3){
                if(progVec[i].remainingIO>1){
                    progVec[i].ioT++;
                    progVec[i].remainingIO--;
                }
                else{
                    progVec[i].ioT++;
                    progVec[i].remainingIO=0;
                }
            }
        }
        
        FOR(i,0,procNum){
            //if the status is ready but not running
            if(progVec[i].status==1){
                progVec[i].waitT++;
            }
        }
        
        //starting cpuBurst
        if(cpuStatus!=-1){// to make sure cpu is not in idle state
            //run down burstRemain, run down ioRemain and increment ready wait time
            //if there is 1 and more time to run
            if(progVec[cpuStatus].remainingBurst>1 && progVec[cpuStatus].remainT>1){
                progVec[cpuStatus].remainingBurst--;
                progVec[cpuStatus].remainT--;
            }
            //process terminates
            else if(progVec[cpuStatus].remainT==1){
                progVec[cpuStatus].remainingBurst=0;
                progVec[cpuStatus].remainT=0;
                progVec[cpuStatus].status=4;
                //record to procResult
                procResult[cpuStatus]=result(cycle,cycle-progVec[cpuStatus].arriveT,progVec[cpuStatus].ioT,progVec[cpuStatus].waitT);
                cpuStatus=-1;
            }
            //else change to status blocked and start ioBurst
            else if(progVec[cpuStatus].remainingBurst==1){
                progVec[cpuStatus].remainingBurst=0;
                progVec[cpuStatus].remainT--;
                progVec[cpuStatus].status=3;
                cpuStatus=-1;
            }
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
    int totalTurn=0;
    int totalWait=0;
    FOR(i,0,procNum){
        totalCPU+=procVec[i].C;
        totalTurn+=procResult[(int)i].turnTime;
        totalWait+=procResult[(int)i].waitTime;
    }
    cpuUtil=(float)totalCPU/(float)cycle;
    ioUtil=(float)blocked/(float)cycle;
    throughPut=100.0/(float)cycle*(float)procNum;
    aveTurn=(float)totalTurn/(float)procNum;
    aveWait=(float)totalWait/(float)procNum;
    
    printf("\nSummary Data: \n");
    printf("\tFinishing Time: %d\n",cycle);
    printf("\tCPU utilization: %f\n",cpuUtil);
    printf("\tI/O utilization: %f\n",ioUtil);
    printf("\tThroughPut: %f processes per hundred cycles\n",throughPut);
    printf("\tAverage turnaround time: %f\n",aveTurn);
    printf("\tAverage waiting time: %f\n",aveWait);
    return 0;
}
