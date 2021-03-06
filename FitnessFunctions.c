#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Main.h"
#include "MazeGeneration.h"
#include "Genetics.h"
#include "FitnessFunctions.h"


int Global_Path_Finnishable = 0;
int Global_Created_Check = 0;


struct Stack * initStack(struct Stack *FullStack){
    FullStack->Front = 0;
    FullStack->back = 0;
    FullStack->Length = 1;
    return FullStack;
}

void Push(struct Stack *FullStack,struct Node value){
    FullStack->back = FullStack->back + 1;
    FullStack->Length = FullStack->Length + 1;
    FullStack->stack[FullStack->back] = value;

}

struct Node Pop(struct Stack *FullStack){
    struct Node temp = FullStack->stack[FullStack->Front];
    FullStack->Front = FullStack->Front+1;
    //FullStack->back = FullStack->back-1;
    FullStack->Length = FullStack->Length-1;
    return(temp);
}

void Path(int **Maze,int *visited_idx){
    Global_Path_Finnishable = 0;

    struct Node start = {.x = 0,
                         .y = 0};

    struct Node goal = {.x = 19,
                        .y = 19};

    
    int vis_idx = 0;
    int visited[size][size] = {0};


    
    struct Stack *FullStack = malloc(sizeof(struct Stack )) ;
    

    initStack(FullStack);

    //add start to visited array
    
    visited[start.x][start.y] = 5; 
    // push start to stack
    Push(FullStack,start);

    while(FullStack->Length > 0){
        vis_idx=vis_idx+1;
        
        struct Node Current = Pop(FullStack);

        if(Current.x == goal.x && Current.y == goal.y){
            printf(" ");
            free(FullStack);
            Global_Path_Finnishable = 1;
            return ;
        }

        if (Current.x < size-1 && Current.y < size-1 && Current.y > -1 && Current.x > 0 && Maze[Current.x-1][Current.y] == 0 ){ 
            if (visited[Current.x-1][Current.y] == 0)
            {
               visited[Current.x-1][Current.y] = 3;
               struct Node temp = {Current.x-1,Current.y};
               Push(FullStack,temp);
            }
        }

        if (Current.x < size-1 && Current.y > -1 && Current.x > -1 && Maze[Current.x+1][Current.y] == 0 ){ 
            if (visited[Current.x+1][Current.y] == 0)
            {
               visited[Current.x+1][Current.y] = 1;
               struct Node temp = {Current.x+1,Current.y};
               Push(FullStack,temp);  
            }
            
        }

        if (Current.y < size-1 && Current.y > -1 && Current.x > -1 && Maze[Current.x][Current.y+1] == 0 ){ 
            if (visited[Current.x][Current.y+1] == 0)
            {
               visited[Current.x][Current.y+1] = 4;
               struct Node temp = {Current.x,Current.y+1};
               Push(FullStack,temp);
            }
            
        }

        if (Current.x < size-1 && Current.y < size-1 && Current.y > 0 && Current.x > -1 && Maze[Current.x][Current.y-1] == 0 ){ 
            if (visited[Current.x][Current.y-1] == 0)
            {
               visited[Current.x][Current.y-1] = 2;
               struct Node temp = {Current.x,Current.y-1};
               Push(FullStack,temp); 
            }
        }
    }
    
    Global_Created_Check++;
    *visited_idx = vis_idx;
    printf(" ");
    free(FullStack);
    return;
}


double Fitness(int **Maze){
    //fitness calculations
    int visited_idx = 0 ;
    Global_Path_Finnishable = 0;
    Path(Maze,&visited_idx);

    if(Global_Path_Finnishable == 0){
        return((double)(visited_idx)/(double)(size*size));
    }else{
        return 1;
    }
}



void sortFitnessArray(double Fits[],struct GeneArray *Gene){
    double temp;
    
    for (int i = 0; i < CrossoverLimit; ++i){
        for (int j = i + 1; j < CrossoverLimit; ++j){
            if (Fits[i] < Fits[j]){
                temp = Fits[i];
                Fits[i] = Fits[j];
                Fits[j] = temp;

            
                for(int z=0;z<walls;z++){
                    int tempint = Gene->Array[i][z].x;
                    int tempint2 = Gene->Array[i][z].y;
                    
                    Gene->Array[i][z].x =  Gene->Array[j][z].x;
                    Gene->Array[i][z].y =  Gene->Array[j][z].y;

                    Gene->Array[j][z].x = tempint;
                    Gene->Array[j][z].y = tempint2;
                }

            }
        }
    }
}

void InsertFitness(double BestFits[CrossoverLimit] , double NewFit[CrossoverLimit] ,struct GeneArray *OrigGenes,  struct GeneArray *cGenes){
    //Future work : Update to shift genes down and not just replace the gene.

    for(int i =0;i<CrossoverLimit;i++){
        for(int j = 0;j<CrossoverLimit;j++){
            if(NewFit[j]>=BestFits[i]){
            BestFits[i] = NewFit[j];
            OrigGenes->Array[i] = cGenes->Array[j];
            break;
            }
        }
    }
}
