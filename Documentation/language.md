# Gravel Live Coding system

This sequencer software uses its own domain specific programming language Gravel to create complex sequences, grouped in tracks.
Unlike traditional sequencers, tracks in Gravel are not bound to a specific instrument. A track contains one or more patterns which can trigger all available instruments. The purpose of a track is to group these patterns under one name so that they can be stoped together while other tracks continue to play.

Due to the way Csound works, all intruments have unlimited polyphony, they can be triggered and layerd without limits except your CPU. So triggering an instrument in a fast way with the duration set to long time will not only add up the volume but also tax your computer and could lead to drop outs when your CPU can't calculate everything in time.

A simple beat with a bassdrum on first note would be

`tdrum:[bd,0,0,0,0,0,0,0];`

Let me explain it from left to right:
We create a new track by beginning with the letter **t** and a name which should consist only of letters and numbers, no other signs. 
**:** signifies that we are done with the name, here drum, and the actual sequence is about to start. The more traditional way are sequences embraced by **[ ]**.
A track, like also the other available commands in Gravel, ends with an **;**

Each position fills the pattern and either takes an instrument name, here **bd**, or **0** for a silent pause. In the example it is one event for the instrument **bd** and 7 times pause which results in a persistent beat on the default 140 bpm. After the last event the pattern is repeated so in other words the length of the pattern is defined by the amount of events. The part is evaluated by being in that line with the cursor and press **shift** and **return** or, in case several lines are about to be activated, by selecting the lines completely and then press **shift** and **return**.

That above can be written faster with the multiplier which can be applied to each position:

`tdrum:[bd,0*7];`

When a line with a track of the same name is activated with shift + return, it replaces the preceding definition but the change is not imediant, its done after the pattern played its last event or pause.

To stop a specific track you can type the command **stop** followed with the name of the track (exclusive the beginning **t**) and a **;**

`stop drum;`

To stop all playing tracks at once use just **stop** and **;**

`stop;`

Another way to define patterns with Gravel follow Euclidian ideas, here a bar with a fixed length is filled equally with the provided events. That type is signified with curly brackets **{ }**. So

`tbdrum:{bd};`

is basically the same as the example above.
The amount of clockbeats the content is destributed can be changed with **/** , default is 16. So this

`t1:{bd}/32;`

is slower because the single event is distributed over 32 clockbeats.

`t1:{bd}/8;` 

is therefore faster Here the trackname is just **1**. 

the speed of the sequencer can be changed with the command **bpm**, default are 140bpm. This changes it to 120bpm after being evaluated with **shift** and **return**:

`bpm 120;`