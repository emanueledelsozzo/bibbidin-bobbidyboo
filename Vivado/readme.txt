This readme describes how to build the Vivado project

Unzip the archive "project_1.xpr.zip" to extract folder "project_1". This is a complete Vivado project, the block design is complete, the user just has to run Synthesis, Implementation and Generate Bitstream.

In order to launch the project, the user may use this command within project_1 folder:
	vivado project_1.xpr

If the user wants to recreate the whole project, here the steps:

- Launch vivado with this command:
	vivado
- Click on "Create New Project"
- Click on "Next"
- Here, the user may change the project name
- Click on "Next"
- Select "RTL Project" and click "Next"
- Click on "Next"
- Click on "Next"
- Click on "Next"
- Click on "Boards" and select "Virtex-7 VC707 Evalutation Platform"
- Click on "Next"
- Click on "Finish"
- Under "IP Integrator" menu, click on "Create Block Design", and click on "OK"
- In the "Diagram" tab, click on "Add IP" and select "Memory Interface Generator (MIG 7 Series)"
- Click on "Run Block Automation", click "OK", and wait for the automation to complete
- Click on "Run Connection Automation", click "OK", and wait for the automation to complete
- In the "Diagram" tab, click on "Add IP" and select "MicroBlaze"
- Click on "Run Block Automation", click "OK", and wait for the automation to complete
- Click on "Run Connection Automation", select "All Automation", click "OK", and wait for the automation to complete
- Click on "Run Connection Automation", click "OK", and wait for the automation to complete
- In the "Diagram" tab, click on "IP Settings"
- Click on "Repository Manager" tab, and click on the green plus.
- Navigate to the HLS project (Once the High-Level Synthesis is done) and click on Select
- A message should pop-up stating that one IP has been found. Click on "OK", and again on "OK"
- In the "Diagram" tab, click on "Add IP" and select "Nbody"
- Click on "Run Connection Automation", select "All Automation", click "OK", and wait for the automation to complete
- Double click on the "AXI Interconnect" module and, in "Master Interfaces" tab, Enable Data FIFO for Master Interface M00_AXI by selecting "512 deep (packet mode)", and click "OK"
- Double click on the "MicroBlaze" module and, in the "Select Configuration" menu, select "Maximum Performance", and click "OK"
- Double click on the "MDM" module and Enable JTAG UART, and click "OK"
- Click on "Run Connection Automation", click "OK", and wait for the automation to complete
- In the "Diagram" tab, click on "Add IP" and select "AXI Timer"
- Click on "Run Connection Automation", click "OK", and wait for the automation to complete
- In the "Diagram" tab, click on "Validate Design" and wait for the validation to complete. If it is successful, click on "OK"
- In the "Design" box, click on "Sources" tab, right-click on "design_1" or the name you gave to the block design, "Create HDL Wrapper...", click on "Let Vivado manage wrapper and auto-update" option, and click "OK"
- Under "Syntesis" menu, click on "Synthesis Settings"
- Select "Flow PerfOptimized high" strategy, and click "OK"
- Under "Implementation" menu, click on "Implementation Settings"
- Select "Performance ExtraTimingOpt" strategy
- In "Opt Design" menu, select "Explore" directive
- In "Place Design" menu, select "ExtraTimingOut" directive
- In "Post-Place Phys Opt Design" menu, select "AggressiveExplore" directive
- In "Route Design" menu, select "Explore" directive
- In "Post-Route Phys Opt Design" menu, select "AggressiveExplore" directive
- Click "OK"
- Under "Program and Debug" menu, click on "Generate Bistream", save the design if Vivado asks to, click "OK" in "Launch Runs" window, click "Yes" in the "No Implementation Results Available" window, and wait for Vivado to complete the synthesis, implementation and bitstream generation
- Once the bitstream generation is over, click on "File" in the top tab, "Export", "Export Hardware", select "Include bitstream", and click "OK". This will generate the HDF file in your project.sdk folder.
