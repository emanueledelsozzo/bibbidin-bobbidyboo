//
//  main.cpp
//  N-Body
//
//  Authors:
//  Emanuele Del Sozzo (emanuele.delsozzo@polimi.it), Lorenzo Di Tucci (lorenzo.ditucci@polimi.it), 
//  Marco Rabozzi (marco.rabozzi@polimi.it), Marco Nanni (marco3.nanni@mail.polimi.it)
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include <unistd.h>
#include "support.hpp"
#include "parser.hpp"
#include "nbody.hpp"

/**
 * \brief Count the number of lines in a file
 * \param [in] fp       File pointer
 * \return The number of lines
 */
static int count_lines(FILE *fp)
{
    int nl = 0;
    int el = 0;
    char buf[BUFSIZ];
    while (fgets(buf, sizeof(buf), fp) != NULL) {
        if (strchr(buf, '\n')) {
            nl++;
            el = 0;
        } else {
            el = 1;
        }
    }
    return nl + el;
}

void final_computation(particle_t * p, coord3d_t *a, int N_loc){
    final:for (int i = 0; i < N_loc; i++) {
    		p[i].p.x += p[i].v.x;
            p[i].p.y += p[i].v.y;
            p[i].p.z += p[i].v.z;
            p[i].v.x += a[i].x;
            p[i].v.y += a[i].y;
            p[i].v.z += a[i].z;
        }
}

void central_computation(particle_t * p, coord3d_t *a, int N_loc, float EPS, const float *m){
    central1:for (int q = 0; q < N_loc; q++) {
            central2:for (int j = 0; j < N_loc; j++) {
                float rx = p[j].p.x - p[q].p.x;
                float ry = p[j].p.y - p[q].p.y;
                float rz = p[j].p.z - p[q].p.z;
                float dd = rx*rx + ry*ry + rz*rz + EPS;
                float d = 1/ (dd*sqrtf(dd));
                float s = m[j] * d;
                a[q].x += rx * s;
                a[q].y += ry * s;
                a[q].z += rz * s;
            }
        }
}


void data_generation(int N_loc, particle_t **particles, float **m, params_t args_info){
    
    if (!args_info.random && !args_info.file) {
        print_usage();
        exit(EXIT_FAILURE);
    }
    
    if (args_info.random) {
        *particles = (particle_t *) calloc(N_loc, sizeof(particle_t));
        *m = (float *) calloc(N_loc, sizeof(float));
        
        srand(time(NULL));
        for (int i = 0; i < N_loc; i++)
        {
            (*m)[i] = (float)rand()/100000;
            (*particles)[i].p.x = (float)rand()/100000;
            (*particles)[i].p.y = (float)rand()/100000;
            (*particles)[i].p.z = (float)rand()/100000;
            (*particles)[i].v.x = (float)rand()/100000;
            (*particles)[i].v.y = (float)rand()/100000;
            (*particles)[i].v.z = (float)rand()/100000;
            
            
        }
    } else {
        const char *filename = args_info.file_name;
        
        FILE *fp = fopen(args_info.file_name, "r");
        if (fp == NULL) {
            fprintf(stderr, "Failed to open input file: `%s'\n", filename);
            exit(EXIT_FAILURE);
        }
        
        N_loc = count_lines(fp) - 1;
        
        if (args_info.num_particles < N_loc) {
            N_loc = args_info.num_particles;
        }
        
        *particles = (particle_t *) calloc(N_loc, sizeof(particle_t));
        *m = (float *) calloc(N_loc, sizeof(float));
        
        rewind(fp);
        
        fscanf(fp, "m,x,y,z,vx,vy,vz\n");
        for (int i = 0; i < N_loc; i++) {
            fscanf(fp, "%g,%g,%g,%g,%g,%g,%g", &((*m)[i]),
                   &((*particles)[i]).p.x, &((*particles)[i]).p.y, &((*particles)[i]).p.z,
                   &((*particles)[i]).v.x, &((*particles)[i]).v.y, &((*particles)[i]).v.z);
        }
        
        fclose(fp);
    }

    
    
}


/**
 * \brief Run the N-body simulation on the CPU.
 * \param [in]  N_loc               Number of particles
 * \param [in]  nt              Number of time-steps
 * \param [in]  EPS             Damping factor
 * \param [in]  m               Masses of the N_loc particles
 * \param [in]  in_particles    Initial state of the N_loc particles
 * \param [out] out_particles   Final state of the N_loc particles after nt time-steps
 * \param [out] time            Execution time
 */
void run_cpu(int N_loc, int nt, float EPS, const float *m,
                    const particle_t *in_particles, particle_t *out_particles,
                    double *time)
{
    particle_t *p = (particle_t *) malloc(N_loc * sizeof(particle_t));
    memcpy(p, in_particles, N_loc * sizeof(particle_t));
    
    coord3d_t *a = (coord3d_t *) malloc(N_loc * sizeof(coord3d_t));
    
    double wall_time_start, wall_time_end;
    double time_it_start, time_it_end;
    double time_up_start, time_up_end;
    
    //wall_time_start = get_time();
    
    outer_loop:for (int t = 0; t < nt; t++) {
        
        memset(a, 0, N_loc * sizeof(coord3d_t));
        
       //time_it_start = get_time();
        central_computation(p,a,N_loc, EPS, m);
       //time_it_end = get_time();
        
        
        //time_up_start = get_time();
        final_computation(p,a,N_loc);
        //time_up_end = get_time();

    }
    
    //wall_time_end = get_time();
    
    //*time = wall_time_end - wall_time_start;
    
    memcpy(out_particles, p, N_loc * sizeof(particle_t));
    
    free(p);
    free(a);
}


