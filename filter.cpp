#include <iostream>
#include <cstdlib> 
#include <time.h> 
#include <cmath>
#include <map>
#include <vector>
#include <chrono>
#include <fstream>
#include <thread>
#include <algorithm>


using namespace std;

long long Nbig = pow(2,31)-1;
long long p = pow(2,31)-1;
long long m = 10000;
long long n = 3000;
int k = 1;


struct hashPair{
    long long  a;
    long long  b;
};


int* map1 = new int[m];
int* map2 = new int[m];

vector<long long*>* hashFunc1;
vector<hashPair*>* hashFunc2;


void  makeHash1(){ // returns array of longs, where h[i] is the seed si, where rand(si+x) is where x is hashed, x IS NOT hashed at si.
    int nano =  std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    srand(nano);
    for(int i=0; i < k; i++){
        (hashFunc1->at(i)) = new long long (rand()%Nbig);
    }
}

long long hash1(int i,int x){
    srand(x + *(hashFunc1->at(i)));
    return rand() % m;
}

void clearHash1(){
    for(int i=0; i < m;i++){
        map1[i] = 0;
    }
     for(int i =0; i <k ; i++){
        delete hashFunc1->at(i);
    }
}

int findHash1(int x, int i){
    srand(x+*(hashFunc1->at(i)));
    return map1[rand()%m];
}

void bloomAdd1(int x){
    for(int i =0; i < k; i++){
        map1[hash1(i,x)] = 1;
    }
}

bool bloomContains1(int x){
    for(int i =0; i < k; i++){
        if (map1[hash1(i,x)] == 0){
            return false;
        }
    }
    return true;
}



void  makeHash2(){ //returns array of n pairs a,b where a is multiplied by x, and then b is added. Two mods are then peformed on the sum.
    
    
    int nano =  std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    srand(nano);
    
    for(int i=0; i < k; i++){
        hashPair * curr = new hashPair;
        hashFunc2->at(i) = curr;
        curr->a = (rand()%(p-2))+1;
        curr->b = (rand()%(p-1));
        }
    
}
void clearHash2(){

    for(int i=0; i < m;i++){
        map2[i] = 0;
    }
    for(int i =0; i <k ; i++){
        delete hashFunc2->at(i);
    }
}

long long hash2(int i,int x){
    return ((((hashFunc2->at(i)->a*x) + hashFunc2->at(i)->b)%p)%m);
}

int findHash2(int x, int i){
    
    return map2[((hashFunc2->at(i)->a*x + hashFunc2->at(i)->b)%p)%m];
}

void bloomAdd2(int x){

    for(int i =0; i < k; i++){
        map2[hash2(i,x)] = 1;
    }
}

bool bloomContains2(int x){

    for(int i =0; i < k; i++){
        if (map2[hash2(i,x)] == 0){
            return false;
        }
    }
    return true;
}

void collisionCounting1(){ //used for counting collisions and finding max loads
   
   ofstream myfile;
   myfile.open ("data.txt");
   for(int b = 1; b <=2500 ; b+=50){

        n = b;
        hashFunc1 = new vector<long long*>(k);
        int collisions1 =0;
        int avrLoad1 = 0;
        for(int y = 0; y <50; y++){
        makeHash1();
        int collisions = 0;

        map<int,int> maxLoad;
        int maxld = 0;
        map<int,int> inserted;
            for(int i = 1; i < n+1;i++){

            int nano =  std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            srand(nano);

            int k1 = rand() % k;
            int insert = rand()% Nbig;
            while(inserted[insert] == 1){
                insert = rand()%Nbig;
            }
            inserted[insert] = 1;
            long long idx = hash1(k1,i);

            if(map1[idx] != 0){
                maxLoad[idx] += 1;
                if(maxLoad[idx] > maxld){
                maxld = maxLoad[idx];
                }
                collisions++;
            }else{
                map1[idx] = 1;
                maxLoad[idx] = 0;
            }
            }
        //cout<<maxld<<endl;
        //cout<<collisions<<endl;

        collisions1+=collisions;
        avrLoad1+=maxld;
        clearHash1();
        }
        myfile <<static_cast<float>(collisions1)/50<<", ";
        cout<<"AVERAGE COLLISIONS: "<< static_cast<float>(collisions1)/50<<endl;
        //myfile<<static_cast<float>(avrLoad1)/50<<", ";
        //cout<<static_cast<float>(avrLoad1)/50<<endl;
        delete hashFunc1;
        }

        myfile <<"\n";
        myfile.close();
}
void collisionCounting2(){ //used for counting collisions and finding max loads
   ofstream myfile;
   myfile.open ("data.txt");

   for(int b = 1; b <= 2500; b+=50){
    n = b;
   hashFunc2 = new vector<hashPair*>(k);
   int totalcollisions =0;
   int avrLoad = 0;
   

   for(int y =0; y <50; y++){
   map<int,int> maxLoad2;
   int maxld2 = 0;
   makeHash2();
   int collisions2 = 0;
   map<int,int> inserted;
   
   for(int i = 1; i < n+1; i++){

        int nano =  std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        srand(nano);
        
        //int x = rand();
        int k1 = rand() % k;
        int insert = rand()% Nbig;
            while(inserted[insert] == 1){
                insert = rand()%Nbig;
            }
        inserted[i] = 1;

        int idx2 = hash2(k1, i);
        
        if(map2[idx2] != 0 ){
            maxLoad2[idx2] += 1;
            if(maxLoad2[idx2] > maxld2){
                maxld2 = maxLoad2[idx2];
            }
            collisions2++;
            
        }else{
            map2[idx2] = i;
            maxLoad2[idx2] = 0;
        }
   }
   //cout<<maxld2<<endl;
   //cout<<collisions2<<endl;
   totalcollisions+=collisions2;
   avrLoad+=maxld2;
   clearHash2();
   }

   myfile <<static_cast<float>(totalcollisions)/50<<", ";
   cout<<"AVERAGE COLLISIONS: "<< static_cast<float>(totalcollisions)/(50)<<endl;
   //cout<<"AVERAGE MAX LOAD: "<< static_cast<float>(avrLoad)/50<<endl;
   //myfile<< static_cast<float>(avrLoad)/50<<", ";


   }
   myfile.close();
}

