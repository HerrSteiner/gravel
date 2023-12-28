<CsoundSynthesizer>
<CsOptions>
-g -b 512
</CsOptions>
<CsInstruments>

sr = 48000
ksmps = 16
nchnls = 2
0dbfs = 1

; the global effect bus
gaRevLeft 	init 0
gaRevRight 	init 0

gaBBLeft		init 0
gaBBRight		init 0

gaWarpLeft	init 0
gaWarpRight	init 0

gaDelayLeft   init  0
gaDelayRight  init  0

; make random really random, rather
seed 0

giDistTable	ftgen	0,0, 257, 9, .5,1,270


instr 1;smp
iDur = p3 ;dur 0.1 #duration in seconds
kcps = p4 ;pitch 1 #speed/freq
ivol = p5 ;vol 0.5 #volume !0 1
ipan = p6 ;pan 0 #panorama in stereofield !0 1
irev = p7 ;rev 0 #amount reverb !0 1
iSampleNumber = p8;wav 2
kcutoff   = p9;cut 15000
iBB = p10 ;bb 0 # stutter fx amount
iWarp = p11;warp 0 #special delay fx amount !0 1
iDelay = p12 ;delay 0 #delay fx amount !0 1
ipanR = 1 - ipan
;aenv linen ivol,0.006,iEnvDur,0.006
aenv	    linenr    ivol, .01, .1, .01 
al,ar loscil3 aenv,kcps,iSampleNumber,1,0

al dcblock2 al
ar dcblock2 ar

aleft1, aright1 pan2 al, ipan
aleft2, aright2 pan2 ar, ipanR
aleft = aleft1 + aleft2
aright = aright1 + aright2

     outs aleft, aright
gaRevLeft = gaRevLeft + aleft*irev
gaRevRight = gaRevRight + aright*irev
gaBBLeft = gaBBLeft + aleft*iBB
gaBBRight = gaBBRight + aright*iBB
gaWarpLeft = gaWarpLeft + aleft*iWarp
gaWarpRight = gaWarpRight + aright*iWarp
gaDelayLeft = gaDelayLeft + aleft*iDelay
gaDelayRight = gaDelayRight + aright*iDelay

endin

instr 2;msmp
iDur = p3 ;dur 0.1 #duration in seconds
kcps = p4 ;pitch 1 #speed/freq
ivol = p5 ;vol 0.5 #volume !0 1
ipan = p6 ;pan 0.5 #panorama in stereofield !0 1
irev = p7 ;rev 0 #amount reverb !0 1
iSampleNumber = p8;wav 2
kcutoff   = p9;cut 15000
iBB = p10 ;bb 0 # stutter fx amount
iWarp = p11;warp 0 #special delay fx amount !0 1
iDelay = p12 ;delay 0 #delay fx amount !0 1
;aenv linen ivol,0.006,iEnvDur,0.006
aenv	    linenr    ivol, .01, .1, .01 
asig loscil3 aenv,kcps,iSampleNumber,1,0

asig dcblock2 asig
;asig = asig * 0.2 * ivol
aleft, aright pan2 asig, ipan
     outs aleft, aright
gaRevLeft = gaRevLeft + aleft*irev
gaRevRight = gaRevRight + aright*irev
gaBBLeft = gaBBLeft + aleft*iBB
gaBBRight = gaBBRight + aright*iBB
gaWarpLeft = gaWarpLeft + aleft*iWarp
gaWarpRight = gaWarpRight + aright*iWarp
gaDelayLeft = gaDelayLeft + aleft*iDelay
gaDelayRight = gaDelayRight + aright*iDelay

endin

instr 3;lof
iDur = p3 ;dur 0.1 #duration in seconds
kcps = p4 ;pitch 1 #speed/freq
ivol = p5 ;vol 0.5 #volume !0 1
ipan = p6 ;pan 0.5 #panorama in stereofield !0 1
irev = p7 ;rev 0 #amount reverb !0 1
iSampleNumber = p8;wav 2
kcutoff   = p9;cut 15000
iBB = p10 ;bb 0 # stutter fx amount
iWarp = p11;warp 0 #special delay fx amount !0 1
iDelay = p12 ;delay 0 #delay fx amount !0 1
ipanR = 1 - ipan

aenv	    linenr    ivol, .01, .1, .01 
al,ar loscil aenv,kcps,iSampleNumber,1,0

al dcblock2 al
ar dcblock2 ar
;asig = asig * 0.2 * ivol

aleft1, aright1 pan2 al, ipan
aleft2, aright2 pan2 ar, ipanR
aleft = aleft1 + aleft2
aright = aright1 + aright2

     outs aleft, aright
