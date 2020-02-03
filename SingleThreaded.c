# include <stdio.h>
# include <pthread.h>
# include <time.h>
int MAT1[2000][2000];
int MAT2[2000][2000];
int MAT3[2000][2000];

int initialLength=10;

void RandomFill();

int main()
{
    FILE *sizes;
    FILE *time;
    sizes = fopen("Sizes_single.txt" , "w+");
    time = fopen("TimeTaken_single.txt", "w+");

for(int ini=0; ini<100; ini++)
{

    int iCount,jCount,kCount;
	RandomFill();
for(iCount=0;iCount<initialLength;iCount++)
        {
            for(jCount=0;jCount<initialLength;jCount++)
            {
                MAT3[iCount][jCount]=0;
            }
        }

clock_t startTime1 = clock();
	for(iCount=0;iCount<initialLength;iCount++)
        {
            for(jCount=0;jCount<initialLength;jCount++)
            {
                for(kCount=0;kCount<initialLength;kCount++)
                {
                    MAT3[iCount][jCount]+=MAT1[iCount][kCount] * MAT2[kCount][jCount];
                }
            }
        }
        
	clock_t termination1 = clock();
    	float executionTime1 = (termination1 - startTime1) * 1000 / CLOCKS_PER_SEC;
	fprintf(sizes, "%d", initialLength);
        fprintf(sizes, "\n");
        fprintf(time, "%f", executionTime1);
        fprintf(time, "\n");
	initialLength+=10;
}
return 0;

}

void RandomFill()
{
	for (int i = 0; i < initialLength; i++)
	{
	        for (int j = 0; j < initialLength; j++)
        	{
            		MAT1[i][j] = rand() % 100 + 1;
        	}
	}

	for (int i = 0; i < initialLength; i++)
	{
	        for (int j = 0; j < initialLength; j++)
        	{
            		MAT2[i][j] = rand() % 100 + 1;
        	}
	}
}