void falseTest2(){//used for finding false positives

   ofstream myfile;
   myfile.open ("data.txt");
   
   int nano =  std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
   srand(nano);
   for(int h =1; h <= 25; h+=1){

        k = h;
        int avrFalsePos = 0;
        hashFunc2 = new vector<hashPair*>(k);
        vector<float> fpVec = vector<float>(50);
        for(int z =0; z< 50;z++){
            map<int, int> added;
            int falsepos = 0;
            makeHash2();

             for(int i = 0; i <= n; i++){
                int add = rand()%Nbig;
                while(added[add] == 1){
                    add = rand()%Nbig;
                }
                bloomAdd2(i);
                added[add] = 1;
            }
    
            for(int i = 1; i  <= n; i++){

                int addtry = rand()%Nbig;
                while(added[addtry] == 1){
                    addtry = rand()%Nbig;
                }

                if(bloomContains2(i+n) == true){
                    falsepos++;
                }
            }
            avrFalsePos+=falsepos;
            float fpRate = static_cast<float>(falsepos)/(n);
            fpVec.at(z) = fpRate;

            
    
            clearHash2();
        }
        sort(fpVec.begin(),fpVec.end());

        

        cout<<"MEDIAN FALSE POSITIVE RATE FOR K = "<<k << ": "<<fpVec[25]<<endl;
        myfile <<fpVec[25]<<", ";

        delete hashFunc2;
   }
   myfile << "\n";
   myfile.close();
   
} 
void falseTest1(){ //used for finding false positives

   ofstream myfile;
   myfile.open ("data.txt");
   
   int nano =  std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    srand(nano);
   for(int h =1; h <= 25; h++){


        k = h;
        int avrFalsePos = 0;
        hashFunc1 = new vector<long long*>(k);
        vector<float> fpVec = vector<float>(50);

        for(int z =0; z< 50;z++){
            map<int, int> added;
            int falsepos = 0;
            makeHash1();
    
            for(int i = 0; i <= n; i++){
                int add = rand()%Nbig;
                while(added[add] == 1){
                    add = rand()%Nbig;
                }
                bloomAdd1(i);

                added[add] = 1;
            }
    
            for(int i = 1; i  <= n; i++){

                int addtry = rand()%Nbig;
                while(added[addtry] == 1){
                    addtry = rand()%Nbig;
                }

                if(bloomContains1(i+n) == true){
                    falsepos++;
                }
            }

            avrFalsePos+=falsepos;
            float fpRate = static_cast<float>(falsepos)/(n);
            fpVec.at(z) = fpRate;
            
            clearHash1();
        }
        
        sort(fpVec.begin(),fpVec.end());
        cout<<"MEDIAN FALSE POSITIVE RATE FOR K = "<<k << ": "<<fpVec[25]<<endl;
        myfile <<fpVec[25]<<", ";
        delete hashFunc1;
   } 
   myfile << "\n";
   myfile.close();
   
} 

