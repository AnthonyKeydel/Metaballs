#include "CSCIx229.h"
//metaball data
const int numBalls = 3;
mBall metaballList[numBalls];

    void setMetaBall(int index, float xin, float yin, float zin, float valin){
        if(index < numBalls){
            metaballList[index].x = xin/10;
            metaballList[index].y = yin/10;
            metaballList[index].z = zin/10;
            metaballList[index].value = valin/5;
        }
    };

    void SetupMetaballs(float radius){
        setMetaBall(0,   5,5,5,      radius);
        setMetaBall(1,   3,3,3,     radius);
        setMetaBall(0,   3,3,7,      radius);
    };
