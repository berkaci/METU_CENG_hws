#include "BinaryHeap.h"
#include <iostream>

BinaryHeap::BinaryHeap()
{

}


bool BinaryHeap::Add(int uniqueId, double weight)
{
    int i, a=elements.size(), sim;
    for(i=0;i<a;i++)
    {
        if(elements[i].uniqueId==uniqueId) return false;
    }
    HeapElement ekl, tmp;
    ekl.uniqueId=uniqueId;
    ekl.weight=weight;
    elements.push_back(ekl);
    sim=a+1;
    while(sim>1)
    {
        if(elements[sim-1].weight<elements[sim/2-1].weight)
        {
            tmp=elements[sim-1];
            elements[sim-1]=elements[sim/2-1];
            elements[sim/2-1]=tmp;
        }
        sim/=2;
    }
    return true;

}

bool BinaryHeap::PopHeap(int& outUniqueId, double& outWeight)
{
    if(elements.size()==0)
    {
        return false;
    }
    HeapElement tmp;
    outUniqueId=elements[0].uniqueId;
    outWeight=elements[0].weight;
    elements[0]=elements[elements.size()-1];
    elements.pop_back();
    int i=1, byt=elements.size(), mini;
    while(true)
    {
        if(2*i>byt) break;
        else if(2*i+1 > byt)
        {
            if(elements[i-1].weight<elements[2*i-1].weight) break; ////asd
            else
            {
                tmp=elements[i-1];
                elements[i-1]=elements[2*i-1];
                elements[2*i-1]=tmp;
                i=2*i;
            }
        }
        else
        {
            if(elements[2*i-1].weight<elements[2*i+1-1].weight ||
               (elements[2*i-1].weight==elements[2*i+1-1].weight &&
                elements[2*i-1].uniqueId<elements[2*i+1-1].uniqueId)  )  //
            {
                mini=2*i;
            }
            else
            {
                mini=2*i+1;
            }
            if(elements[i-1].weight<elements[mini-1].weight) break; //////asd
            else
            {
                tmp=elements[i-1];
                elements[i-1]=elements[mini-1];
                elements[mini-1]=tmp;
                i=mini;
            }

        }

    }
    return true;
}

bool BinaryHeap::ChangePriority(int uniqueId, double newWeight)
{
    HeapElement tmp;
    int i, sim, mini, byt=elements.size();
    for(i=1 ; i<byt+1 ; i++)
    {
        if(elements[i-1].uniqueId==uniqueId) break;
    }
    if(i==byt+1) return false;
    elements[i-1].weight=newWeight;

    if(i>1 && elements[i-1].weight<elements[i/2-1].weight)
    {
        sim=i;
        while(sim>1)
        {
            if(elements[sim-1].weight<elements[sim/2-1].weight)
            {
                tmp=elements[sim-1];
                elements[sim-1]=elements[sim/2-1];
                elements[sim/2-1]=tmp;
            }
            sim/=2;
        }
    }
    while(true)
    {
        if(2*i>byt) break;
        else if(2*i+1 > byt)
        {
            if(elements[i-1].weight<elements[2*i-1].weight) break; ////asd
            else
            {
                tmp=elements[i-1];
                elements[i-1]=elements[2*i-1];
                elements[2*i-1]=tmp;
                i=2*i;
            }
        }
        else
        {
            if(elements[2*i-1].weight<elements[2*i+1-1].weight ||
               (elements[2*i-1].weight==elements[2*i+1-1].weight &&
                elements[2*i-1].uniqueId<elements[2*i+1-1].uniqueId)  )  //
            {
                mini=2*i;
            }
            else
            {
                mini=2*i+1;
            }
            if(elements[i-1].weight<elements[mini-1].weight) break; /////asd
            else
            {
                tmp=elements[i-1];
                elements[i-1]=elements[mini-1];
                elements[mini-1]=tmp;
                i=mini;
            }

        }

    }
    return true;
}

int BinaryHeap::HeapSize() const
{
    return elements.size();
}
