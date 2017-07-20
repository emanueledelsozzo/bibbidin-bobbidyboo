//
//  support.hpp
//  N-Body
//
//  Authors:
//	Emanuele Del Sozzo (emanuele.delsozzo@polimi.it), Lorenzo Di Tucci (lorenzo.ditucci@polimi.it), 
//  Marco Rabozzi (marco.rabozzi@polimi.it), Marco Nanni (marco3.nanni@mail.polimi.it)
//

#ifndef SUPPORT_HPP
#define SUPPORT_HPP

typedef struct {
    float x;
    float y;
    float z;
} coord3d_t;

typedef struct {
    coord3d_t p;
    coord3d_t v;
} particle_t;

/*
double get_time()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec * 1e-6;
}*/

#endif
