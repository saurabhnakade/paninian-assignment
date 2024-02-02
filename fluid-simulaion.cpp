#include<iostream>

int getIndex(int x,int y,int n){
    return x+(y*n);
}

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

        // Constructor
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

        // Destructor
        ~Fluid(){
            delete[] sc;
            delete[] density;
            delete[] u;
            delete[] v;
            delete[] oU;
            delete[] oV;
        }

        // Add Density ie dye to a specific cell
        void addDensity(int x,int y,float amt){
            int index=getIndex(x,y,size);
            density[index]+=amt;
        }

        // Add Velocity to a specific cell
        void addVelocity(int x,int y,float amtX,float amtY){
            int index=getIndex(x,y,size);
            u[index]+=amtX;
            v[index]+=amtY;
        }
};

int main(){

}