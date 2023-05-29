#include "MeshGraph.h"
#include "BinaryHeap.h"

// For printing
#include <fstream>
#include <iostream>
#include <sstream>


MeshGraph::MeshGraph(const std::vector<Double3>& vertexPositions,
                     const std::vector<IdPair>& edges)
{
    // TODO:
    int i, byt=vertexPositions.size();
    for(i=0;i<byt;i++)
    {
        Vertex vrt;
        vrt.id=i;
        vrt.position3D=vertexPositions[i];
        vertices.push_back(vrt);
        adjList.push_back((std::list<Vertex*>()));
    }
    byt=edges.size();
    for(i=0;i<byt;i++)
    {
        std::list<Vertex*>::iterator it(adjList[edges[i].vertexId0].begin()) ;
        for(  ; it!=adjList[edges[i].vertexId0].end() ; it++)
        {
            if(*it-&vertices[0]>edges[i].vertexId1) break;
        }
        adjList[edges[i].vertexId0].insert(it, &vertices[edges[i].vertexId1]);


        std::list<Vertex*>::iterator it2(adjList[edges[i].vertexId1].begin()) ;
        for(  ; it2!=adjList[edges[i].vertexId1].end() ; it2++)
        {
            if(*it2-&vertices[0]>edges[i].vertexId0) break;
        }
        adjList[edges[i].vertexId1].insert(it2, &vertices[edges[i].vertexId0]);
    }


}

double MeshGraph::AverageDistanceBetweenVertices() const
{
    int i, byt=adjList.size(), sayi=0;
    double top=0;
    for(i=0;i<byt;i++)
    {
        std::list<Vertex*>::const_iterator it(adjList[i].begin()) ;
        for(  ; it!=adjList[i].end() ; it++)
        {
            sayi++;
            top+=(*it)->position3D.Distance(vertices[i].position3D, (*it)->position3D);
        }

    }
    return top/sayi;
}

double MeshGraph::AverageEdgePerVertex() const
{
    int i, byt=adjList.size(), top=0;
    for(i=0;i<byt;i++)
    {
        top+=adjList[i].size();
    }
    return (double)(top)/(double)(byt)/2;
}

int MeshGraph::TotalVertexCount() const
{
    return vertices.size();
}

int MeshGraph::TotalEdgeCount() const
{
    int i, byt=adjList.size(), top=0;
    for(i=0;i<byt;i++)
    {
        top+=adjList[i].size();
    }
    return top/2;
}

int MeshGraph::VertexEdgeCount(int vertexId) const
{
    if(vertexId>=vertices.size()) return -1;
    return adjList[vertexId].size();
}

void MeshGraph::ImmediateNeighbours(std::vector<int>& outVertexIds,
                                    int vertexId) const
{
    outVertexIds.resize(0);
    if(vertexId>=vertices.size()) return;
    std::list<Vertex*>::const_iterator it(adjList[vertexId].begin()) ;
    for(  ; it!=adjList[vertexId].end() ; it++)
    {
        outVertexIds.push_back(*it-&vertices[0]);
    }


}

void MeshGraph::PaintInBetweenVertex(std::vector<Color>& outputColorAllVertex,
                                     int vertexIdFrom, int vertexIdTo,
                                     const Color& color) const
{
    if(vertexIdFrom>=(vertices.size()) || vertexIdTo>=(vertices.size()) || vertexIdFrom<0 || vertexIdTo<0) return;
    std::vector<double> mesafe(vertices.size());
    std::vector<int> prev(vertices.size());
    std::vector<bool> bitti(vertices.size(), false);
    BinaryHeap pq;
    int sim=vertexIdFrom;
    double simuzaklik;
    bool ekledi;
    mesafe[sim]=0;
    Color siyah;
    siyah.r=0;
    siyah.g=0;
    siyah.b=0;
    outputColorAllVertex.assign(vertices.size(), siyah);
    while(true)
    {
        if(sim==vertexIdTo) break;
        bitti[sim]=true;
        std::list<Vertex*>::const_iterator it(adjList[sim].begin()) ;
        for(  ; it!=adjList[sim].end() ; it++)
        {
            if(bitti[*it-&vertices[0]]) continue;
            simuzaklik=mesafe[sim] +(*it)->position3D.Distance(vertices[sim].position3D, (*it)->position3D);

            ekledi=
            pq.Add(*it-&vertices[0],  simuzaklik );

            if(ekledi)
            {
                prev[*it-&vertices[0]]=sim;
                mesafe[*it-&vertices[0]] = simuzaklik;
            }
            else
            {
                if(simuzaklik < mesafe[*it-&vertices[0]])
                {
                    mesafe[*it-&vertices[0]] = simuzaklik;
                    pq.ChangePriority(*it-&vertices[0], simuzaklik);
                    prev[*it-&vertices[0]]=sim;
                }
            }
        }
        if(pq.HeapSize()==0) return;
        pq.PopHeap(sim, simuzaklik);
    }
    while(sim!=vertexIdFrom)
    {
        outputColorAllVertex[sim]=color;
        sim=prev[sim];
    }
    outputColorAllVertex[sim]=color;


}

