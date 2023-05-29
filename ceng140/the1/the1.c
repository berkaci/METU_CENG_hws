#include <stdio.h>
#define W_INDEX 0
#define H_INDEX 1
#define T_INDEX 2
#define P_INDEX 3
#define S_INDEX 4
#define A_INDEX 5
#define N_INDEX 6

int main() 
{
    long int duvar,tribsay,i,j,eb,tr[100][7];
    scanf("%ld %ld",&duvar,&tribsay);
    for(i=0;i<tribsay;i++)
    {
        for(j=0;j<7;j++) scanf("%ld",tr[i]+j);
    }
    for(i=0;i<=duvar;i++)
    {
        eb=1;
        for(j=0;j<tribsay;j++)
        {
            if(tr[j][4]>0&&(i>=tr[j][3])&&i<=tr[j][3]+tr[j][0]-1+(tr[j][4]*tr[j][5]*(tr[j][6]-1))&& (i-tr[j][3])%(tr[j][4]*tr[j][5])<tr[j][0] &&eb<tr[j][1]) eb=tr[j][1];
            if(tr[j][4]<0&&(i<tr[j][3]+tr[j][0])&&i>=tr[j][3]+(tr[j][4]*tr[j][5]*(tr[j][6]-1))&& (((-(tr[j][4]*tr[j][5])-((-i+tr[j][3]-1)%(-tr[j][4]*tr[j][5]))-1)<tr[j][0])||i>=tr[j][3]) && eb<tr[j][1]) eb=tr[j][1];
            if(tr[j][4]==0 && i>=tr[j][3] && i<tr[j][3]+tr[j][0] && eb<tr[j][1]) eb=tr[j][1];
        }
        printf("%ld",eb);
        if(i<duvar) printf(" ");
    }
    printf("\n");
    return 0;
}