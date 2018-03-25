#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include<stdbool.h>
/* Control Parameters of CSA */
#define N 20        //Flock Size
#define AP 0.1      //Awareness Probability
#define FL 2        //Flight Length
#define runtime 1   //Algorithm runtime

/* Problem Parameters */
#define ub 100      //Upper Bound
#define lb -100     //Lower Bound
#define PD 2       //Problem Dimension

double Crows[N][PD];
double Objectives[N];

double xNew[N][PD];

double ObjectivesMemory[N];
double SolutionMemory[N][PD];

double GlobalMin;
double GlobalSolution[PD];

//Objective Function
double sphere(double solution[PD])
{
    int p;
    double result=0;
    for(p=0;p<PD;p++)
    {
        result+=solution[p]*solution[p];
    }
    return result;
}


//Create initial random solutions.
void initial()
{
    int i,j;
    double r;
    for(i=0;i<N;i++)
    {
        for(j=0;j<PD;j++)
        {
            r = (  (double)rand() / ((double)(RAND_MAX)+(double)(1)) );      //Create Random Number Between 0,1;
            Crows[i][j]=r*(ub-lb)+lb;
        }
        Objectives[i]=sphere(Crows[i]);
    }
}
//Memorize Initial Solutions
void MemorizeAllSolutions()
{
    int i,j;
    for(i=0;i<N;i++)
    {
        ObjectivesMemory[i]=Objectives[i];
        for(j=0;j<PD;j++)
            SolutionMemory[i][j]=Crows[i][j];
    }
    GlobalMin=ObjectivesMemory[0];
    for(i=0;i<PD;i++)
        GlobalSolution[i]=SolutionMemory[0][i];
}
//Memorize Best Solutions
void MemorizeBestSolutions()
{
    int i,p;
    for(i=0;i<N;i++)
    {
        if (Objectives[i]<ObjectivesMemory[i])
        {
            ObjectivesMemory[i]=Objectives[i];
            for(p=0;p<PD;p++)
                SolutionMemory[i][p]=Crows[i][p];

            if(ObjectivesMemory[i]<GlobalMin)
            {
                GlobalMin=ObjectivesMemory[i];
                for(p=0;p<PD;p++)
                    GlobalSolution[p]=SolutionMemory[i][p];
            }

        }
    }
}

int main()
{
    srand(time(NULL));
    int i,p;
    double r;
    int random_crows[N];
    //Create Initial Population.
    initial();
    //Memorize
    MemorizeAllSolutions();
    printf("iteration=1 objective best=%f \n",GlobalMin);

    int t,iter_max=1000;    //Maximum iteration for each runtime
    for(t=0;t<iter_max;t++)
    {
        //Generate random candidate crows for following
        for(i=0;i<N;i++)
            random_crows[i] = rand()%N;

        for(i=0;i<N;i++)
        {
            r = ( (double)rand() / ((double)(RAND_MAX)+(double)(1)) );
            if(r>AP)
            {
                //Update From Memory
              for(p=0;p<PD;p++)
                 xNew[i][p] = Crows[i][p]+FL*r*(SolutionMemory[random_crows[i]][p]-Crows[i][p]);
            }
            else{
                for(p=0;p<PD;p++)
                {
                    //Generate Random Solution
                    r = (  (double)rand() / ((double)(RAND_MAX)+(double)(1)) );      //Create Random Number Between 0,1;
                    xNew[i][p]=r*(ub-lb)+ub;
                }
            }
        }
        for(i=0;i<N;i++)
        {
            bool valid=true;
            for(p=0;p<PD;p++)
                if (xNew[i][p]<lb || xNew[i][p]>ub)
                    valid=false;
            if(valid)
            {
                for(p=0;p<PD;p++)
                    Crows[i][p] =xNew[i][p];
                Objectives[i]=sphere(Crows[i]);

            }
        }
        MemorizeBestSolutions();
        if(t%100==0)
            printf("iteration=%d objective best=%e \n",t,GlobalMin);

    }

    return 0;
}
