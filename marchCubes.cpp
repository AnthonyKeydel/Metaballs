#include "CSCIx229.h"
#include "tables.h"
#include "mBalls.h"

//
// Marching Cubes algorithm
//
// based on the methods described
// (and tables provided) by
// Paul Bourke at
// http://paulbourke.net/geometry/polygonise/
//
float getVal(float fX, float fY, float fZ);
float marchCubes(float x, float y, float z, float scale, float threshold);


//getOffset finds the approximate intersection of the surface between two points
float getOffset(float val1, float val2, float goalVal)
{
        double delta = val2 - val1;

        if(delta == 0.0){
                return 0.5;
        }
        return (goalVal - val1)/delta;
}


//getColor generates a color from the normal of a triangle
void getColor(gVertex &color, gVertex &vertexNormal)
{
        float fX = vertexNormal.x;
        float fY = vertexNormal.y;
        float fZ = vertexNormal.z;
        color.x = (fX > 0.0 ? fX : 0.0) + (fY < 0.0 ? -0.5*fY : 0.0) + (fZ < 0.0 ? -0.5*fZ : 0.0);
        color.y = (fY > 0.0 ? fY : 0.0) + (fZ < 0.0 ? -0.5*fZ : 0.0) + (fX < 0.0 ? -0.5*fX : 0.0);
        color.z = (fZ > 0.0 ? fZ : 0.0) + (fX < 0.0 ? -0.5*fX : 0.0) + (fY < 0.0 ? -0.5*fY : 0.0);
}

void normalizeVertex(gVertex &vertexResult, gVertex &vertexSource){

        float oldLen;
        float scale;

        oldLen = sqrtf( (vertexSource.x * vertexSource.x) +
                        (vertexSource.y * vertexSource.y) +
                        (vertexSource.z * vertexSource.z) );

        if(oldLen == 0.0)
        {
                vertexResult.x = vertexSource.x;
                vertexResult.y = vertexSource.y;
                vertexResult.z = vertexSource.z;
        }
        else
        {
                scale = 1.0/oldLen;
                vertexResult.x = vertexSource.x*scale;
                vertexResult.y = vertexSource.y*scale;
                vertexResult.z = vertexSource.z*scale;
        }
}

//getVal finds the total val of (x, y, z) based on all the metaballs, and their sizes
float getVal(float x, float y, float z){

    float result = 0.0;
    float dX, dY, dZ;

    for(int i = 0; i<numBalls; i++){
        dX = x - metaballList[i].x;
        dY = y - metaballList[i].y;
        dZ = z - metaballList[i].z;
        result += metaballList[i].value/(dX*dX + dY*dY + dZ*dZ);
    }
        return result;
}


//getNormal() finds the gradient of the scalar field at a point
//This gradient can be used as a normal for the surface
void getNormal(gVertex &vertexNormal, float fX, float fY, float fZ)
{
        vertexNormal.x = getVal(fX-0.01, fY, fZ) - getVal(fX+0.01, fY, fZ);
        vertexNormal.y = getVal(fX, fY-0.01, fZ) - getVal(fX, fY+0.01, fZ);
        vertexNormal.z = getVal(fX, fY, fZ-0.01) - getVal(fX, fY, fZ+0.01);
        normalizeVertex(vertexNormal, vertexNormal);
}

float marchCubes(float x, float y, float z, float scale, float threshold, gVertex setColor){

        extern int edgeTable[256];
        extern int triTable[256][16];

        float offset, a=0, atemp=0;

        float cubeValues[8];
        gVertex edgeVertex[12];
        gVertex edgeNormal[12];
        int iCorner, iVertex, iVertexTest, iEdge, iTriangle, iFlagIndex, iEdgeFlags;

        gVertex color;

        //Make a copy of the values at the cube's corners
        for(iVertex = 0; iVertex < 8; iVertex++)
        {
                cubeValues[iVertex] = getVal(    x + cubeOffset[iVertex][0]*scale,
                                                y + cubeOffset[iVertex][1]*scale,
                                                z + cubeOffset[iVertex][2]*scale);
                atemp += cubeValues[iVertex];
        }
        atemp/=8;

        //Find the number of verticies of the cube that are inside the surface
        iFlagIndex = 0;
        for(iVertexTest = 0; iVertexTest < 8; iVertexTest++){

                if(cubeValues[iVertexTest] <= threshold){
                        iFlagIndex |= 1<<iVertexTest;
                }
        }

        //Find which edges are intersected by the surface
        iEdgeFlags = edgeTable[iFlagIndex];

        //If the cube is entirely inside or outside of the surface, then there will be no intersections
        //The flags will be all 0 if either iFlagIndex was never shifted (no vertices in surface), or
        // if it was shifted 8 times (all vertices in surface) due to the structure of edgeTable 
        if(iEdgeFlags == 0){
                return 0;
        }

        //Get the approximate average value of points on the surface, to show during debug
        if(a==0){ a = atemp; }
        else{ a = (a+atemp)/2; }

        //Find the point of intersection of the surface with each edge
        //Then find the normal to the surface at those points
        for(iEdge = 0; iEdge < 12; iEdge++){
                //if there is an intersection on this edge
                if(iEdgeFlags & (1<<iEdge)){
                        offset = getOffset(     cubeValues[ cubeEdgeVert[iEdge][0] ], 
                                                cubeValues[ cubeEdgeVert[iEdge][1] ], threshold);

                        edgeVertex[iEdge].x = x + (cubeOffset[ cubeEdgeVert[iEdge][0] ][0]  +  offset * cubeEdgeDir[iEdge][0]) * scale;
                        edgeVertex[iEdge].y = y + (cubeOffset[ cubeEdgeVert[iEdge][0] ][1]  +  offset * cubeEdgeDir[iEdge][1]) * scale;
                        edgeVertex[iEdge].z = z + (cubeOffset[ cubeEdgeVert[iEdge][0] ][2]  +  offset * cubeEdgeDir[iEdge][2]) * scale;

                        getNormal(edgeNormal[iEdge], edgeVertex[iEdge].x, edgeVertex[iEdge].y, edgeVertex[iEdge].z);
                }
        }


        //Draw triangles
        for(iTriangle = 0; iTriangle < 5; iTriangle++){
                if(triTable[iFlagIndex][3*iTriangle] < 0)
                        break;

                for(iCorner = 0; iCorner < 3; iCorner++)
                {
                        iVertex = triTable[iFlagIndex][3*iTriangle+iCorner];

                        if(setColor.x+setColor.y+setColor.z == 0){
                                getColor(color, edgeNormal[iVertex]);
                        }
                        else{
                                color = setColor;
                        }
                        glColor3f(color.x, color.y, color.z);
                        glNormal3f(edgeNormal[iVertex].x, edgeNormal[iVertex].y, edgeNormal[iVertex].z);
                        glVertex3f(edgeVertex[iVertex].x, edgeVertex[iVertex].y, edgeVertex[iVertex].z);
                }
        }
                return(a);
}