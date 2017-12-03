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
#define DEHERE cout<<"here"<<endl
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
int M,P,S,J,N,procNum,cycle,frameNum,pageNum;
int lastFrame;
int totalFault=0,totalEvict=0,totalRes=0.0;
double averRes=0.0;
string R;
fstream inFile;


//struct to store the final result when task terminates, need to initialize to 0s to prevent garbage vaule

struct frame{
    bool used=false;
    int lastUse=-1;
    int process_id, page_id;
    
    frame(){}
};

double randDouble(){
    int randint;
    inFile>>randint;
    double result = (double) randint / 2147483648.0;
    return result;
}

struct process{
    int cur, faultCount = 0, evictCount = 0, recidencyTime = 0, remainNum;
    double A, B, C;
    bool finished = false;
    vector<int> page_table;
    vector<int> load_time;
    process(){};
    process(double _A, double _B, double _C, int index){
        A = _A;
        B = _B;
        C = _C;
        cur = (index * 111) % S;
        page_table.assign(pageNum, -1);
        load_time.assign(pageNum, -1);
        remainNum = N;
    }
    
    void nextReference(){
        double y = randDouble();
        if (y<A) {
            cur = (cur + 1) % S;
        }
        else if (y<A+B){
            cur = (cur - 5 + S) % S;
        }
        else if (y<A+B+C){
            cur = (cur + 4) % S;
        }
        else{
            int x;
            inFile >> x;
            cur = x % S;
        }
    }
};

vector<process> procs;
vector<frame> frameTable;

void lifo(int currProc, int currPage, int lastProc, int lastPage){
    //evict and change PTE
    procs[lastProc].page_table[lastPage] = -1;
    //change frame table
    frameTable[lastFrame].process_id = currProc;
    frameTable[lastFrame].page_id = currPage;
    //update process status
    procs[currProc].page_table[currPage] = lastFrame;
    procs[lastProc].evictCount++;
    procs[lastProc].recidencyTime += cycle - procs[lastProc].load_time[lastPage];
    procs[currProc].load_time[currPage] = cycle;
}

void random(int currProc, int currPage, int lastProc, int lastPage){
    //get random num and select random frame
    int randInt;inFile>>randInt;
    int evictFrame = randInt%frameNum;
    
    //updates
    procs[lastProc].page_table[lastPage] = -1;
    frameTable[evictFrame].process_id = currProc;
    frameTable[evictFrame].page_id = currPage;
    procs[currProc].page_table[currPage] = evictFrame;
    procs[lastProc].evictCount++;
    procs[lastProc].recidencyTime += cycle - procs[lastProc].load_time[lastPage];
    procs[currProc].load_time[currPage] = cycle;
}

void lru(int currProc, int currPage, int lastProc, int lastPage){
    //select lru frame
    int lruFrame=-1;
    int usedTime=INFINITY;
    FOR(i,0,frameNum){
        if(frameTable[i].lastUse!=-1){
            if(frameTable[i].lastUse<usedTime){
                lruFrame=(int)i;
                usedTime=frameTable[i].lastUse;
            }
        }
    }
    
    //updates
    procs[lastProc].page_table[lastPage] = -1;
    frameTable[lruFrame].process_id = currProc;
    frameTable[lruFrame].page_id = currPage;
    procs[currProc].page_table[currPage] = lruFrame;
    procs[lastProc].evictCount++;
    procs[lastProc].recidencyTime += cycle - procs[lastProc].load_time[lastPage];
    procs[currProc].load_time[currPage] = cycle;
}

void replacement(int currProc, int currPage, int lastProc, int lastPage){
    if(R == "lifo") {
        lifo(currProc, currPage, lastProc, lastPage);
    }
    else if(R == "random") {
        random(currProc, currPage, lastProc, lastPage);
    }
    else if(R == "lru") {
        lru(currProc, currPage, lastProc, lastPage);
    }
}

bool checkPage(int curPage,int proc_id){
    if(procs[proc_id].page_table[curPage]==-1) return true;
    else return false;
}

int checkFrame(){
    
    for(int i=frameNum-1;i>=0;i--){
        //cout<<frameTable[i].used<<endl;//
        if(frameTable[i].used==false) return (int)i;
    }
    return -1;
}

