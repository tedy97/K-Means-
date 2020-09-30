/* Thodoris Apostolopoulos  */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include <omp.h>    
// *************************************************************************************************
//
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
    bool Identity[N] = {false};             // Boolean array avoids getting same centers
    for(i=0;i<Nc;i++){
        random = rand() % N ;               // Random choose of center from range [0,N+1]
        while(Identity[random]){            // While the center is already chosen, tries to find new center
            random = rand() % N ;          
        }
        for(j=0;j<Nv;j++){
            Center[i][j] = Vec[random][j] ;
            Identity[random] = true ;       // Center chosen becomes true
        }
    }
}
// ***********************************************************************************
// Returns the total minimum distance 
// ***********************************************************************************
// --estimateClass-- function takes over ~98% of time --> Will be parallelized
// ***********************************************************************************
float estimateClass(void){

    float total_dist = 0 ;
    float distance[Nc] = {0} ;
    #pragma omp parallel for schedule(static,500) reduction(+:total_dist)
    for(int i=0;i<N;i++){                       
        float min_distance = 1.0e30 ;
        int cluster = 0 ; 
        for(int k=0;k<Nc;k++){
            distance[k] = 0 ;                  
            float d = 0 ;                      
            #pragma omp simd reduction(+:d)
            for(int j=0;j<Nv;j++){              
                d += sq(Vec[i][j] - Center[k][j]) ;     
            }
            distance[k] = sqrt(d) ;      
            if(distance[k] < min_distance){     
                min_distance = distance[k] ;    
                cluster = k  ;                  
            }                                   
        }
        total_dist += min_distance ;            
        Class[i] = cluster ;                  
   }
   return total_dist ;
}
// ***************************************************
// New centers calculation
// ***************************************************
void estimateCenter(void){
     
    #pragma omp parallel for                    
    for(int k=0;k<Nc;k++){
        float dist[Nv] = {0} ;                          
        int counter = 0 ; 
        for(int i=0;i<N;i++){
            if(Class[i] == k){                       
                counter++ ;                             
                #pragma omp simd
                for(int j=0;j<Nv;j++){ 
                    dist[j] += Vec[i][j] ;
                }    
            }
        }   
        #pragma omp simd
        for(int n=0;n<Nv;n++){
            Center[k][n] = dist[n] / (counter*1.0) ;    
        }
    }
}
// *********************************************************
// Initialization of N random vectors : Vec[N][Nv]
// *********************************************************
void SetVec(void){
    //int i, j ; 
    #pragma omp simd
    for(int i=0;i<N;i++){
        for(int j=0;j<Nv;j++){
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

    int count = 0 ;         // Loop counter
    do{  
        count++ ;   printf("Wait...\n")   ;    
        prevDist = totDist ;
        totDist = estimateClass() ; printf("totDist = %12f \n\n",totDist) ; 
        estimateCenter() ; 
    } while(fabs(prevDist-totDist)/totDist > THRESHOLD) ;

    printf("\nTimes Looped : %d\n",count) ; 
    return 0 ;
}