# FrequencyCounter
Tuner and Frequency counter of Essential shield

This program uses the F&S Essential shield to realize a frequency counter and musical tuner.
It utilises the interrupt trigger pin on the essential shield to measure the period between 2 trigger events.
At the same time a 10kHz interrupt routine is running which is here only used to trigger the refresh event for the display.

Hardware needed:
- F&S Essential shield
- 128*64 pixel black and white OLED display (SPI)
- 220nF condensator
- 10KOhm resistor

Place the display on the essential shield, connect the signal you want to measure to a 220nF condensator, the other side connect to 
10kOhm resistor and to the trigger input on the essential shield. The other pin of the resistor goes to GND.

Load the bin file into your essential shield. 
Now you are ready to measure! :-)
