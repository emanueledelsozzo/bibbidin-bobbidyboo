This readme describes how to test the proposed design using Xilinx SDK

- Start Xilinx SDK
- Create a new Application Project
- On the "New Project" window, give a name to the project and click on "New..." in the "Target Hardware" box
- On the "New Hardware Project" window, click on "Browse...", select the "design_1_wrapper.hdf" file, and click on "Finish"
- Back on the "New Project" window, click "Next >", select a template, like the "Hello World" one, and click "Finish"
- Copy into the "YourProjectName"/src folder the files in the code folder
- In the top bar, click on "Xilinx Tools" tab and then "Program FPGA"
- On the "Program FPGA" window, click on "Program" and wait for the FPGA to be programmed
- Within the file "helloworld.c", the user may change the value of define N in order to test a different number of bodies. It is important to notice that the best performance may be achieved only when N is multiple of TILE_ELEM (60000), according to the design described in the report. If N is not a multiple of TILE_ELEM, the code automatically allocates more bodies to provide the IP Core with a number of bodies multiple of TILE_ELEM. These bodies will not impact on the outcome of the computation, since their generic charge (e.g. mass in this case) is set to 0.
- In the top bar, click on "Run" tab and then "Run Configurations..."
- In the "Run Configurations" window, double click on "Xilinx C/C++ application (GDB)" to create a new configuration
- In the "Target Setup" tab, click on "Search..." and select the "design_1_wrapper.bit" bitstream
- In the "Application" tab, click on the "Browse..." button related to "Project Name", and select the project. This should fill the "Application" textbox as well. If not, click on "Search..." and select the .elf file
- In the "STDIO Connection" tab, check "Connect STDIO to Console" and select "JTAG UART" Port.
- Click on the "Run" button and wait for the computation to complete
- The program should print:
	- the number of cycles required to perform the computation at 100MHz
	- the execution time
	- the MPairs/s value