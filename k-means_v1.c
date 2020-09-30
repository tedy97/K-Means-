/* Thodoris Apostolopoulos */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
// *************************************************************************************************
// Vector Processing using GCC compiler 
// Vector Processing
// *************************************************************************************************
#pragma GCC optimize("O3","unroll-loops","omit-frame-pointer","inline", "unsafe-math-optimizations")
#pragma GCC option("arch=native","tune=native","no-zero-upper")
// *************************************************************************************************
#define N 100000
#define Nv 1000
#define Nc 100
#define THRESHOLD 0.000001
#define sq(x) ((x)*(x))     
// ***************************************************
float Vec[N][Nv] ;
float Center[Nc][Nv] ;
int Class[N] ;
// ******************************************************
// Random initialization of center array : Center[Nc][Nv]
// ******************************************************
void initCenter(void){

    int i, j, random ;
    bool Identity[N] = {false} ;                // Boolean array avoids getting same centers
    for(i=0;i<Nc;i++){
        random = rand() % N ;                   // Random choose of center from range [0,N+1]
        while(Identity[random]){                // While the center is already chosen, tries to find new center
            random = rand() % N ;          
        }
        for(j=0;j<Nv;j++){
            Center[i][j] = Vec[random][j] ;
            Identity[random] = true ;           // center chosen becomes true
        }
    }
}
// ***************************************************
// Returns the total minimum distance 
// ***************************************************
float estimateClass(void){

    int i, j, k, cluster ;
    float min_distance, total_dist = 0.0 ;
    float distance[Nc] = {0} ;                  // initial the array distance to save the distances of each vector from the centers
    for(i=0;i<N;i++){                           // running the lines of Vec[N][Nv]
        min_distance = 1.0e30, cluster = 0 ; 
        for(k=0;k<Nc;k++){                      // running the lines of Center[Nc][Nv]
            distance[k] = 0 ;                   // calculating the distance of each Center : k = [0,Nc]
            for(j=0;j<Nv;j++){                  // running the columns of the arrays 
                distance[k] += sq(Vec[i][j] - Center[k][j]) ;     
            }
            distance[k] = sqrt(distance[k]) ;   // Euclidian distance of each Vector from the Centers
            if(distance[k] < min_distance){     // Chooses the minimum distance of Vector from each Center
                min_distance = distance[k] ;    
                cluster = k  ;                  // The minimum distance defines the cluster of the Vector
            }                                   // Clusters in range of [0,Nc-1] --> Nc clusters
        }
        total_dist += min_distance ;            // Total minimum distance of all Vectors
        Class[i] = cluster ;                    // Class[N] gives the cluster of each Vec[N][Nv] 
   }
   return total_dist ;
}
// ***************************************************
// New centers calculation
// ***************************************************
void estimateCenter(void){
    
    int i, j, k, counter, n ;                           
    for(k=0;k<Nc;k++){
        float dist[Nv] = {0} ;                          // dist[Nv] array shows the sum of all vectors in the same cluster
        counter = 0 ; 
        for(i=0;i<N;i++){
            if(Class[i] == k){                          // When Vector is same cluster 
                counter++ ;                             // counter will count how many vectors are in the same cluster
                for(j=0;j<Nv;j++){ 
                    dist[j] += Vec[i][j] ;
                }    
            }
        }   
        for(n=0;n<Nv;n++){
            Center[k][n] = dist[n] / (counter*1.0) ;    // New center is the mean of all vectors belong to the same cluster
        }
    }
}
// *********************************************************
// Initialization of N random vectors : Vec[N][Nv]
// *********************************************************
void SetVec(void){
    int i, j ; 
    for(i=0;i<N;i++){
        for(j=0;j<Nv;j++){
            Vec[i][j] = (1.0*rand()) / RAND_MAX ;
        }
    }
}
// ***************************************************
// Print vectors : Vec[N][Nv]
// ***************************************************
void printVec(void){
    int i, j ;
    for(i=0;i<N;i++){
        for(j=0;j<Nv;j++){
        } 
    }
}
// ***************************************************
// Main 
// ***************************************************
int main(void){

    float totDist ;
    float prevDist ;
    SetVec() ;

    initCenter() ;
    totDist = 1.0e30 ;

    int count = 0 ;                         // Loop counter
    for(int i=0; i<16; i++){                  // 16 loops
    //do{
        count++ ;  printf("Wait...\n")   ;    
        prevDist = totDist ;
        totDist = estimateClass() ; printf("totDist = %12f \n\n",totDist) ; 
        estimateCenter() ; 
    } //while((prevDist-totDist)/totDist > THRESHOLD) ;
    printf("\nTimes Looped : %d\n",count) ; 
    return 0 ;
}