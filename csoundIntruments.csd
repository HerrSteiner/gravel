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

; the mixbus
gaMixBusLeft init 0
gaMixBusRight init 0

gkDelayTime init 0.5
gaDelayTime init 0.5
gkDelayFeedback init 0.5

; make random really random, rather
seed 0

giDistTable	ftgen	0,0, 257, 9, .5,1,270


instr 1;smp
iDur = p3 ;dur 0.1 #duration in seconds
icps = p4 ;pitch 1 #speed/freq
ivol = p5 ;vol 0.25 #volume !0 1
ipan = p6 ;pan 0.5 #panorama in stereofield !0 1
irev = p7 ;rev 0 #amount reverb !0 1
iSampleNumber = p8;wav 1 #sample number
iBB 	 = p9 ;bb 0 # stutter fx amount
iWarp = p10;warp 0 #special delay fx amount !0 1
iDelay = p11 ;delay 0 #delay fx amount !0 1

ichnls = ftchnls(p8)

aenv	    linenr    ivol, .01, .1, .01

if (ichnls == 1) then
	asig loscil3 aenv,icps,iSampleNumber,1,0
	ablock dcblock2 asig
	aleft, aright pan2 ablock, ipan

elseif (ichnls == 2) then

	al,ar loscil3 aenv,icps,iSampleNumber,1,0
	alb dcblock2 al
	arb dcblock2 ar
	if (ipan == 0.5) then
		aleft = alb
		aright = arb
	elseif (ipan < 0.5) then
		aleft = alb
		aright = arb * ipan * 2
	else 
		aleft = alb * (1 - (ipan - 0.5)*2)
		aright = arb
	endif

else ; can't handle multichannel wavs 
	aleft = 0
	aright = 0
endif
	; output
	gaMixBusLeft = gaMixBusLeft + aleft
	gaMixBusRight = gaMixBusRight + aright
     
     ;effects
	gaRevLeft = gaRevLeft + aleft*irev
	gaRevRight = gaRevRight + aright*irev
	gaBBLeft = gaBBLeft + aleft*iBB
	gaBBRight = gaBBRight + aright*iBB
	gaWarpLeft = gaWarpLeft + aleft*iWarp
	gaWarpRight = gaWarpRight + aright*iWarp
	gaDelayLeft = gaDelayLeft + aleft*iDelay
	gaDelayRight = gaDelayRight + aright*iDelay

endin

instr 2;lof
iDur = p3 ;dur 0.1 #duration in seconds
icps = p4 ;pitch 1 #speed/freq
ivol = p5 ;vol 0.25 #volume !0 1
ipan = p6 ;pan 0.5 #panorama in stereofield !0 1
irev = p7 ;rev 0 #amount reverb !0 1
iSampleNumber = p8;wav 2 #sample number
iBB 	 = p9 ;bb 0 # stutter fx amount
iWarp = p10;warp 0 #special delay fx amount !0 1
iDelay = p11 ;delay 0 #delay fx amount !0 1

ichnls = ftchnls(p8)

aenv	    linenr    ivol, .01, .1, .01

if (ichnls == 1) then
	asig loscil aenv,icps,iSampleNumber,1,0
	ablock dcblock2 asig
	aleft, aright pan2 ablock, ipan

elseif (ichnls == 2) then

	al,ar loscil aenv,icps,iSampleNumber,1,0
	alb dcblock2 al
	arb dcblock2 ar
	if (ipan == 0.5) then
		aleft = alb
		aright = arb
	elseif (ipan < 0.5) then
		aleft = alb
		aright = arb * ipan * 2
	else 
		aleft = alb * (1 - (ipan - 0.5)*2)
		aright = arb
	endif

else ; can't handle multichannel wavs 
	aleft = 0
	aright = 0
endif

     ; output
	gaMixBusLeft = gaMixBusLeft + aleft
	gaMixBusRight = gaMixBusRight + aright
     
     ;effects
	gaRevLeft = gaRevLeft + aleft*irev
	gaRevRight = gaRevRight + aright*irev
	gaBBLeft = gaBBLeft + aleft*iBB
	gaBBRight = gaBBRight + aright*iBB
	gaWarpLeft = gaWarpLeft + aleft*iWarp
	gaWarpRight = gaWarpRight + aright*iWarp
	gaDelayLeft = gaDelayLeft + aleft*iDelay
	gaDelayRight = gaDelayRight + aright*iDelay

endin

instr 3;sndwarp
iDur = p3 ;dur 0.1 #duration in seconds
icps = p4 ;pitch 1 #speed/freq
ivol = p5 ;vol 0.25 #volume !0 1
ipan = p6 ;pan 0.5 #panorama in stereofield !0 1
irev = p7 ;rev 0 #amount reverb !0 1
iSampleNumber = p8;wav 1 #sample number
ibeg = p9;start 0 #sample begin !0 1
iwsize = p10;wsize 100 #grain size
ioverlap = p11;over 5 #grain overlap
ispeed = p12;speed 1 #playspeed
irandw = p13;jitter 100 #range of randomness applied to grain position
iBB 	 = p14 ;bb 0 # stutter fx amount
iWarp = p15;warp 0 #special delay fx amount !0 1
iDelay = p16 ;delay 0 #delay fx amount !0 1

