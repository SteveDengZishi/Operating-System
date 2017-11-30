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
int M,P,S,J,N,machineSize,pageSize,procSize,mixNum,refNum,procNum,totalFault;
double averRes;
string R;
fstream inFile;


//struct to store the final result when task terminates, need to initialize to 0s to prevent garbage vaule
struct result{
    int first=0,second=0,third=0;
    
    result(){}
    result(int a, int b, double c):first(a),second(b),third(c){}
};

double randDouble(){
    int randint;
    inFile>>randint;
    double result = (double) randint / (double)RAND_MAX+1;
    return result;
}

//start of main()
int main(int argc, const char * argv[]) {
    //Taking command line argument
    if(argc==7){
        M = atoi(argv[1]);
        P = atoi(argv[2]);
        S = atoi(argv[3]);
        J = atoi(argv[4]);
        N = atoi(argv[5]);
        R = argv[6];
    }
    else{
        cerr<<"illegal input, you should provide 6 command line arguments"<<endl;
        exit(0);
    }
    
    //open input file
    inFile.open("random-numbers.txt",ios::in);
    
    //start of the cycles
    while(true){
        
    }
    
    //after execution, print output in the given format
    printf("The machine size is %d.\n",machineSize);
    printf("The page size is %d.\n",pageSize);
    printf("The process size is %d\n.",procSize);
    printf("The job mix number is %d.\n",mixNum);
    printf("The number of references per process is %d.\n",refNum);
    cout<<"The replacement algorithm is "<<R<<endl;
    cout<<"The level of debugging output is 0"<<endl<<endl;
    
    FOR(i,0,procNum){
        
    }
    cout<<endl;
    printf("The total number of faults is %d and the overall average residency is %f",totalFault,averRes);
    
    //delete heap memory
   
    
    return 0;
}