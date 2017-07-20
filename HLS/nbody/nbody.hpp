//
//  nbody.hpp
//  N-Body
//
//  Authors:
//	Emanuele Del Sozzo (emanuele.delsozzo@polimi.it), Lorenzo Di Tucci (lorenzo.ditucci@polimi.it), 
//  Marco Rabozzi (marco.rabozzi@polimi.it), Marco Nanni (marco3.nanni@mail.polimi.it)
//

#ifndef __KERNEL__HPP__
#define __KERNEL__HPP__

#include "ap_int.h"

typedef float my_type;
#define AP_ELEM 16
#define ELEM 48
#define RED_0 8
#define RED_1 ELEM
#define ELEM_RATIO (ELEM/AP_ELEM)
#define N_INPUT (N/AP_ELEM)

#define MY_REDUCTION2(num) reduction_##num
#define MY_REDUCTION(num) MY_REDUCTION2(num)

#define TILE_ELEM 60000

#define TILE_SIZE (TILE_ELEM/AP_ELEM)
#define FOR_INDEX_TILE (TILE_SIZE*AP_ELEM)
#define LOOP_TILE (TILE_SIZE/ELEM_RATIO)
#define N_LOCAL_TILE (TILE_ELEM/ELEM)

typedef struct {
	my_type val[ELEM];
}my_type_48;


void nbody(ap_uint<512> *p_x, ap_uint<512> *p_y, ap_uint<512> *p_z, ap_uint<512> *a_x, ap_uint<512> *a_y, ap_uint<512> *a_z, ap_uint<512> *c, my_type EPS, unsigned int tiling_factor);

#endif
