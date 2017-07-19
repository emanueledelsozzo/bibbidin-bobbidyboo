# README #

This repository contains the project "Bibbidi N-BObbiDY Boo", which was sumbitted to the Xilinx Open Hardware 2017. The goal of this project is to accelerate N-Body simulation on Xilinx's FPGA.

- Folder: HLS contains the code for Vivado HLS toolchain. It was designed with Vivado Design Suite 2016.4

- Folder: SWVersion contains both a single threaded and a parallel software version og N-Body simulation algirhtm

- Folder: Vivado contains the Vivado project. It may be necessary to update the IP repository in order to link the N-Body IP Core (after it has been exported from HLS)

- Folder: SDK contains a code example for Vivado SDK, as well as the HDF file containing the bitstream.
