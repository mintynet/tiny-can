# tiny-can
## If you would like a tiny-can PCB feel free to contact me on Twitter @mintynet, I will send if you cover the cost of postage.
<br>This repository has the arduino code for use with tiny-can PCBs.
<br>This was created as a cheap device for people to get into car hacking, it is based on the new Attiny1614 microcontroller which has hardware SPI to connect to the MCP2515 CAN controller, it also uses the SN65HVD230 3,3v CAN transceiver. The ATTINY 0 and 1 series of processors do need programming using three wire UPDI, more details in the link below.<br>
http://www.technoblogy.com/show?2OCH
<br>At time of creation the attiny1614 costs about £0.60.
# <b>I take no responsibility for any issues caused if using either the hardware or the code</b>
<br><br>Requires the use of the following Arduino library
<br><br>https://github.com/coryjfowler/MCP_CAN_lib
<br><br>The <b>tiny-slcan</b> sketch is slcan compatible software but the serial chip on the arduino boards I use will only run at 500000bps.
<br><br>If you use the following CP2102 serial adapter, pressing H will give you the bash script to allow you to start slcand under linux.
https://www.ebay.co.uk/itm/401062572142
<br><br>There are 4 PCBs.
#### tiny-can PCB
![tiny-can pcb schematic](pics/Schematic_tiny-can-pcb.png)
<br>Small PCB with Processor/CAN controller/transceiver<br>
![tiny-can pcb top](pics/tiny-can-pcb-top.png)
<br>Small PCB with Processor/CAN controller/transceiver TOP<br>
![tiny-can pcb bottom](pics/tiny-can-pcb-bottom.png)
<br>Small PCB with Processor/CAN controller/transceiver BOTTOM<br>
#### tiny-can Power only PCB (can be used for implant on OBD2 port)
![tiny-can Power only pcb schematic](pics/Schematic_power-obd2.png)
<br>Small PCB with Power circuit<br>
![tiny-can pcb top](pics/power-only-top.png)
<br>Small PCB with Power circuit TOP<br>
![tiny-can pcb bottom](pics/power-only-bottom.png)
<br>Small PCB with Power circuit BOTTOM<br>
![tiny-can-implant-front](photos/tiny-can-implant-front.jpg)
![tiny-can-implant-rear](photos/tiny-can-implant-rear.jpg)
![tiny-can-implant-side](photos/tiny-can-implant-side.jpg)
#### tiny-can Power and Serial PCB (used with slcan sketch for cheap CAN adapter)
![tiny-can pcb top](pics/power+serial-top.png)
<br>Small PCB with Power and Serial circuit TOP<br>
![tiny-can pcb bottom](pics/power+serial-bottom.png)
<br>Small PCB with Power and Serial BOTTOM<br>
![tiny-slcan-front](photos/tiny-slcan-front.jpg)
![tiny-slcan-rear](photos/tiny-slcan-rear.jpg)
![tiny-slcan-side](photos/tiny-slcan-side.jpg)
#### tiny-can plug PCB (fits inside a J1942 Male Plug)
![tiny-can plug pcb schematic](pics/Schematic_tiny-can-plug-pcb.png)
<br>Small PCB with Processor/CAN controller/transceiver and Serial header<br>
![tiny-can plug pcb top](pics/tiny-can-plug-pcb-top.png)
<br>Small PCB with Processor/CAN controller/transceiver and Serial header TOP<br>
![tiny-can plug pcb bottom](pics/tiny-can-plug-pcb-bottom.png)
<br>Small PCB with Processor/CAN controller/transceiver and Serial header BOTTOM<br>
![tiny-can-plug-front](photos/tiny-can-plug-front.jpg)
![tiny-can-plug-rear](photos/tiny-can-plug-rear.jpg)
![tiny-can-plug-front-shell](photos/tiny-can-plug-front-shell.jpg)
![tiny-can-plug-rear-shell](photos/tiny-can-plug-rear-shell.jpg)
<br><br>Notes for connecting to OBD2 plug.
<br>CAN L goes to OBD2 pin 14
<br>CAN H goes to OBD2 pin 6
