#include<iostream>

class Fluid{
    public:
        int size;
        float dt;
        float diff;
        float visc;

        float *sc;
        float *density;

        float *u;
        float *v;

        float *oU;
        float *oV;

        Fluid(int s,float d,float vi,float t){
            size=s;
            diff=d;
            visc=vi;
            dt=t;

            int n=size;

            sc=new float[n*n];
            density=new float[n*n];

            u=new float[n*n];
            v=new float[n*n];

            oU=new float[n*n];
            oV=new float[n*n];
        }

        ~Fluid(){
            delete[] sc;
            delete[] density;
            delete[] u;
            delete[] v;
            delete[] oU;
            delete[] oV;
        }
};

int main(){
    
}