//start of main()
int main(int argc, const char * argv[]) {
    //Taking command line argument
    if(argc==7 or argc==8){
        M = atoi(argv[1]);
        P = atoi(argv[2]);
        S = atoi(argv[3]);
        J = atoi(argv[4]);
        N = atoi(argv[5]);
        R = argv[6];
    }
    else{
        cerr<<"illegal input, you should provide 6 command line arguments, and we do support debugging input"<<endl;
        exit(0);
    }
    
    //open input file
    inFile.open("random-numbers.txt",ios::in);
    
    //    DEHERE;//
    
    //setup input
    frameNum = M/P;
    pageNum = S/P;
    procs.clear();
    frameTable.assign(frameNum, frame());
    
    //checking whether index match!!!!
    if (J==1){
        procNum = 1;
        procs.emplace_back(1,0,0,1);
    }
    else if (J==2){
        procNum = 4;
        for (int i=1; i<=procNum; i++)
            procs.emplace_back(1,0,0,i);
    }
    else if (J==3){
        procNum = 4;
        for (int i=1; i<=procNum; i++)
            procs.emplace_back(0,0,0,i);
    }
    else{
        procNum = 4;
        procs.emplace_back(0.75,0.25,0,1);
        procs.emplace_back(0.75,0,0.25,2);
        procs.emplace_back(0.75,0.125,0.125,3);
        procs.emplace_back(0.5,0.125,0.125,4);
    }
    
    //    DEHERE;//
    
    //start of the cycles
    while(true){
        
        //check stopping conditions
        bool allDone=true;
        FOR(i,0,procNum){
            if(procs[i].finished==false) allDone=false;
        }
        if(allDone) break;
        
        //        DEHERE;//
        
        //start of execution
        FOR(i,0,procNum){
            if(!procs[i].finished){
                //round robin schduling with quantum 3
                FOR(ref,0,3){
                    
                    cycle++;
                    //simulate reference
                    int curPage=procs[i].cur / P;
                    bool pageFault=checkPage(curPage,(int)i);
                    
                    //                    DEHERE;//
                    //                    DE(pageFault);
                    
                    //if page fault occurs
                    if(pageFault){
                        
                        //                        DEIF;//
                        
                        procs[i].faultCount++;
                        int freeFrame=checkFrame();
                        
                        //                        DE(freeFrame);//
                        
                        //check whether need eviction
                        if(freeFrame!=-1){
                            
                            //                            DEIFIF;//
                            
                            //load page into frame table
                            frameTable[freeFrame].process_id = (int)i;
                            frameTable[freeFrame].page_id = curPage;
                            frameTable[freeFrame].lastUse = cycle;
                            frameTable[freeFrame].used = true;
                            procs[i].page_table[curPage] = freeFrame;
                            procs[i].load_time[curPage] = cycle;
                        }
                        else{
                            
                            //                            DEIFELSE;//
                            
                            //replace page
                            int lastProc = frameTable[lastFrame].process_id;
                            int lastPage = frameTable[lastFrame].page_id;
                            replacement((int)i, curPage,lastProc,lastPage);
                        }
                        //check
                        lastFrame = procs[i].page_table[curPage];
                        frameTable[lastFrame].lastUse=cycle;
                    }
                    else{
                        //read from the frame directly
                        lastFrame=procs[i].page_table[curPage];
                        frameTable[lastFrame].lastUse=cycle;
                    }
                    procs[i].remainNum--;
                    //calculate next reference
                    if(procs[i].remainNum==0){
                        procs[i].finished=true;
                        procs[i].nextReference();
                        break;
                    }
                    else procs[i].nextReference();
                }
            }
        }
    }
    
    //after execution, print output in the given format
    inFile.close();
    printf("The machine size is %d\n",M);
    printf("The page size is %d\n",P);
    printf("The process size is %d\n",S);
    printf("The job mix number is %d\n",J);
    printf("The number of references per process is %d\n",N);
    cout<<"The replacement algorithm is "<<R<<endl;
    cout<<"The level of debugging output is 0"<<endl<<endl;
    
    FOR(i,0,procNum){
        if(procs[i].evictCount==0){
            cout<<"Process "<<i+1<<" had "<<procs[i].faultCount<<" faults."<<endl;
            cout<<"\tWith no eviction, the average residence time is undefined."<<endl;
            totalFault+=procs[i].faultCount;
        }
        else{
            averRes=(double)procs[i].recidencyTime/(double)procs[i].evictCount;
            cout<<"Process "<<i+1<<" had "<<procs[i].faultCount<<" faults. and "<<averRes<<" average residency"<<endl;
            totalFault+=procs[i].faultCount;
            totalRes+=procs[i].recidencyTime;
            totalEvict+=procs[i].evictCount;
        }
    }
    cout<<endl;
    if(totalEvict==0) cout<<"The total number of page fault is "<<totalFault<<endl<<"\tWith no eviction, the average residence time is undefined."<<endl;
    else{
        averRes=totalRes/(double)totalEvict;
        printf("The total number of faults is %d and the overall average residency is %.5lf\n",totalFault,averRes);
    }
    
    //delete heap memory if required
    return 0;
}