ichnls = ftchnls(p8)

aenv	    linenr    ivol, .01, .1, .01

if (ichnls == 1) then
	awarp,ac sndwarp  aenv, ispeed, icps, iSampleNumber, ibeg, iwsize, irandw, ioverlap, 99, 0
	asig balance awarp,ac 
	ablock dcblock2 asig
	aleft, aright pan2 ablock, ipan

elseif (ichnls == 2) then

	al,ar,acl,acr sndwarpst  aenv, ispeed, icps, iSampleNumber, ibeg, iwsize, irandw, ioverlap, 99, 0
	al balance al,acl 
	ar balance ar,acr
	
	alb dcblock2 al
	arb dcblock2 ar
	if (ipan == 0.5) then
		aleft = alb
		aright = arb
	elseif (ipan < 0.5) then
		aleft = alb
		aright = arb * ipan * 2
	else 
		aleft = alb * (1 - (ipan - 0.5)*2)
		aright = arb
	endif

else ; can't handle multichannel wavs 
	aleft = 0
	aright = 0
endif

     ; output
	gaMixBusLeft = gaMixBusLeft + aleft
	gaMixBusRight = gaMixBusRight + aright
     
     ;effects
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
asig foscili aexo, kcps, kcar, kmod, kndx, 100,iphase
aleft, aright pan2 asig*ivol*0.25, ipan

     ; output
	gaMixBusLeft = gaMixBusLeft + aleft
	gaMixBusRight = gaMixBusRight + aright
     
     ;effects
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
idur = p3 ;dur 0.4 #unused yet
iPitch = p4;pitch 1 #pitchfactor
ivol = p5 ;vol 1 #volume !0 1
ipan = p6 ;pan 0.5 #panorama in stereofield !0 1
irev = p7 ;rev 0 #amount reverb !0 1
kcutoff1   = p8;cut1 2000
kcutoff2   = p9;cut2 1500
iBB = p10 ;bb 0 # stutter fx amount
iWarp = p11;warp 0 #special delay fx amount !0 1
iDelay = p12 ;delay 0 #delay fx amount !0 1
ibells = p13 ;bell 30 #number of bells
;the sum of the two feedback
;values should not exceed  0.5
ifeedback1 = 0.26						
ifeedback2 = 0.25						
asig tambourine .25, 0.01, ibells, 0.5, 0.4,57,1024,223
asig wguide2 asig, 120, 1200, kcutoff1, kcutoff2, ifeedback1, ifeedback2
asig dcblock2 asig	

	; Setup phase vocoder
    ; Window size, overlap, and analysis bins
    iFftsize = 1024
    iOverlap = iFftsize / 4
    iBins = iFftsize

    ; Perform phase vocoder analysis
    fSpec pvsanal asig, iFftsize, iOverlap, iBins, 1

    ; Apply pitch shift
    fShifted pvscale fSpec, iPitch

    ; Resynthesize audio
    aOut pvsynth fShifted
asig = aOut * 0.2 * ivol
aleft, aright pan2 asig, ipan
    
    ; output
	gaMixBusLeft = gaMixBusLeft + aleft
	gaMixBusRight = gaMixBusRight + aright
     
     ;effects
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
     ; output
	gaMixBusLeft = gaMixBusLeft + aleft
	gaMixBusRight = gaMixBusRight + aright
     
     ;effects

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
idist = p8 ;dist 1 #distortion !0 2
iBB = p9 ;bb 0 #amount stutter fx !0 1
iWarp = p10;warp 0 #special delay fx amount !0 1
iDelay = p11 ;delay 0 #delay fx amount !0 1
iClickVol = p12 ;cvol 0.1 #click volume !0 1
iClickDecay = p13 ;cdec 0.1 #click decay

iCompensation = ivol + ivol*(idist*2)

aboumEnv expsegr 0.00000001, 0.01, 0.7, iDur, 0.00000001, 0.001, 0.00000001
aboum vco2 0.25,icps,4,0.6,0,0.33

aclickEnv expon 0.7,iClickDecay,0.00001
kclickEnv expon 400,iClickDecay + 0.03,0.00001
aclick vco2 0.15,kclickEnv,4,0.6,0,0.22

asig = aclick*aclickEnv*iClickVol + aboum*aboumEnv

adist distort asig,idist,giDistTable
aleft, aright pan2 adist*iCompensation, ipan
	; output
	gaMixBusLeft = gaMixBusLeft + aleft
	gaMixBusRight = gaMixBusRight + aright
     
     ;effects
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
iNoiseDecay = p12 ;ndec 0.2 #noise decay
iClickVol = p13 ;cvol 1 #click volume
iClickDecay = p14 ;cdec 0.1 #click decay

