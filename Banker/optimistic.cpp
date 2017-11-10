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
int perWait;
int taskNum;
int typeNum;
int* resources;
vector<string>* instructions;
int* status;//0 is unstarted, 1 is terminated, 2 is running, 3 is blocked, 4 is prioritized
int* release;
int* waiting;
int* compute;
int** claims;
int** holding;
queue<int> blocked;
vector<vector<string>::iterator> vecOfInterators;//keep tracking about which instruction it is currently at, in case of blocking, the iterator will stop moving

//struct to store the final result when task terminates, need to initialize to 0s to prevent garbage vaule
struct result{
    int first=0,second=0,third=0;
    
    result(){}
    result(int a, int b, double c):first(a),second(b),third(c){}
};

result* recordTable;

bool checkEnd(){
    bool end=true;
    for(int i=0;i<taskNum;i++){
        if(status[i]!=1) end=false;
    }
    return end;
}

//if the task is not terminated, blocked, or prioritized in blocked processes return the next instruction using iterators
string fetchEffectiveInstruction(int i){
    if(status[i]!=1 && status[i]!=4 && status[i]!=3) return *vecOfInterators[i];
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
    status = new int[taskNum]{};
    recordTable = new result[taskNum]{};
    waiting = new int[taskNum]{};
    release = new int[typeNum]{};
    compute = new int[taskNum]{};
    claims = new int*[taskNum];
    
    memset(compute,-1,taskNum*sizeof(int));
    memset(release,0,typeNum*sizeof(int));
    
    for(int i=0;i<taskNum;i++){
        claims[i]=new int[typeNum]{};
    }
    
    holding = new int*[taskNum];
    for(int i=0;i<taskNum;i++){
        holding[i]=new int[typeNum]{};
    }
    
    //divide and store instruction lines according to their taskNum
    string instruction;
    getline(inFile,instruction);
    
    while(getline(inFile,instruction)){
        if(instruction.size()!=0) instructions[(instruction[10]-'0')-1].push_back(instruction);
    }
    
    //initialized and start cycle 0
    cycle=0;
    //initialized iterators to point to the beginning of the instructions
    FOR(i,0,taskNum){
        vector<string>::iterator itrS=instructions[i].begin();
        vecOfInterators.push_back(itrS);
    }
    //start of the cycles
    while(true){
        //if all tasks have terminated, break out of the infinite loop
        if(checkEnd()==true) break;
        //clear and store how many resources are released in the round, as they can only be added at the end of the round
        memset(release,0,typeNum*sizeof(int));
        
        //CHECKING BLOCKING PROCESS FIRST
        int blockingNum=0;
        FOR(i,0,taskNum){
            if(status[i]==3) blockingNum++;
        }
        
        while(blockingNum--){
            int i=blocked.front();
            blocked.pop();
            
            //the previous request was not granted
            if(status[i]!=1){
                
                string instruction=*vecOfInterators[i];
                //cout<<"INSTRUCTION: "<<instruction<<endl;
                int taskId=instruction[10]-'0';
                int taskIndex=taskId-1;
                int resourceType=instruction[12]-'0';
                int resourceIndex=resourceType-1;
                int requestQuantity=instruction[14]-'0';
                
                //if able to grant request
                if(resources[resourceIndex]>=requestQuantity){
                    printf("Grants request of %d resource type %d to task %d\n",requestQuantity,resourceType,taskId);
                    resources[resourceIndex]-=requestQuantity;
                    holding[taskIndex][resourceIndex]+=requestQuantity;
                    //task prioritized need reset to running at the end of cycle
                    status[taskIndex]=4;
                    vecOfInterators[i]++;
                }
                //if not check if it is in a deadlock
                else{
                    bool deadlock=true;
                    int blockNum=0;
                    FOR(j,0,taskNum){
                        if(status[j]==3) blockNum++;
                        if(status[j]!=3 && status[j]!=1) deadlock=false;
                    }
                    if(blockNum==1) deadlock=false;
                    //if not deadlock block again and increase wait time
                    if(!deadlock){
                        printf("Task %d's request could not be granted\n",(int)i+1);
                        status[i]=3;
                        waiting[i]++;
                        blocked.push(i);
                    }
                    //else keep aborting lowest index task until deadlock is solved
                    else{
                        while(deadlock){
                            //finding lowest index task
                            int lowestId=0;
                            FOR(j,0,taskNum){
                                if(status[j]==3){
                                    lowestId=(int)j;
                                    break;
                                }
                            }
                            //abort the task with lowestId
                            cout<<"Deadlock occurred, Aborting task "<<lowestId+1<<endl;
                            status[lowestId]=1;
                            FOR(j,0,typeNum){
                                //release resources
                                resources[j]+=holding[lowestId][j];
                                holding[lowestId][j]=0;
                            }
                            //check whether deadlock still exists
                            lowestId=0;
                            FOR(j,0,taskNum){
                                if(status[j]==3){
                                    lowestId=(int)j;
                                    break;
                                }
                            }
                            string instruction=*vecOfInterators[lowestId];
                            int taskId=instruction[10]-'0';
                            int taskIndex=taskId-1;
                            int resourceType=instruction[12]-'0';
                            int resourceIndex=resourceType-1;
                            int requestQuantity=instruction[14]-'0';
                            
                            if(resources[resourceIndex]>=requestQuantity) deadlock=false;
                            
                        }
                    }
                }
            }
        }
        
        for(int i=0;i<taskNum;i++){
            
            //The rest of the tasks that is not blocked, prioritized or terminated
            instruction = fetchEffectiveInstruction(i);
            
            if(instruction!="void"){
                
                //execute this instruction
                string order=instruction.substr(0,instruction.find(' '));
                int taskId=instruction[10]-'0';
                int taskIndex=taskId-1;
                int resourceType=instruction[12]-'0';
                int resourceIndex=resourceType-1;
                int requestQuantity=instruction[14]-'0';
                
                
                //check what is the instruction
                if(order=="initiate"){
                    
                    //fill up max claim arrays
                    claims[taskIndex][resourceIndex]=requestQuantity;
                    status[taskIndex]=2;
                    
                    vecOfInterators[taskIndex]++;
                    printf("Initiates max claim of %d resources type %d on task %d\n",requestQuantity,resourceType,taskId);
                }
                
                else if(order=="request"){
                    if(resources[resourceIndex]>=requestQuantity){
                        //grant request and deduct from system
                        resources[resourceIndex]-=requestQuantity;
                        holding[taskIndex][resourceIndex]+=requestQuantity;
                        vecOfInterators[taskIndex]++;
                        printf("Grants request of %d resource type %d to task %d\n",requestQuantity,resourceType,taskId);
                    }
                    else{
                        //do not grant request and block
                        status[taskIndex]=3;
                        waiting[taskIndex]++;
                        blocked.push(taskIndex);
                        printf("Task %d's request could not be granted\n", taskId);
                    }
                }
                
                else if(order=="release"){
                    //add resources back to buffer but only add to system in the end of round
                    release[resourceIndex]+=requestQuantity;
                    holding[taskIndex][resourceIndex]-=requestQuantity;
                    vecOfInterators[taskIndex]++;
                    printf("Task %d releases %d type %d resource\n",taskId,requestQuantity,resourceType);
                }
                
                else if(order=="compute"){
                    //if compute round is uninitialized, initialize it and compute -1 round
                    if(compute[taskIndex]==-1){
                        compute[taskIndex]=resourceType;
                        compute[taskIndex]--;
                    }
                    //else continue to compute;
                    else{
                        compute[taskIndex]--;
                    }
                    //check whether finished computation, if yes move to the next instruction
                    if(compute[taskIndex]==0){
                        vecOfInterators[taskIndex]++;
                        printf("Computes on task %d, remaining %d rounds\n",taskId,compute[taskIndex]);
                        compute[taskIndex]=-1;
                    }
                    //output hold resources and do computation
                    else printf("Computes on task %d, remaining %d rounds\n",taskId,compute[taskIndex]);
                }
                
                else if(order=="terminate"){
                    printf("Terminates task %d\n",taskId);
                    //terminate task
                    status[taskIndex]=1;
                    //release all remaining resource if still holding
                    FOR(j,0,typeNum){
                        if(holding[taskIndex][j]>0){
                            resources[j]+=holding[taskIndex][j];
                            holding[taskIndex][j]=0;
                        }
                    }
                    //record results
                    waitTime=waiting[taskIndex];
                    perWait=(int)round((waitTime*100)/cycle);
                    result data=result(cycle,waitTime,perWait);
                    recordTable[taskIndex]=data;
                }
            }
        }
        
        //set prioritized task back to running
        FOR(i,0,taskNum){
            if(status[i]==4) status[i]=2;
        }
        
        //add back released resources, to be used in the next round
        FOR(i,0,typeNum) resources[i]+=release[i];
        
        //increment cycle
        printf("\n");
        cycle++;
    }
    
    //after execution, print output in the given format
    printf("\n\t\tFIFO\n");
    int totalRun=0;
    int totalWait=0;
    FOR(i,0,taskNum){
        result info=recordTable[i];
        if(info.first==0) printf("Task %d\taborted\n",(int)i+1);
        else {
            printf("Task %d\t%d\t%d\t%d%%\n",(int)i+1,info.first,info.second,info.third);
            totalRun+=info.first;
            totalWait+=info.second;
        }
    }
    
    printf("total\t%d\t%d\t%d%%\n",totalRun,totalWait,(int)round((totalWait*100/totalRun)));
    
    //delete heap memory
    delete [] resources;
    delete [] instructions;
    delete [] status;
    delete [] release;
    delete [] waiting;
    delete [] recordTable;
    
    FOR(i,0,taskNum){
        delete [] claims[i];
    }
    delete [] claims;
    
    FOR(i,0,taskNum){
        delete [] holding[i];
    }
    delete [] holding;
    
    return 0;
}