gaRevLeft = gaRevLeft + aleft*irev
gaRevRight = gaRevRight + aright*irev
gaBBLeft = gaBBLeft + aleft*iBB
gaBBRight = gaBBRight + aright*iBB
gaWarpLeft = gaWarpLeft + aleft*iWarp
gaWarpRight = gaWarpRight + aright*iWarp
gaDelayLeft = gaDelayLeft + aleft*iDelay
gaDelayRight = gaDelayRight + aright*iDelay

endin

instr 4;mlof
iDur = p3 ;dur 0.1 #duration in seconds
kcps = p4 ;pitch 1 #speed/freq
ivol = p5 ;vol 0.5 #volume !0 1
ipan = p6 ;pan 0.5 #panorama in stereofield !0 1
irev = p7 ;rev 0 #amount reverb !0 1
iSampleNumber = p8;wav 2
kcutoff   = p9;cut 15000
iBB = p10 ;bb 0 # stutter fx amount
iWarp = p11;warp 0 #special delay fx amount !0 1
iDelay = p12 ;delay 0 #delay fx amount !0 1
aenv	    linenr    ivol, .01, .1, .01 
asig loscil aenv,kcps,iSampleNumber,1,0
asig dcblock2 asig
;asig = asig * 0.2 * ivol
aleft, aright pan2 asig, ipan
     outs aleft, aright
gaRevLeft = gaRevLeft + aleft*irev
gaRevRight = gaRevRight + aright*irev
gaBBLeft = gaBBLeft + aleft*iBB
gaBBRight = gaBBRight + aright*iBB
gaWarpLeft = gaWarpLeft + aleft*iWarp
gaWarpRight = gaWarpRight + aright*iWarp
gaDelayLeft = gaDelayLeft + aleft*iDelay
gaDelayRight = gaDelayRight + aright*iDelay

endin

instr 5;fm

iDur = p3 ;dur 0.1 #duration in seconds
kcps = p4 ;pitch 30 #frequency in hz
ivol = p5 ;vol 1 #volume !0 1
ipan = p6 ;pan 0.5 #panorama in stereofield !0 1
irev = p7 ;rev 0 #amount reverb !0 1
kcar = p8 ;car 1 #carrier ratio
kmod = p9 ;ratio 2 #ratio modulator
iIndex = p10 ;index 20 #fm intensity
iBB = p11 ;bb 0 # stutter fx amount
iWarp = p12;warp 0 #special delay fx amount !0 1
iDelay = p13 ;delay 0 #delay fx amount !0 1

kndx line iIndex, iDur, 0	;intensivy sidebands
aexo expseg 0.00001, 0.01, 0.25, iDur, 0.00001

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
gaDelayLeft = gaDelayLeft + aleft*iDelay
gaDelayRight = gaDelayRight + aright*iDelay
endin


instr 6;tam
ivol = p5 ;vol 1 #volume !0 1
ipan = p6 ;pan 0.5 #panorama in stereofield !0 1
irev = p7 ;rev 0 #amount reverb !0 1
kcutoff1   = p8;cut1 2000
kcutoff2   = p9;cut2 1500
iBB = p10 ;bb 0 # stutter fx amount
iWarp = p11;warp 0 #special delay fx amount !0 1
iDelay = p12 ;delay 0 #delay fx amount !0 1

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
gaDelayLeft = gaDelayLeft + aleft*iDelay
gaDelayRight = gaDelayRight + aright*iDelay

endin


instr 7;pwm
iDur = p3 ;dur 1 #duration in seconds
icps = p4 ;pitch 40 #frequency in hz
iCut = p5 ;cut 10000
iAttack = p6 ;att 0.8 #rise time in seconds
iDecay = p7 ;dec 0.2 #decay time in seconds
ivol = p8 ;vol 1 #volume !0 1
ipan = p9 ;pan 0.5 #panorama in stereofield !0 1
irev = p10 ;rev 0 #amount reverb !0 1
iBB = p11 ;bb 0 # stutter fx amount
iWarp = p12;warp 0 #special delay fx amount !0 1
iDelay = p13 ;delay 0 #delay fx amount !0 1

iphase = rnd(1)

kdrop    expon icps*2, iDur, icps                   
kpw     linseg 0.1, iAttack, 0.9, iDecay, 0.1        ; PWM example
kFilter expseg 10,iAttack,iCut,iDecay,10

aOsc      vco2 0.25, kdrop, 2, kpw,iphase

afilter K35_lpf aOsc, kFilter, 3, 1, 1.1
aenv    linseg 1, iDur - 0.1, 1, 0.1, 0 
a1 = afilter*aenv*0.20*ivol

aleft, aright pan2 a1*0.6, ipan
     outs aleft, aright

gaRevLeft = gaRevLeft + aleft*irev
gaRevRight = gaRevRight + aright*irev
gaBBLeft = gaBBLeft + aleft*iBB
gaBBRight = gaBBRight + aright*iBB
gaWarpLeft = gaWarpLeft + aleft*iWarp
gaWarpRight = gaWarpRight + aright*iWarp
gaDelayLeft = gaDelayLeft + aleft*iDelay
gaDelayRight = gaDelayRight + aright*iDelay
endin



