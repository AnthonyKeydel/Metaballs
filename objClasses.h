class gVertex{
    public:
        float x;
        float y;
        float z;
        
        gVertex(){
            x = 0;
            y = 0;
            z = 0;
        }
        gVertex(float xin, float yin, float zin){
            x = xin;
            y = yin;
            z = zin;
        }
};

class mBall{
    public:
        float x;
        float y;
        float z;
        float value;
        
        mBall(){
            x = 0;
            y = 0;
            z = 0;
            value = 0;
        }
        mBall(float xin, float yin, float zin){
            x = xin;
            y = yin;
            z = zin;
            value = 0;
        }
        mBall(float xin, float yin, float zin, float valin){
            x = xin;
            y = yin;
            z = zin;
            value = valin;
        }
};
