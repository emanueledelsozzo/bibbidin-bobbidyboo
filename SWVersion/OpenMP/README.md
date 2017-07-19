Compile as:
make o3

Run example:
OMP_NUM_THREADS=4 ./N-Body -r -N 60000

Usage:    
  -h, --help               Print help and exit
  -N, --num-particles=INT  Maximum number of particles. The default value is 
                             only used
                             	 when the input is random.  (default=`384')
  -t, --num-timesteps=INT  Number of time-steps  (default=`1')
  -e, --EPS=FLOAT          Damping factor  (default=`100')

 Group: input
  -r, --random             Generate random input data
  -f, --file=FILE          Read input data from file

