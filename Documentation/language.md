# Gravel Live Coding system

This sequencer software uses its own domain specific programming language Gravel to create complex sequences. A simple beat with a bassdrum on first note would be

`tdrum:[bd,0,0,0,0,0,0,0];`

As in C languages each command ends up with ;
But let me explain it from left to right:
We create a new track with the letter t and a name which should consist only from letters and numbers, no other signs. 
: signifies that we are done with the name, here drum, and the actual sequence is about to start. The more traditional way are sequences embraced by [ ]
Each position fills a bar and either takes an instrument name, here bd, or 0 for a silent pause. In the example it is a bd and 7 times pause which results in a persistent beat on the default 140 bpm. The part is activated by being in that line with the cursor and press shift + return or, in case several lines are about to be activated, by selecting the lines completely and then press shift + return.

That above can be written faster with the multiplier which can be applied to each position:

`tdrum:[bd,0*7];`

When a line with a track of the same name is activated with shift + return, it replaces the preceding definition but the change is not imidiant, its done after the pattern played its last event or pause.

To stop a specific track you can type the command stop followed with the name of the track (exclusive the beginning t) and a ;

`stop drum;`

To stop all playing tracks at once use just stop and ;

`stop;`