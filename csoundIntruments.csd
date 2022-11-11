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

gaBBLeft		init 0
gaBBRight		init 0

gaWarpLeft	init 0
gaWarpRight	init 0

; make random really random, rather
seed 0

giDistTable	ftgen	0,0, 257, 9, .5,1,270

instr 1;fm

iDur = p3 ;dur 0.1
kcps = p4 ;pitch 30
ivol = p5 ;vol 1
ipan = p6 ;pan 0.5
irev = p7 ;rev 0
kcar = p8 ;car 1
kmod = p9 ;ratio 2
iBB = p10 ;bb 0
iWarp = p11;warp 0

kndx line 20, iDur, 0	;intensivy sidebands
aexo expon 0.25, iDur, 0.0001

iphase = rnd(1)
asig foscili aexo, kcps, kcar, kmod, kndx, 1,iphase
aleft, aright pan2 asig*ivol*0.25, ipan
     outs aleft, aright
gaRevLeft = gaRevLeft + aleft*irev
gaRevRight = gaRevRight + aright*irev
gaBBLeft = gaBBLeft + aleft*iBB
gaBBRight = gaBBRight + aright*iBB
gaWarpLeft = gaWarpLeft + aleft*iWarp
gaWarpRight = gaWarpRight + aright*iWarp

endin


instr 2;tam
ivol = p5 ;vol 1
ipan = p6 ;pan 0.5
irev = p7 ;rev 0
kcutoff1   = p8;cut1 2000
kcutoff2   = p9;cut2 1500
iBB = p10 ;bb 0
iWarp = p11;warp 0

kfeedback1 = 0.26						;the sum of the two feedback
kfeedback2 = 0.25						;values should not exceed  0.5
asig tambourine .25, 0.01, 30, 0.5, 0.4,57,1024,223
asig wguide2 asig, 120, 1200, kcutoff1, kcutoff2, kfeedback1, kfeedback2
asig dcblock2 asig	
asig = asig * 0.2 * ivol
aleft, aright pan2 asig, ipan
     outs aleft, aright
gaRevLeft = gaRevLeft + aleft*irev
gaRevRight = gaRevRight + aright*irev
gaBBLeft = gaBBLeft + aleft*iBB
gaBBRight = gaBBRight + aright*iBB
gaWarpLeft = gaWarpLeft + aleft*iWarp
gaWarpRight = gaWarpRight + aright*iWarp

endin


instr 3;pwm
iDur = p3 ;dur 0.13
icps = p4 ;pitch 30
ivol = p5 ;vol 1
ipan = p6 ;pan 0.5
irev = p7 ;rev 0
iBB = p8 ;bb 0
iWarp = p9;warp 0

iphase = rnd(1)

kdrop    expon icps*2, iDur, icps                   
kpw     linseg 0.1, iDur/2, 0.9, iDur/2, 0.1        ; PWM example
a1      vco2 0.25, kdrop, 2, kpw,iphase
aenv    linseg 1, iDur - 0.1, 1, 0.1, 0 
a1 = a1*aenv*0.20*ivol
aleft, aright pan2 a1, ipan
     outs aleft, aright
gaRevLeft = gaRevLeft + aleft*irev
gaRevRight = gaRevRight + aright*irev
gaBBLeft = gaBBLeft + aleft*iBB
gaBBRight = gaBBRight + aright*iBB
gaWarpLeft = gaWarpLeft + aleft*iWarp
gaWarpRight = gaWarpRight + aright*iWarp

endin



instr 4;bd
iDur = p3 ;dur 0.70
icps = p4 ;pitch 50
ivol = p5 ;vol 1
ipan = p6 ;pan 0.5
irev = p7 ;rev 0
idist = p8 ;dist 0
iBB = p9 ;bb 0
iWarp = p10;warp 0
iCompensation = ivol + ivol*(idist*2)

aboumEnv expon 0.5, iDur, 0.00001
aboum vco2 0.25,icps,4,0.6,0,0.33

aclickEnv expon 0.5,0.1,0.00001
kclickEnv expon 400,0.13,0.00001
aclick vco2 0.15,kclickEnv,4,0.6,0,0.22

asig = aclick*aclickEnv + aboum*aboumEnv

adist distort asig,idist,giDistTable
aleft, aright pan2 adist*iCompensation, ipan
	outs aleft, aright
