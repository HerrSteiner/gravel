<CsoundSynthesizer>
<CsOptions>
-+rtaudio=auhal -odac3 -g -b 512
</CsOptions>
<CsInstruments>

sr = 48000
ksmps = 32
nchnls = 2
0dbfs = 1

instr 1;fm

iDur = p3 ;dur 0.1
kcps = p5 ;pitch 30
kcar = p4 ;car 4
kmod = p5 ;ratio 2
kndx line 20, iDur, 0	;intensivy sidebands
aexo expon 0.5, iDur, 0.0001

asig foscili aexo, kcps, kcar, kmod, kndx, 1
     outs asig, asig

endin

instr 2;tam
asig  tambourine .25, 0.01, 30, 0.5, 0.4,57,1024,223
kcutoff1   = p4;cut1 2000
kcutoff2   = p5;cut2 1500
kfeedback1 = 0.26						;the sum of the two feedback
kfeedback2 = 0.25						;values should not exceed  0.5
asig wguide2 asig, 120, 1200, kcutoff1, kcutoff2, kfeedback1, kfeedback2
asig dcblock2 asig	
      outs asig*0.25, asig*0.25
gaRevLeft = asig*0.25
gaRevRight = gaRevLeft
endin

instr 3;pwm
iDur = p3 ;dur 0.13
kcps    expon 100, iDur, 50                   
kpw     linseg 0.1, iDur/2, 0.9, iDur/2, 0.1        ; PWM example
a1      vco2 0.25, kcps, 2, kpw
aenv    linseg 1, iDur - 0.1, 1, 0.1, 0 
a1 = a1*aenv
        outs a1*0.25, a1*0.3
endin

instr 4;bd
iDur = p3 ;dur 0.13
kcps = p4 ;pitch 30

aboumEnv expon 0.5, iDur, 0.00001
aboum vco2 0.5,kcps,4,0.6,0,0.33

aclickEnv expon 0.5,0.1,0.00001
kclickEnv expon 400,0.13,0.00001
aclick vco2 0.25,kclickEnv,4,0.6,0,0.22

asig = aclick*aclickEnv + aboum*aboumEnv
	outs asig,asig
endin

instr 5;sd
iDur = p3 ;dur 0.13
kcps = p4 ;pitch 160
kcut = p5 ;cut 200

aboumEnv expon 0.5, iDur, 0.00001
aboum vco2 0.5,kcps,4,0.6,0,0.33

aclickEnv expon 0.5,0.1,0.00001
kclickEnv expon 400,0.13,0.00001
aclick vco2 0.25,kclickEnv,4,0.6,0,0.22

anoiseEnv expon 0.5,0.2,0.00001
anoise noise 0.2,-0.9
ahp,alp,abp,abr statevar anoise, kcut, 4

asig = aclick*aclickEnv + aboum*aboumEnv + ahp*anoiseEnv
	outs asig,asig
endin	
      
instr 99
        denorm gaRevLeft, gaRevRight
aL, aR  reverbsc gaRevLeft, gaRevRight, 0.85, 12000, sr, 0.5, 1
     outs aL, aR
endin

</CsInstruments>
<CsScore>
; sine wave.
f 1 0 32768 10 1

i 99 0 3600
e
</CsScore>
</CsoundSynthesizer>
<bsbPanel>
 <label>Widgets</label>
 <objectName/>
 <x>0</x>
 <y>0</y>
 <width>0</width>
 <height>0</height>
 <visible>true</visible>
 <uuid/>
 <bgcolor mode="nobackground">
  <r>255</r>
  <g>255</g>
  <b>255</b>
 </bgcolor>
</bsbPanel>
<bsbPresets>
</bsbPresets>
