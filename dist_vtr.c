#include <stdio.h>



struct routers
{
    unsigned distance[20];
    unsigned from[20];
} RT[20];


int main(){

    int costMatrix[20][20], n;
    int i, j,k;
    printf("Number of nodes: ");
    scanf("%d", &n);
    

   

    printf("\nEnter cost matrix\n");
    for (i = 0; i < n; ++i)
    {
        for (j = 0; j < n; ++j)
        {
            scanf("%d", &costMatrix[i][j]);
          
            costMatrix[i][i] = 0;
            RT[i].distance[j] = costMatrix[i][j]; 
            RT[i].from[j] = j;
        }
    }



    
    for (i = 0; i < n; ++i)
    {
        for (j = 0; j < n; ++j)
        {
            for (k = 0; k < n; ++k)
            {   
                
                if (RT[i].distance[j] > costMatrix[i][k] + RT[k].distance[j])
                {

                    RT[i].distance[j] = RT[i].distance[k] + RT[k].distance[j];

                    RT[i].from[j] = k;
                }
            }
        }
    }



   
    for (i = 0; i < n; ++i)
    {
        printf("\nRouter %d\n", i + 1);
        for (j = 0; j < n; ++j)
        {
            printf("\t\n nodes %d via %d : Distance %d\n", j + 1, RT[i].from[j] + 1, RT[i].distance[j]);
        }
        printf("\n");
    }




   
    return 0;
}