gaRevLeft = gaRevLeft + aleft*irev
gaRevRight = gaRevRight + aright*irev
gaBBLeft = gaBBLeft + aleft*iBB
gaBBRight = gaBBRight + aright*iBB
gaWarpLeft = gaWarpLeft + aleft*iWarp
gaWarpRight = gaWarpRight + aright*iWarp

endin



instr 5;sd
iDur = p3 ;dur 0.23
icps = p4 ;pitch 160
ivol = p5 ;vol 1
ipan = p6 ;pan 0.5
irev = p7 ;rev 0
icut = p8 ;cut 200
iBB = p10 ;bb 0
iWarp = p11 ;warp 0

iphase = rnd(1)

aboumEnv expon 0.5, iDur, 0.00001
aboum vco2 0.5,icps,4,0.6,iphase,0.33

aclickEnv expon 0.5,0.1,0.00001
kclickEnv expon 400,0.13,0.00001

aclick vco2 0.25,kclickEnv,4,0.6,0,0.22

anoiseEnv expon 0.5,0.2,0.00001
anoise noise 0.2,-0.9
ahp,alp,abp,abr statevar anoise, icut, 4

asig = aclick*aclickEnv + aboum*aboumEnv + ahp*anoiseEnv
asig =  asig*0.5*ivol
	aleft, aright pan2 asig, ipan
	outs aleft, aright
gaRevLeft = gaRevLeft + aleft*irev
gaRevRight = gaRevRight + aright*irev

gaBBLeft = gaBBLeft + aleft*iBB
gaBBRight = gaBBRight + aright*iBB

gaWarpLeft = gaWarpLeft + aleft*iWarp
gaWarpRight = gaWarpRight + aright*iWarp

endin	

instr 6;bass
idur = p3 ;dur 0.23
icps = p4 ;pitch 160
ivol = p5 ;vol 1
ipan = p6 ;pan 0.5
irev = p7 ;rev 0
icut = p8 ;cut 200
iBB = p10 ;bb 0
iHarm = p11 ;harm 20
iWarp = p12 ;warp 0

ibuzzPhase = rnd(1)
iphase = rnd(1)
kfe  expon icut, idur*0.9, 30
asig buzz  0.5, icps, iHarm, 1,ibuzzPhase
asaw vco2  0.5,icps+0.1,0,0,iphase
keg expon .5, idur, 0.000001
afil moogladder asig + asaw, kfe, 0.1
     afil =  afil*0.25*ivol
	aleft, aright pan2 afil, ipan
	outs aleft, aright
gaRevLeft = gaRevLeft + aleft*irev
gaRevRight = gaRevRight + aright*irev

gaBBLeft = gaBBLeft + aleft*iBB
gaBBRight = gaBBRight + aright*iBB

gaWarpLeft = gaWarpLeft + aleft*iWarp
gaWarpRight = gaWarpRight + aright*iWarp

endin

instr 97

ifftsize = 1024 * 2
ioverlap  = ifftsize / 4
iwinsize  = ifftsize
iwinshape = 1

fftLeft pvsanal gaWarpLeft, ifftsize, ioverlap, iwinsize, iwinshape
fftRight pvsanal gaWarpRight, ifftsize, ioverlap, iwinsize, iwinshape
;fftblurLeft pvscale fftLeft,0.4
;fftblurRight pvscale fftRight,0.4
;fftblurLeft pvsblur fftLeft,3.5,6
;fftblurRight pvsblur fftRight,3.5,6
;fftblurLeft pvswarp fftLeft,16,-500
;fftblurRight pvswarp fftRight,16,-500
fftblurLeft pvsmooth fftLeft,0.005,0.004
fftblurRight pvsmooth fftRight,0.004,0.005
aleft	pvsynth fftblurLeft
aright	pvsynth fftblurRight

	      outs aleft, aright
	      clear gaWarpLeft,gaWarpRight
endin


instr 98

ibps = 12
isubdiv = 8
ibarlength = 2
iphrasebars = 2
inumrepeats = 16


aleft, aright bbcuts gaBBLeft, gaBBRight, ibps, isubdiv, ibarlength, iphrasebars, inumrepeats,1,0.7
	      outs aleft, aright
	      clear gaBBLeft,gaBBRight

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
i 97 0 36000
i 98 0 36000
i 99 0 36000
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
