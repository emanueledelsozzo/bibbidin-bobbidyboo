//
//  parser.hpp
//  N-Body
//
//  Authors:
//  Emanuele Del Sozzo (emanuele.delsozzo@polimi.it), Lorenzo Di Tucci (lorenzo.ditucci@polimi.it), 
//  Marco Rabozzi (marco.rabozzi@polimi.it), Marco Nanni (marco3.nanni@mail.polimi.it)
//

#ifndef __PARSER__HPP__
#define __PARSER__HPP__

typedef struct params {
    int num_particles;
    int num_timesteps;
    float EPS;
    int random;
    int file;
    char *file_name;
} params_t;

struct options {
    char *param_short;
    char *param_long;
    int step;
    char param_val;
};

void print_usage();

int parse_input(int argc, char *argv[], params_t *args_info);

void free_params_t(params_t *args_info);


#endif /* parser_hpp */
