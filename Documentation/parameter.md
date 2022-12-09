# parameters and automation

One main advantage of Gravel are the multiple ways to sequence parameters of the instruments. Parameters are defined together with the instruments in the Csound script which is loaded when Gravel starts. The application scans for the definitions and make them available for the sequencer.

In the non editable part of Gravel below the code editor the available instruments and parameters are listed. The names colored in light blue are the ids you can use to refer in your sequence. For instance there is a parameter pan available for all instruments, affecting the stereo panorama position, range is 0.0 (left) - 1.0 (right) with 0.5 being the center and default when no parameter is written in the sequence. Preceding is the parameter sign **#** then parameter name, then a **:** and the value.

So

`tsynth:[bd#pan:0,0*15];`

creates a simple bass drum beat on the left speaker.

`tsynth:[bd#pan:0,0*7,bd#pan:1,0*7];`

adds another bass drums on the right side, with 7 times pause in between.

Parameters can be stacked, separated with the **#**.

`tsynth:[bass#pitch:70#pan:0.3,0*15];`

creates a Moogy synth sound, slighlty off center to the left. When you add more synth trigger events to your pattern with the same parameter settings, you don't have to repeat them, the last used ones can be repeated with the parameter signifier **#** and **$**.

`tsynth:[bass#pitch:70#pan:0.3,0*3,bass#$,0*7];`