void MeshGraph::PaintInRangeGeodesic(std::vector<Color>& outputColorAllVertex,
                                    int vertexId, const Color& color,
                                    int maxDepth, FilterType type,
                                    double alpha) const
{
    if(vertexId>=(vertices.size()) || vertexId<0) return;
    std::vector<double> mesafe(vertices.size());
    std::vector<bool> sirada(vertices.size(),false);
    sirada[vertexId]=true;
    BinaryHeap pq;
    pq.Add(vertexId, 0);
    mesafe[vertexId]=0;
    Color siyah;
    Double3 nc;
    siyah.r=0;
    siyah.g=0;
    siyah.b=0;
    outputColorAllVertex.assign(vertices.size(), siyah);
    int kacincider=0, kacvar=1, gelecekkacvar=0, syc=0, i=1, oid;
    double ow;
    while(pq.HeapSize()!=0)
    {
        syc++;
        pq.PopHeap(oid, ow);
        std::list<Vertex*>::const_iterator it(adjList[oid].begin()) ;
        for(  ; it!=adjList[oid].end() ; it++)
        {
            if(sirada[*it-&vertices[0]]) continue;
            mesafe[*it-&vertices[0]]=mesafe[oid]+ (*it)->position3D.Distance(vertices[oid].position3D, (*it)->position3D);
            pq.Add(*it-&vertices[0], i++);
            gelecekkacvar++;
            sirada[*it-&vertices[0]]=true;
        }
        if(type==FILTER_BOX)
        {
            if(mesafe[oid]<=alpha)
            {
                outputColorAllVertex[oid]=color;
            }
        }
        else
        {
            nc.x=color.r;
            nc.y=color.g;
            nc.z=color.b;

            nc.x=nc.x*std::exp(-mesafe[oid]*mesafe[oid]/alpha/alpha);
            nc.y=nc.y*std::exp(-mesafe[oid]*mesafe[oid]/alpha/alpha);
            nc.z=nc.z*std::exp(-mesafe[oid]*mesafe[oid]/alpha/alpha);

            outputColorAllVertex[oid].r=nc.x;
            outputColorAllVertex[oid].g=nc.y;
            outputColorAllVertex[oid].b=nc.z;




        }

        if(syc==kacvar)
        {
            kacincider++;
            kacvar=gelecekkacvar;
            gelecekkacvar=0;
            syc=0;
            if(kacincider>maxDepth) break;
        }


    }

}

void MeshGraph::PaintInRangeEuclidian(std::vector<Color>& outputColorAllVertex,
                                      int vertexId, const Color& color,
                                      int maxDepth, FilterType type,
                                      double alpha) const
{









    if(vertexId>=(vertices.size()) || vertexId<0) return;
    std::vector<bool> sirada(vertices.size(),false);
    sirada[vertexId]=true;
    BinaryHeap pq;
    pq.Add(vertexId, 0);
    Color siyah;
    Double3 nc;
    double uzaklik;
    siyah.r=0;
    siyah.g=0;
    siyah.b=0;
    outputColorAllVertex.assign(vertices.size(), siyah);
    int kacincider=0, kacvar=1, gelecekkacvar=0, syc=0, i=1, oid;
    double ow;
    while(pq.HeapSize()!=0)
    {
        syc++;
        pq.PopHeap(oid, ow);
        std::list<Vertex*>::const_iterator it(adjList[oid].begin()) ;
        for(  ; it!=adjList[oid].end() ; it++)
        {
            if(sirada[*it-&vertices[0]]) continue;
            pq.Add(*it-&vertices[0], i++);
            gelecekkacvar++;
            sirada[*it-&vertices[0]]=true;
        }
        uzaklik=vertices[oid].position3D.Distance(vertices[oid].position3D, vertices[vertexId].position3D);
        if(type==FILTER_BOX)
        {
            if(uzaklik<=alpha)
            {
                outputColorAllVertex[oid]=color;
            }
        }
        else
        {
            nc.x=color.r;
            nc.y=color.g;
            nc.z=color.b;

            nc.x=nc.x*std::exp(-uzaklik*uzaklik/alpha/alpha);
            nc.y=nc.y*std::exp(-uzaklik*uzaklik/alpha/alpha);
            nc.z=nc.z*std::exp(-uzaklik*uzaklik/alpha/alpha);

            outputColorAllVertex[oid].r=nc.x;
            outputColorAllVertex[oid].g=nc.y;
            outputColorAllVertex[oid].b=nc.z;




        }

        if(syc==kacvar)
        {
            kacincider++;
            kacvar=gelecekkacvar;
            gelecekkacvar=0;
            syc=0;
            if(kacincider>maxDepth) break;
        }


    }












}

void MeshGraph::WriteColorToFile(const std::vector<Color>& colors,
                                 const std::string& fileName)
{
    // IMPLEMENTED
    std::stringstream s;
    for(int i = 0; i < static_cast<int>(colors.size()); i++)
    {
        int r = static_cast<int>(colors[i].r);
        int g = static_cast<int>(colors[i].g);
        int b = static_cast<int>(colors[i].b);

        s << r << ", " << g << ", " << b << "\n";
    }
    std::ofstream f(fileName.c_str());
    f << s.str();
}

void MeshGraph::PrintColorToStdOut(const std::vector<Color>& colors)
{
    // IMPLEMENTED
    for(int i = 0; i < static_cast<int>(colors.size()); i++)
    {
        std::cout << static_cast<int>(colors[i].r) << ", "
                  << static_cast<int>(colors[i].g) << ", "
                  << static_cast<int>(colors[i].b) << "\n";
    }
}
