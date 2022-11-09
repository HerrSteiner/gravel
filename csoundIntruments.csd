<CsoundSynthesizer>
<CsOptions>
-+rtaudio=auhal -odac5 -g -b 512
</CsOptions>
<CsInstruments>

sr = 48000
ksmps = 32
nchnls = 2
0dbfs = 1

; the global effect bus
gaRevLeft 	init 0
gaRevRight 	init 0

; make random really random, rather
seed 0

instr 1;fm

iDur = p3 ;dur 0.1
kcps = p4 ;pitch 30
kvol = p5 ;vol 1
kpan = p6 ;pan 0.5
krev = p7 ;rev 0
kcar = p8 ;car 4
kmod = p9 ;ratio 2
kndx line 20, iDur, 0	;intensivy sidebands
aexo expon 0.25, iDur, 0.0001

asig foscili aexo, kcps, kcar, kmod, kndx, 1
aleft, aright pan2 asig, kpan
     outs aleft, aright
gaRevLeft = gaRevLeft + aleft*krev
gaRevRight = gaRevRight + aright*krev
endin


instr 2;tam
kvol = p5 ;vol 1
kpan = p6 ;pan 0.5
krev = p7 ;rev 0
kcutoff1   = p8;cut1 2000
kcutoff2   = p9;cut2 1500
kfeedback1 = 0.26						;the sum of the two feedback
kfeedback2 = 0.25						;values should not exceed  0.5
asig tambourine .25, 0.01, 30, 0.5, 0.4,57,1024,223
asig wguide2 asig, 120, 1200, kcutoff1, kcutoff2, kfeedback1, kfeedback2
asig dcblock2 asig	
asig = asig * 0.25
aleft, aright pan2 asig, kpan
     outs aleft, aright
gaRevLeft = gaRevLeft + aleft*krev
gaRevRight = gaRevRight + aright*krev
endin


instr 3;pwm
iDur = p3 ;dur 0.13
kcps = p4 ;pitch 30
kvol = p5 ;vol 1
kpan = p6 ;pan 0.5
krev = p7 ;rev 0

kdrop    expon 100, iDur, 50                   
kpw     linseg 0.1, iDur/2, 0.9, iDur/2, 0.1        ; PWM example
a1      vco2 0.25, kdrop, 2, kpw
aenv    linseg 1, iDur - 0.1, 1, 0.1, 0 
a1 = a1*aenv*0.25
aleft, aright pan2 a1, kpan
     outs aleft, aright
gaRevLeft = gaRevLeft + aleft*krev
gaRevRight = gaRevRight + aright*krev
endin



instr 4;bd
iDur = p3 ;dur 0.13
kcps = p4 ;pitch 30
kvol = p5 ;vol 1
kpan = p6 ;pan 0.5
krev = p7 ;rev 0

aboumEnv expon 0.5, iDur, 0.00001
aboum vco2 0.25,kcps,4,0.6,0,0.33

aclickEnv expon 0.5,0.1,0.00001
kclickEnv expon 400,0.13,0.00001
aclick vco2 0.15,kclickEnv,4,0.6,0,0.22

asig = aclick*aclickEnv + aboum*aboumEnv
aleft, aright pan2 asig*0.25*kvol, kpan
	outs aleft, aright
gaRevLeft = gaRevLeft + aleft*krev
gaRevRight = gaRevRight + aright*krev
endin



instr 5;sd
iDur = p3 ;dur 0.13
kcps = p4 ;pitch 160
kvol = p5 ;vol 1
kpan = p6 ;pan 0.5
krev = p7 ;rev 0
kcut = p8 ;cut 200

aboumEnv expon 0.5, iDur, 0.00001
aboum vco2 0.5,kcps,4,0.6,0,0.33

aclickEnv expon 0.5,0.1,0.00001
kclickEnv expon 400,0.13,0.00001
aclick vco2 0.25,kclickEnv,4,0.6,0,0.22

anoiseEnv expon 0.5,0.2,0.00001
anoise noise 0.2,-0.9
ahp,alp,abp,abr statevar anoise, kcut, 4

asig = aclick*aclickEnv + aboum*aboumEnv + ahp*anoiseEnv
	aleft, aright pan2 asig*0.25*kvol, kpan
	outs aleft, aright
gaRevLeft = gaRevLeft + aleft*krev
gaRevRight = gaRevRight + aright*krev
endin	
      
instr 99
        denorm gaRevLeft, gaRevRight
aL, aR  reverbsc gaRevLeft, gaRevRight, 0.85, 12000, sr, 0.5, 1
     outs aL, aR
     clear gaRevLeft,gaRevRight
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
