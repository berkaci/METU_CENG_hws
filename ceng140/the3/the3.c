#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "the3.h"

#define MAX_SIZE 10

CountNode *bas=NULL;
QueryNode *(tablo[10]);

void yaz()
{
    CountNode *cnp;
    QueryNode *qnp;
    for(cnp=bas;cnp!=NULL;cnp=cnp->next)
    {
        printf("%d ", cnp->count);
        for(qnp=*(cnp->querylist);qnp->next!=NULL;qnp=qnp->next)
        {
            printf("%s ", qnp->query);
        }
        printf("%s", qnp->query);
        printf("\n");
    }
}

QueryNode* qnekle(CountNode *count, QueryNode* yeni)
{
    QueryNode *p=*(count->querylist);
    if(p==NULL)
    {
        yeni->prev=NULL;
        yeni->next=NULL;
        *(count->querylist)=yeni;
        return yeni;
    }
    for(;p->next!=NULL && strcmp((p->next)->query,yeni->query)<=0;)
    {
        p=p->next;
    }
    if(p==*(count->querylist) && strcmp(yeni->query, p->query)<0)
    {
     	*(count->querylist)=yeni;
     	yeni->prev=NULL;
     	yeni->next=p;
     	p->prev=yeni;
    }
    else
    {
		yeni->prev=p;
		yeni->next=p->next;
		p->next=yeni;
		if(yeni->next!=NULL)
		    (yeni->next)->prev=yeni;
	}
    return yeni;
    
}


CountNode* cn(int ayi)
{
    CountNode *p=bas, *yeni;
    if(p==NULL)
    {
        p=malloc(sizeof(CountNode));
        bas=p;
        p->prev=NULL;
        p->next=NULL;
        p->count=ayi;
        p->querylist=malloc(sizeof(QueryNode*));
        *(p->querylist)=NULL;
        return p;
    }
    for(;p->next!=NULL && (p->next)->count<=ayi;)
    {
        if((p->next)->count==ayi) return p->next;
        p=p->next;
    }
    if(p->count==ayi) return p;
    if(p==bas && p->count>ayi)
    {
        bas=malloc(sizeof(CountNode));
        yeni=bas;
        yeni->count=ayi;
        yeni->prev=NULL;
        yeni->next=p;
        yeni->querylist=malloc(sizeof(QueryNode*));
        *(yeni->querylist)=NULL;
        p->prev=yeni;

    }
    else
    {
        yeni=malloc(sizeof(CountNode));
        yeni->count=ayi;
        yeni->prev=p;
        yeni->next=p->next;
        p->next=yeni;
        yeni->querylist=malloc(sizeof(QueryNode*));
        *(yeni->querylist)=NULL;
        if(yeni->next!=NULL) (yeni->next)->prev=yeni;
    }
    return yeni;
    
}

int main() 
{
    int sayi,i,ii,iii, ayi, aramasay, dolu, cns;
    char *str;
    CountNode *cnp;
    QueryNode *qnp;
    scanf("%d",&sayi);
    dolu=sayi;
    for(i=0;i<sayi;i++)
    {
        str=malloc(5*sizeof(char));
        scanf("%s %d",str,&ayi);
        tablo[i]=malloc(sizeof(QueryNode));
        tablo[i]->query=str;
        qnekle(cn(ayi),tablo[i]);
    }
    
    scanf("%d",&aramasay);
    for(ii=0;ii<aramasay;ii++)
    {
        str=malloc(5*sizeof(char));
        scanf("%s",str);
        for(i=0;i<dolu;i++)
        {
            if(!strcmp(str,tablo[i]->query)) break;
        }
        if(i!=dolu)
        {
            if(tablo[i]->prev!=NULL)
                (tablo[i]->prev)->next=tablo[i]->next;
            if(tablo[i]->next!=NULL)
                (tablo[i]->next)->prev=tablo[i]->prev;
            for(qnp=tablo[i];qnp->prev!=NULL;qnp=qnp->prev);
            for(cnp=bas;*(cnp->querylist)!=qnp;cnp=cnp->next);
            cns=cnp->count+1;
            if(tablo[i]->prev==NULL&&tablo[i]->next==NULL)
            {
            	if(cnp!=bas)
            	{
		            if(cnp->prev!=NULL)
		                (cnp->prev)->next=cnp->next;
		            if(cnp->next!=NULL)
		                (cnp->next)->prev=cnp->prev;
		        }
		        else
		        {
		        	bas=bas->next;
		        }
                free(cnp);
            }
            else if(tablo[i]->prev==NULL)
            {
            	*(cnp->querylist)=tablo[i]->next;
            }
            qnekle(cn(cns),tablo[i]);
        }
        else
        {
            qnp=malloc(sizeof(QueryNode));
            if(dolu==MAX_SIZE)
            {
                for(iii=0; tablo[iii]!= (*(bas->querylist)) ; iii++);
                if((*(bas->querylist))->next==NULL)
                {
                    bas=bas->next;
                    free(*((bas->prev)->querylist));
                    free(bas->prev);
                    bas->prev=NULL;
                }
                else
                {
                    *(bas->querylist)=(*(bas->querylist))->next;
                    (*(bas->querylist))->prev=NULL;
                }
            }
            else 
            {
                iii=dolu;
                dolu++;
            }
            tablo[iii]=qnp;
            qnp->query=str;
            
            
            qnekle(cn(1),qnp);
        }
    }

    yaz();
    return 0;
}