instr 8;bd
iDur = p3 ;dur 0.70 #duration in seconds
icps = p4 ;pitch 50 #frequency in hz
ivol = p5 ;vol 1 #volume !0 1
ipan = p6 ;pan 0.5 #panorama in stereofield !0 1
irev = p7 ;rev 0 #amount reverb !0 1
idist = p8 ;dist 0 #distortion !0 2
iBB = p9 ;bb 0 #amount stutter fx !0 1
iWarp = p10;warp 0 #special delay fx amount !0 1
iDelay = p11 ;delay 0 #delay fx amount !0 1

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
gaDelayLeft = gaDelayLeft + aleft*iDelay
gaDelayRight = gaDelayRight + aright*iDelay
endin



instr 9;sd
iDur = p3 ;dur 0.23 #duration in seconds
icps = p4 ;pitch 160 #frequency in hz
ivol = p5 ;vol 1 #volume !0 1
ipan = p6 ;pan 0.5 #panorama in stereofield !0 1
irev = p7 ;rev 0 #amount reverb !0 1
icut = p8 ;cut 200 #be careful with that one !20 2000
iBB = p9 ;bb 0 #amount stutter fx !0 1
iWarp = p10 ;warp 0 #special delay fx amount !0 1
iDelay = p11 ;delay 0 #delay fx amount !0 1

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
asig  = limit(asig, -1.0, 1.0)
	aleft, aright pan2 asig, ipan
	outs aleft, aright
gaRevLeft = gaRevLeft + aleft*irev
gaRevRight = gaRevRight + aright*irev

gaBBLeft = gaBBLeft + aleft*iBB
gaBBRight = gaBBRight + aright*iBB

gaWarpLeft = gaWarpLeft + aleft*iWarp
gaWarpRight = gaWarpRight + aright*iWarp

gaDelayLeft = gaDelayLeft + aleft*iDelay
gaDelayRight = gaDelayRight + aright*iDelay

endin	

instr 10;bass
idur = p3 ;dur 0.7 #duration in seconds
icps = p4 ;pitch 60 #frequency in hz
ivol = p5 ;vol 1 #volume !0 1
ipan = p6 ;pan 0.5 #panorama in stereofield !0 1
irev = p7 ;rev 0 #amount reverb !0 1
icut = p8 ;cut 6000 #filter cutoff frequency in hz
iBB = p9 ;bb 0 #amount stutter fx !0 1
iHarm = p10 ;harm 32 #additive oscillator harmonics
iWarp = p11 ;warp 0 #special delay fx amount !0 1
iDelay = p12 ;delay 0 #delay fx amount !0 1

ibuzzPhase = rnd(1)
iphase = rnd(1)
kfe  expseg 10,0.001, icut, idur*0.8, 10
asig buzz  0.5, icps, iHarm, 1,ibuzzPhase
asaw vco2  0.5,icps+0.1,0,0,iphase
keg expseg 0.00000001, 0.001, 1., idur, 0.00000001
afil moogladder asig + asaw, kfe, 0.1
     afil =  afil*0.22*keg*ivol
	aleft, aright pan2 afil, ipan
	outs aleft, aright
gaRevLeft = gaRevLeft + aleft*irev
gaRevRight = gaRevRight + aright*irev

gaBBLeft = gaBBLeft + aleft*iBB
gaBBRight = gaBBRight + aright*iBB

gaWarpLeft = gaWarpLeft + aleft*iWarp
gaWarpRight = gaWarpRight + aright*iWarp

gaDelayLeft = gaDelayLeft + aleft*iDelay
gaDelayRight = gaDelayRight + aright*iDelay
endin


instr 96
iFdback =        0.7           ; feedback ratio
aDelayLeft	init 0
aDelayRight	init 0

aDelayLeft  delay    gaDelayLeft+(aDelayLeft*iFdback), .5 ;delay 0.3 seconds
aDelayRight delay    gaDelayRight+(aDelayRight*iFdback), .5
        out      aDelayLeft, aDelayRight
        clear gaDelayLeft,gaDelayRight
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

;samples
f 2 0 0 1 "../../../samples/fbitsBD.wav" 0 0 0
f 3 0 0 1 "../../../samples/fbitsSD.wav" 0 0 0
f 4 0 0 1 "../../../samples/fbitsZng.wav" 0 0 0
f 5 0 0 1 "../../../samples/abrissDumpfSchellRX.wav" 0 0 0
f 6 0 0 1 "../../../samples/abschluesselRX.wav" 0 0 0

; start and let run the instruments defining the effects
i 96 0 36000
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
