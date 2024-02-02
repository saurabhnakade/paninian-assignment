#include<iostream>

static int getIndex(int x,int y,int n){
    return x+(y*n);
}

static void lin_solve(int b, float *x, float *x0, float a, float c, int iter, int n){
    float cRecip = 1.0 / c;
    for (int k = 0; k < iter; k++) {
        for (int j = 1; j < n - 1; j++) {
            for (int i = 1; i < n - 1; i++) {
                x[getIndex(i, j, n)] =
                    (x0[getIndex(i, j, n)]
                        + a*
                        ( x[getIndex(i+1, j  , n )]
                            +x[getIndex(i-1, j  , n  )]
                            +x[getIndex(i  , j+1, n  )]
                            +x[getIndex(i  , j-1, n  )]
                                +x[getIndex(i  , j  , n)]
                        )) * cRecip;
            }
        }
    }
}


// diffuses any set of values x , depending upon it's previous values x0 , diffusion amount and time stamp
static void diffuse(int b, float *x, float *x0, float diff, float dt, int iter, int n) {
    float a=dt*diff*(n-2)*(n-2);
    lin_solve(b, x, x0, a, 1 + 6 * a, iter, n);
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