iphase = rnd(1)

aboumEnv expsegr 0.00000001, 0.01, 0.7, iDur, 0.00000001, 0.001, 0.00000001
//aboumEnv expon 0.5, iDur, 0.00001
aboum vco2 0.5,icps,4,0.6,iphase,0.33

aclickEnv expon 0.9,iClickDecay,0.00001
kclickEnv expon 400,iClickDecay+0.03,0.00001

aclick vco2 0.25,kclickEnv,4,0.6,0,0.22

anoiseEnv expon 0.5,iNoiseDecay,0.00001
anoise noise 0.2,-0.9
ahp,alp,abp,abr statevar anoise, icut, 4

asig = aclick*aclickEnv*iClickVol + aboum*aboumEnv + ahp*anoiseEnv
asig =  asig*0.6*ivol
asig  = limit(asig, -1.0, 1.0)
	
	aleft, aright pan2 asig, ipan
	; output
	gaMixBusLeft = gaMixBusLeft + aleft
	gaMixBusRight = gaMixBusRight + aright
     
     ;effects
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
asig buzz  0.5, icps, iHarm, 100,ibuzzPhase
asaw vco2  0.5,icps+0.1,0,0,iphase
keg expseg 0.00000001, 0.001, 1., idur, 0.00000001
afil moogladder asig + asaw, kfe, 0.1
     afil =  afil*0.42*keg*ivol
	aleft, aright pan2 afil, ipan

	; output
	gaMixBusLeft = gaMixBusLeft + aleft
	gaMixBusRight = gaMixBusRight + aright
     
     ;effects
	gaRevLeft = gaRevLeft + aleft*irev
	gaRevRight = gaRevRight + aright*irev

	gaBBLeft = gaBBLeft + aleft*iBB
	gaBBRight = gaBBRight + aright*iBB

	gaWarpLeft = gaWarpLeft + aleft*iWarp
	gaWarpRight = gaWarpRight + aright*iWarp

	gaDelayLeft = gaDelayLeft + aleft*iDelay
	gaDelayRight = gaDelayRight + aright*iDelay
endin

instr 11;dt
idur = p3 ;dur 0.1 #duration in seconds
iDelayTime = p4 ;s 0.5 #delaytime in s !0 5
iFeedback = p5 ;fb 0 #feedback !0 0.99
if (iFeedback > 0) then
	gkDelayFeedback = iFeedback
endif
iStart = i(gkDelayTime)
gkDelayTime = k(iDelayTime)
aDelayTime linseg iStart,idur,iDelayTime
gaDelayTime = aDelayTime
endin

;delay
instr 96
iFdback = i(gkDelayFeedback)*1.1           ; feedback ratio

aDelayLeft  vdelayx    gaDelayLeft, gaDelayTime,5.5,1024
aDelayRight vdelayx    gaDelayRight, gaDelayTime, 5.5,1024
          
          ; output
		gaMixBusLeft = gaMixBusLeft + aDelayLeft
		gaMixBusRight = gaMixBusRight + aDelayRight
        clear gaDelayLeft,gaDelayRight
        gaDelayLeft = aDelayLeft * iFdback
        gaDelayRight = aDelayRight * iFdback
endin

;fft based reverb/delay effect
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

	; output
	gaMixBusLeft = gaMixBusLeft + aleft
	gaMixBusRight = gaMixBusRight + aright
     clear gaWarpLeft,gaWarpRight
endin

; stutter effect 
instr 98

ibps = 12
isubdiv = 8
ibarlength = 2
iphrasebars = 2
inumrepeats = 16


aleft, aright bbcuts gaBBLeft, gaBBRight, ibps, isubdiv, ibarlength, iphrasebars, inumrepeats,1,0.7
		; output
		gaMixBusLeft = gaMixBusLeft + aleft
		gaMixBusRight = gaMixBusRight + aright
	      clear gaBBLeft,gaBBRight

endin
   
;reverb
instr 99
        denorm gaRevLeft, gaRevRight
aleft, aright  reverbsc gaRevLeft, gaRevRight, 0.85, 12000, sr, 0.5, 1
     ; output
	gaMixBusLeft = gaMixBusLeft + aleft
	gaMixBusRight = gaMixBusRight + aright
     clear gaRevLeft,gaRevRight
endin

;mixbus, the central stereo output
instr 100
	; potentially applying a delay as compensation when jam with others, default is minimum as possible, adjust this to your needs
	aDelayLeft delay gaMixBusLeft,0.0001
	aDelayRight delay gaMixBusRight,0.0001
	outs aDelayLeft,aDelayRight
	clear gaMixBusLeft,gaMixBusRight
endin

</CsInstruments>

<CsScore>

;other tables
f 99 0 1024 9 0.5 1 0	

; sine wave.
f 100 0 32768 10 1

; start and let run the instruments defining the effects and buses
i 96 0 36000
i 97 0 36000
i 98 0 36000
i 99 0 36000
i 100 0 36000

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
