
// Authors:
// Emanuele Del Sozzo, Lorenzo Di Tucci, Marco Rabozzi, Marco Nanni


#ifndef SUPPORT_H_
#define SUPPORT_H_

typedef struct {
    float x;
    float y;
    float z;
} coord3d_t;

typedef struct {
    coord3d_t p;
    coord3d_t v;
} particle_t;

#endif
