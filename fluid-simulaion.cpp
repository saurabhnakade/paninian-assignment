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
        // set bound
    }
}


// diffuses any set of values x , depending upon it's previous values x0 , diffusion amount and time stamp
static void diffuse(int b, float *x, float *x0, float diff, float dt, int iter, int n) {
    float a=dt*diff*(n-2)*(n-2);
    lin_solve(b, x, x0, a, 1 + 6 * a, iter, n);
}

// makes sure that the liquid is incompressible
static void project(float *velocX, float *velocY, float *p, float *div, int iter, int n)
{

    for (int j = 1; j < n - 1; j++) {
        for (int i = 1; i < n - 1; i++) {
            div[getIndex(i, j, n)] = -0.5f*(
                        velocX[getIndex(i+1, j  , n )]
                    -velocX[getIndex(i-1, j  , n )]
                    +velocY[getIndex(i  , j+1, n )]
                    -velocY[getIndex(i  , j-1, n )]
                )/n;
            p[getIndex(i, j, n)] = 0;
        }
    }

    //set bound
    lin_solve(0, p, div, 1, 6, iter, n);
    
    for (int j = 1; j < n - 1; j++) {
        for (int i = 1; i < n - 1; i++) {
            velocX[getIndex(i, j, n)] -= 0.5f * (  p[getIndex(i+1, j, n)]
                                            -p[getIndex(i-1, j, n)]) * n;
            velocY[getIndex(i, j, n)] -= 0.5f * (  p[getIndex(i, j+1, n)]
                                            -p[getIndex(i, j-1, n)]) * n;
        }
    }

    //set bound
}

// advection - transferring velocities
void advect(int b, float *d, float *d0, float *velocX, float *velocY, float dt, int n) {
    float i0, i1, j0, j1, k0, k1;
    float dtx = dt * (n - 2);
    float dty = dt * (n - 2);
    float dtz = dt * (n - 2);
    float s0, s1, t0, t1, u0, u1;
    float tmp1, tmp2, tmp3, x, y, z;
    float nfloat = n;
    float ifloat, jfloat, kfloat;
    int i, j, k;
    
    for (k = 1, kfloat = 1; k < n - 1; k++, kfloat++) {
        for (j = 1, jfloat = 1; j < n - 1; j++, jfloat++) {
            for (i = 1, ifloat = 1; i < n - 1; i++, ifloat++) {
                tmp1 = dtx * velocX[getIndex(i, j, k)];
                tmp2 = dty * velocY[getIndex(i, j, k)];
                tmp3 = dtz * velocZ[getIndex(i, j, k)];
                x = ifloat - tmp1;
                y = jfloat - tmp2;
                z = kfloat - tmp3;
                
                if (x < 0.5f) x = 0.5f;
                if (x > nfloat + 0.5f) x = nfloat + 0.5f;
                i0 = floorf(x);
                i1 = i0 + 1.0f;
                if (y < 0.5f) y = 0.5f;
                if (y > nfloat + 0.5f) y = nfloat + 0.5f;
                j0 = floorf(y);
                j1 = j0 + 1.0f;
                if (z < 0.5f) z = 0.5f;
                if (z > nfloat + 0.5f) z = nfloat + 0.5f;
                k0 = floorf(z);
                k1 = k0 + 1.0f;
                
                s1 = x - i0;
                s0 = 1.0f - s1;
                t1 = y - j0;
                t0 = 1.0f - t1;
                u1 = z - k0;
                u0 = 1.0f - u1;
                
                int i0i = i0;
                int i1i = i1;
                int j0i = j0;
                int j1i = j1;
                int k0i = k0;
                int k1i = k1;
                
                d[getIndex(i, j, k)] =
                    s0 * (t0 * (u0 * d0[getIndex(i0i, j0i, k0i)] + u1 * d0[getIndex(i0i, j0i, k1i)]) +
                          t1 * (u0 * d0[getIndex(i0i, j1i, k0i)] + u1 * d0[getIndex(i0i, j1i, k1i)])) +
                    s1 * (t0 * (u0 * d0[getIndex(i1i, j0i, k0i)] + u1 * d0[getIndex(i1i, j0i, k1i)]) +
                          t1 * (u0 * d0[getIndex(i1i, j1i, k0i)] + u1 * d0[getIndex(i1i, j1i, k1i)]));
            }
        }
    }
    // set bound
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