void mapScatter1(){//used for making scatter plots

    map<int,int> inserted;
    vector<int> input = vector<int>(n);
    vector<int> output = vector<int>(n);
    hashFunc1 = new vector<long long*>(k);
    makeHash1();
    for(int i = 0; i < n; i++){

        int nano =  std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        srand(nano);
        
        int k1 = rand() % k;
        int insert = rand()% Nbig;
            while(inserted[insert] == 1){
                insert = rand()%Nbig;
            }
        inserted[insert] = 1;

        int idx1 = hash1(k1, i);
        input.at(i) = i;
        output.at(i) = idx1;

   }

        ofstream myfile;
        myfile.open("data.txt");
        for(int j = 0 ; j < n; j++){
            myfile << input.at(j) << ", ";
        }
        myfile<<"\n";
        for(int j = 0 ; j < n; j++){
            myfile << output.at(j) << ", ";
        }
        myfile.close();



}

void mapScatter2(){//used for making scatter plots

    map<int,int> inserted;
    vector<int> input = vector<int>(n);
    vector<int> output = vector<int>(n);
    hashFunc2 = new vector<hashPair*>(k);
    makeHash2();
    for(int i = 0; i < n; i++){

        int nano =  std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        srand(nano);
        
        int k1 = rand() % k;
        int insert = rand()% Nbig;
            while(inserted[insert] == 1){
                insert = rand()%Nbig;
            }
        inserted[insert] = 1;

        int idx1 = hash2(k1, insert);
        input.at(i) = insert;
        output.at(i) = idx1;

   }

        ofstream myfile;
        myfile.open("data.txt");
        for(int j = 0 ; j < n; j++){
            myfile << input.at(j) << ", ";
        }
        myfile<<"\n";
        for(int j = 0 ; j < n; j++){
            myfile << output.at(j) << ", ";
        }
        myfile.close();



}



void falseTest2Fixk(){//used to make plots with a fixed k, and changing c, did not use in report

   ofstream myfile;
   myfile.open ("data.txt");
   
   int nano =  std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
   srand(nano);
   for(int h =1; h <= 6000; h+=50){

        n = h;
        int avrFalsePos = 0;
        hashFunc2 = new vector<hashPair*>(k);
        vector<float> fpVec = vector<float>(50);
        for(int z =0; z< 50;z++){
            map<int, int> added;
            int falsepos = 0;
            makeHash2();

             for(int i = 0; i <= n; i++){
                int add = rand()%Nbig;
                while(added[add] == 1){
                    add = rand()%Nbig;
                }
                bloomAdd2(add);
                added[add] = 1;
            }
    
            for(int i = 0; i  <= n; i++){

                int addtry = rand()%Nbig;
                while(added[addtry] == 1){
                    addtry = rand()%Nbig;
                }

                if(bloomContains2(addtry) == true){
                    falsepos++;
                }
            }
            avrFalsePos+=falsepos;
            float fpRate = static_cast<float>(falsepos)/(n);
            fpVec.at(z) = fpRate;

            
    
            clearHash2();
        }
        sort(fpVec.begin(),fpVec.end());


        cout<<"MEDIAN FALSE POSITIVE RATE FOR N = "<<n << ": "<<fpVec[25]<<endl;
        myfile <<fpVec[25]<<", ";

        delete hashFunc2;
   }
   myfile << "\n";
   myfile.close();
   
} 
void falseTest1Fixk(){//used to make plots with a fixed k, and changing c, did not use in report

   ofstream myfile;
   myfile.open ("data.txt");
   
   int nano =  std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    srand(nano);
   for(int h =1; h <= 6000; h+=50){


        n = h;
        int avrFalsePos = 0;
        hashFunc1 = new vector<long long*>(k);
        vector<float> fpVec = vector<float>(50);

        for(int z =0; z< 50;z++){
            map<int, int> added;
            int falsepos = 0;
            makeHash1();
    
            for(int i = 0; i <= n; i++){
                int add = rand()%Nbig;
                while(added[add] == 1){
                    add = rand()%Nbig;
                }
                bloomAdd1(add);

                added[add] = 1;
            }
    
            for(int i = 0; i  <= n; i++){

                int addtry = rand()%Nbig;
                while(added[addtry] == 1){
                    addtry = rand()%Nbig;
                }

                if(bloomContains1(addtry) == true){
                    falsepos++;
                }
            }

            avrFalsePos+=falsepos;
            float fpRate = static_cast<float>(falsepos)/(n);
            fpVec.at(z) = fpRate;
            
            clearHash1();
        }
        
        sort(fpVec.begin(),fpVec.end());
        cout<<"MEDIAN FALSE POSITIVE RATE FOR N = "<<n << ": "<<fpVec[25]<<endl;

        myfile <<fpVec[25]<<", ";
        delete hashFunc1;
   } 
   myfile << "\n";
   myfile.close();
   
} 
int main(){//call functions from main and write to text file to collect data.
    collisionCounting1();
    collisionCounting2();
    //falseTest1();
    //mapScatter2();

}