void run_FPGA(int N_loc, int nt, float EPS, float *m,
                    const particle_t *in_particles, particle_t *out_particles)
{

	int tiling_factor = N_loc / TILE_ELEM;

//	if(N_loc % TILE_ELEM != 0){
//		tiling_factor++;
//	}
//
//	int new_N_loc = TILE_ELEM * tiling_factor;
	int new_N_loc = N_loc;

	particle_t *p = (particle_t *) malloc(new_N_loc * sizeof(particle_t));
    memcpy(p, in_particles, N_loc * sizeof(particle_t));
    
    float *m_local = (float *) calloc(new_N_loc, sizeof(float));
    memcpy(m_local, m, N_loc * sizeof(float));

    my_type p_x [new_N_loc];
    my_type p_y [new_N_loc];
    my_type p_z [new_N_loc];

    my_type v_x [new_N_loc];
    my_type v_y [new_N_loc];
    my_type v_z [new_N_loc];

    my_type a_x [new_N_loc];
    my_type a_y [new_N_loc];
    my_type a_z [new_N_loc];


    for(int i = 0; i < N_loc; i++){
        p_x[i] = p[i].p.x;
    	p_y[i] = p[i].p.y;
        p_z[i] = p[i].p.z;
        v_x[i] = p[i].v.x;
        v_y[i] = p[i].v.y;
        v_z[i] = p[i].v.z;
        a_x[i] = 0;
        a_y[i] = 0;
        a_z[i] = 0;

    }
        
    outer_loop:for (int t = 0; t < nt; t++) {
        
        nbody((ap_uint<512>*)p_x, (ap_uint<512>*)p_y, (ap_uint<512>*)p_z, (ap_uint<512>*)a_x, (ap_uint<512>*)a_y, (ap_uint<512>*)a_z, (ap_uint<512>*)m, EPS, tiling_factor);
        
        for(int kk = 0; kk < N_loc; kk++){
        	p_x[kk] += v_x[kk];
        	p_y[kk] += v_y[kk];
        	p_z[kk] += v_z[kk];
        	v_x[kk] += a_x[kk];
        	v_y[kk] += a_y[kk];
        	v_z[kk] += a_z[kk];
        }

    }

    for(int i = 0; i < N_loc; i++){
    	p[i].p.x = p_x[i];
    	p[i].p.y = p_y[i];
    	p[i].p.z = p_z[i];
    	p[i].v.x = v_x[i];
    	p[i].v.y = v_y[i];
    	p[i].v.z = v_z[i];
    }
    
    memcpy(out_particles, p, N_loc * sizeof(particle_t));
    
    free(p);
}

int main(int argc, char **argv)
{
    params_t args_info;
    
    printf("Before parsing...");

    if (parse_input(argc, argv, &args_info) != 0) {
        exit(EXIT_FAILURE);
    }
    
    printf("Done...");

    int N_loc = args_info.num_particles;
    int nt = args_info.num_timesteps;
    float EPS = args_info.EPS;
    float threshold = 0.1;
    
    if (EPS == 0) {
        fprintf(stderr, "EPS cannot be set to zero\n");
        exit(EXIT_FAILURE);
    }
    
    particle_t *particles;
    float *m;

    data_generation(N_loc, &particles, &m, args_info);
    
    double cpuTime = 0;
    particle_t *cpu_particles = NULL;
    particle_t *FPGA_particles = NULL;
    
    cpu_particles = (particle_t *) malloc(N_loc * sizeof(particle_t));
    FPGA_particles = (particle_t *)malloc(N_loc * sizeof(particle_t));
        
    printf("running on CPU ...\n");
    run_cpu(N_loc, nt, EPS, m, particles, cpu_particles, &cpuTime);
    printf("CPU execution time: %.3gs\n", cpuTime);

    printf("simulating FPGA..\n");
    run_FPGA(N_loc, nt, EPS, m, particles, FPGA_particles);

    int mismatches = 0;

    for(int i =0; i < N_loc; i++){
    	if( abs(FPGA_particles[i].p.x - cpu_particles[i].p.x) > threshold ||
    			abs(FPGA_particles[i].p.y - cpu_particles[i].p.y) > threshold ||
				abs(FPGA_particles[i].p.z - cpu_particles[i].p.z) > threshold ||
				abs(FPGA_particles[i].v.x - cpu_particles[i].v.x) > threshold ||
				abs(FPGA_particles[i].v.y - cpu_particles[i].v.y) > threshold ||
				abs(FPGA_particles[i].v.z - cpu_particles[i].v.z) > threshold){
    		printf("ERROR at index %d\n", i);
    		printf(" FPGA %f - CPU %f \n", FPGA_particles[i].p.x, cpu_particles[i].p.x);
    		printf(" FPGA %f - CPU %f \n", FPGA_particles[i].p.y, cpu_particles[i].p.y);
    		printf(" FPGA %f - CPU %f \n", FPGA_particles[i].p.z, cpu_particles[i].p.z);
    		printf(" FPGA %f - CPU %f \n", FPGA_particles[i].v.x, cpu_particles[i].v.x);
    		printf(" FPGA %f - CPU %f \n", FPGA_particles[i].v.y, cpu_particles[i].v.y);
    		printf(" FPGA %f - CPU %f \n", FPGA_particles[i].v.z, cpu_particles[i].v.z);
    		mismatches++;

    	}
    }
    
    printf("Mismatch rate: %f %\n", 100.0*mismatches/N_loc);
    printf("Results Checked! \n");
    

    free_params_t(&args_info);
    free(particles);
    free(m);
    free(cpu_particles);
    free(FPGA_particles);
    
    return 0;
}
