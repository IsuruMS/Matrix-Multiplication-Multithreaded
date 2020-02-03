# include <stdio.h>
# include <pthread.h>
# include <time.h>
int MAT1[1000][1000];
int MAT2[1000][1000];
int MAT3[1000][1000];

int r1,c1,r2,c2;

void *MultiplicationAlgorithm(int n);
void RandomFill();
void FileFill();

int main()
{
    FILE *sizes;
    FILE *time;
    sizes = fopen("Matrix Sizes_Algo2.txt" , "w+");
    time = fopen("Execution Time_Algo2.txt", "w+");
    int temp=10;
for(int ini=0; ini<100; ini++)
{
    pthread_t tid1, tid2, tid3, tid4;    
    int iCount,jCount,kCount;
    

    r1=c1=r2=c2=temp;
    
	RandomFill();
        for(iCount=0;iCount<r1;iCount=iCount+2)
        {
            for(jCount=0;jCount<c2;jCount=jCount+2)
            {
                MAT3[iCount][jCount]=0;
            }
        }
        int lim1 = 0;
        int lim2 = lim1 + r1/5;
        int lim3 = lim2 + r1/5;
        int lim4 = lim3 + r1/5;
        int lim5 = lim4 + r1/5;
        clock_t startTime = clock();
        
        for(iCount=lim1;iCount<r1/5;iCount++)
        {
            for(jCount=0;jCount<c2;jCount++)
            {
                for(kCount=0;kCount<c1;kCount++)
                {
                    MAT3[iCount][jCount]+=MAT1[iCount][kCount] * MAT2[kCount][jCount];
                }
            }
        }
        pthread_create(&tid1,NULL,MultiplicationAlgorithm,lim2);
        pthread_create(&tid2,NULL,MultiplicationAlgorithm,lim3);
        pthread_create(&tid3,NULL,MultiplicationAlgorithm,lim4);
        pthread_create(&tid4,NULL,MultiplicationAlgorithm,lim5);        

        pthread_join(tid1,NULL);
        pthread_join(tid2,NULL);
        pthread_join(tid3,NULL);
        pthread_join(tid4,NULL);

	clock_t termination = clock();
    	float executionTime = (termination - startTime) * 1000 / CLOCKS_PER_SEC;
	//printf("\n Time Taken =  %f",executionTime);
	    fprintf(sizes, "%d", temp);
        fprintf(sizes, "\n");
        fprintf(time, "%f", executionTime);
        fprintf(time, "\n");
	temp=temp+10;
    	
    }
    return 0;
}

void *MultiplicationAlgorithm(int n)
{
    int iCount,jCount,kCount;
    for(iCount=n;iCount<(r1/5 )+n;iCount++)
        {
            for(jCount=0;jCount<c2;jCount++)
            {
                for(kCount=0;kCount<c1;kCount++)
                {                    
                    MAT3[iCount][jCount]+=MAT1[iCount][kCount] * MAT2[kCount][jCount];
                }
            }
        }
        pthread_exit(NULL);
}

void RandomFill()
{
	for (int i = 0; i < r1; i++)
	{
	        for (int j = 0; j < c1; j++)
        	{
            		MAT1[i][j] = rand() % 100 + 1;
        	}
	}

	for (int i = 0; i < r2; i++)
	{
	        for (int j = 0; j < c2; j++)
        	{
            		MAT2[i][j] = rand() % 100 + 1;
        	}
	}
}

void FileFill()
{
    FILE *fp = fopen("matA.txt", "r");
    const char s[2] = ", ";
    char *token;
    int i;
    if(fp != NULL)
    {
        char line[20];
        while(fgets(line, sizeof line, fp) != NULL)
        {
            token = strtok(line, s);
            for(i=0;i<2;i++)
            {
                if(i==0)
                {   
                    printf("%s\t",token);
                    token = strtok(NULL,s);
                } else {
                    printf("%d\n",atoi(token));
                }       
            }
        }
        fclose(fp);
    } else {
        perror("user.dat");
    }   
}
