This readme describes how to build the Vivado HLS project

- Enter the "nbody" folder
- The file "script.tcl" is in charge of building the project and exporting the IP Core. If the user wants to, both c-simulation and co-simulation may be performed. In particular, according to the number of bodies used for the co-simulation, the user could have to change the depth of master axi ports in "nbody.cpp" file. Please refer to "usage.txt" file for the description of testbench arguments.
- Run the script using this command: 
	vivado_hls script.tcl
- Once the High-Level Synthesis is over, the output may be found in "nbody_hls" folder.