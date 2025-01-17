# uSDR-pico
A uSDR implementation based on a RP2040 Pi Pico.  
This code is experimental, intended to investigate how the HW and SDK work with an application like this. So download and play with it, ask questions and  suggest improvements, but do not expect any kind of "product" support. 

This is the repository for an experimental implementation of the control and signal processing for a QSD/QSE based transceiver. The platform used is a Pi Pico module with an RP2040 processor. This processor has dual core, running at 125MHz each and very configurable I/O which eases the HW design.

The software consists of a TX branch and an RX branch, each running inside a timer callback function, once every 16 usec. This makes the signal processing rythm 62.5kHz. At a later stage this could be cranked up, or maybe # of taps could be increased, but the goal for now is to get the functionality running. 
 
The TX branch 
- samples audio input with ADC2 (rate = 62.5 kHz), 
- applies a low-pass filter Fc=3kHz, 
- reduces sampling by 2 to get better low frequency Hilbert transform (rate = 31.25 kHz), 
- splits into an I-channel 7 sample delay line and a Q-channel 15-tap DHT
- scales and outputs I and Q samples on PWM based DACs towards filters, opamps and QSE
 
The RX branch
- intermittently samples I and Q channels from QSD on ADC0 and ADC1 (rate = 31.25 kHz)
- corrects for sampling shift between I and Q (average last two I samples)
- applies 15-tap DHT on Q channel and 7 sample delay on I channel
- adds I and Q samples
- scales and outputs audio on an PWM based DAC

The main loop takes care of user I/O and the serial port. There is also a LED flashing timer callback.

The Pico controls an Si5351A clock module to obtain the switching clock for the QSE and QSD. The module outputs two synchronous square wave clocks on ch 0 and 1, whith selectable phase difference (0, 90, 180 or 270 degrees). The clock on ch2 is free to be used for other goals. The module is controlled over the **i2c1** channel.
The display is a standard 16x2 LCD, but with an I2C interface. The display is connected through the **i2c0** channel.

## Open issues: 
- [x] take care of processing cycles, by moving signal processing parts to the second core
- [X] add some more filtering
- [x] implement the user I/O bit: LCD+Rotary encoder+buttons
- [ ] implement AGC and RSSI
- [ ] implement other modes than USB
- [ ] sort out the surrounding HW
- [ ] maybe design a PCB?

## Installing and using the SDK for Windows: 
Please refer to https://github.com/ndabas/pico-setup-windows/releases where the latest installer can be downloaded (e.g. **pico-setup-windows-0.3-x64.exe**).  
Execute the installer to set up the SDK environment, e.g. in **~/Documents/Pico**  (let's call this folder $PICO). 

## Building uSDR-pico: 
Clone/copy the uSDR-pico code files into a subdirectory, e.g. **$PICO/uSDR-pico**  
Create the build folder: **$PICO/uSDR-pico/build**  

Before doing any building you need to adapt the file **$PICO/uSDR-pico/CMakeLists.txt**, using your favourite browser, to reflect your own directory structure.  
In **$PICO/** you will find a command to start a Developer Command Prompt window. Within this DCP all environment settings are initialized to enable building.  
In the DCP window, chdir to the build folder and execute: **cmake -G "NMake Makefiles" ..**  
Now you have initialized the make environment, and by executing **nmake** in that same build folder, the Pi Pico loadable file **uSDR.uf2** is created.  
Rebooting the Pico while the bootsel button is pressed will open a file explorer window with the Pico as a Mass Storage Device. Moving the binary to the Pico is as easy as dragging and dropping this uf2 file into that MSD.  

# Background
The folder **$PICO/docs** also contains some manuals, of which the *C-SDK description*, the *RP2040 datasheet* and the *Pico Pinout* are absolute must-reads when you start writing software.  
For calculating filters I have used the free software from Iowa Hills (http://www.iowahills.com/8DownloadPage.html)  
I also used the online FIR filter calculator T-Filter (http://t-filter.engineerjs.com/) 

