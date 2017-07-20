# Bibbidi N-BObbiDY Boo #

Authors:
Emanuele Del Sozzo (emanuele.delsozzo@polimi.it)
Lorenzo Di Tucci (lorenzo.ditucci@polimi.it)
Marco Rabozzi (marco.rabozzi@polimi.it)
Marco Nanni (marco3.nanni@mail.spolimi.it)

This repository contains the project "Bibbidi N-BObbiDY Boo", which was sumbitted to the Xilinx Open Hardware 2017. The goal of this project is to accelerate N-Body simulation on Xilinx's FPGA.

- Folder: HLS contains the code for Vivado HLS toolchain. It was designed with Vivado Design Suite 2016.4

- Folder: SWVersion contains both a single threaded and a parallel software version og N-Body simulation algirhtm

- Folder: Vivado contains the Vivado project. It may be necessary to update the IP repository in order to link the N-Body IP Core (after it has been exported from HLS)

- Folder: SDK contains a code example for Vivado SDK, as well as the HDF file containing the bitstream.

Such implementation was accelerated on VC707 Evaluation Board, powered by a Xilinx's Virtex 7 FPGA.


E. Del Sozzo, L. Di Tucci and M. D. Santambrogio, "A Highly Scalable and Efficient Parallel Design of N-Body Simulation on FPGA," 2017 IEEE International Parallel and Distributed Processing Symposium Workshops (IPDPSW), Orlando / Buena Vista, FL, USA, 2017, pp. 241-246.
doi: 10.1109/IPDPSW.2017.43
URL: http://ieeexplore.ieee.org/stamp/stamp.jsp?tp=&arnumber=7965050&isnumber=7965008
