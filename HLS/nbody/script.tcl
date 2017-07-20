############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2017 Xilinx, Inc. All Rights Reserved.
############################################################

# Authors:
#	Emanuele Del Sozzo (emanuele.delsozzo@polimi.it), Lorenzo Di Tucci (lorenzo.ditucci@polimi.it), 
# 	Marco Rabozzi (marco.rabozzi@polimi.it), Marco Nanni (marco3.nanni@mail.polimi.it)

open_project nbody_hls
set_top nbody
add_files nbody.cpp
add_files nbody.hpp
add_files utils.hpp
add_files -tb main.cpp
add_files -tb parser.cpp
add_files -tb parser.hpp
add_files -tb support.hpp
open_solution "solution1"
set_part {xc7vx485tffg1761-2} -tool vivado
create_clock -period 10 -name default
#csim_design -argv {-r -N 60000} -compiler gcc
csynth_design
#cosim_design -argv {-r -N 60000} -compiler gcc
export_design -rtl verilog -format ip_catalog
