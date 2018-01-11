Compiler for PIC32 Microchip
=========================================

[![Compiler Microchip](http://www.microchip.com/mplab/compilers)](http://www.microchip.com/mplab/compilers)

Microchip’s line of award-winning MPLAB® XC C Compilers provides a comprehensive solution for your project’s software development and is offered in free, unrestricted-use downloads. MPLAB XC32/32++ supports all 32-bit PIC MCUs

The optimizations found on MPLAB® XC C Compilers provide code size reductions and speed enhancements that benefit your design projects. PRO license is available for designs that require maximum code reductions and best performance. The MPLAB® XC C Compiler contains a free, 60-day trial of a PRO license for evaluation when activated. 


For more information, see the user guide in the following PDF file:

[![More Information about XC32 Compiler](http://ww1.microchip.com/downloads/en/DeviceDoc/50001686J.pdf)](http://ww1.microchip.com/downloads/en/DeviceDoc/50001686J.pdf)

MPLABX IDE for Microchip
========================

MPLAB® X Integrated Development Environment (IDE) is a software program that runs on a PC (Windows®, Mac OS®, Linux®) to develop applications for Microchip PIC® microcontrollers and dsPIC® digital signal controllers.

MPLAB X IDE provides an extensive array of features for developing and debugging applications on any device from the PIC microcontroller product family. However, as with any feature-rich program, it can be a challenge to learn to use all of its capabilities or even to find out what all of those capabilities are. The goal of this section of the Developer Help site is to help you to discover and learn how to use the many available features that are sure to simplify and enhance your development activities.

Follow the website to Download MPLAB® X Installer
http://microchip.wikidot.com/mplabx:installation
I would like the MPLAB X to be builin a stand-alone independent devlope platform, to control Operation System in my project. and control the Microchip Peripheral Components.

[![More information about MPLABX IDE](http://microchipdeveloper.com/mplabx:installation)](http://microchipdeveloper.com/mplabx:installation)


Harmony Microchip with Open Source Code
=======================================

The MPLAB® Harmony Integrated Software Framework is a flexible, abstracted, fully integrated firmware development platform for PIC32 microcontrollers. It takes key elements of modular and object oriented design, adds in the flexibility to use a Real-Time Operating System (RTOS) or work without one, and provides a framework of software modules that are easy to use, configurable for your specific needs, and that work together in complete harmony.

MPLAB Harmony includes a set of peripheral libraries, drivers and system services that are readily accessible for application development. The code development format allows for maximum re-use and reduces time to market.

The MPLAB Harmony basic framework is free to download. For information on what is included within the basic framework and their release versions please read "Release Notes". Premium products including third party and Microchip Solutions are available for purchase.

To download and install the source code and libraries for PIC32.

http://ww1.microchip.com/downloads/en/DeviceDoc/harmony_v1_06_linux_installer.run

[![More information about Harmony](http://microchipdeveloper.com/harmony:start)](http://microchipdeveloper.com/harmony:start)

Installation of Compiler XC32 and MPLABX
========================================

To install the compiler:

$ 7z x microchip_compilador.7z  

$ sudo mv microchip /opt/

To install the IPE tool to program and debug:

$ wget http://ww1.microchip.com/downloads/en/DeviceDoc/MPLABX-v3.35-linux-installer.tar

$ tar xvf MPLABX-v3.35-linux-installer.tar

$ sudo ./MPLABX-v3.35-linux-installer.sh

IMPORTANT: If you use MPLABX with newer version v4.xx.  You can have problems with IPE to program using commands-line and IPE-interface. Release from RESET. 

Include the compiler in ~/.bashrc
=================================

function microchip-compiler-v11 () {

    export PATH="/opt/microchip/xc32/v1.11/bin:$PATH"

    XC32_GCC_VERSION=`xc32-gcc -dumpversion`

    if [ "$XC32_GCC_VERSION" == "4.5.1" ]; then

        echo "Compiler xc32-gcc 4.5.1 for Microchip PIC32 PRO v1.11"

    else

        echo "ERROR: Compiler xc32-gcc $XC32_GCC_VERSION != 4.5.1 for Microchip PIC32 PRO v1.11"

    fi

}

function microchip-compiler-v34 () {

    export PATH="/opt/microchip/xc32/v1.34/bin:$PATH"

    XC32_GCC_VERSION=`xc32-gcc -dumpversion`

    if [ "$XC32_GCC_VERSION" == "4.5.2" ]; then

        echo "Compiler xc32-gcc 4.5.2 for Microchip PIC32 PRO v1.34"

    else

        echo "ERROR: Compiler xc32-gcc $XC32_GCC_VERSION != 4.5.2 for Microchip PIC32 PRO v1.34"

    fi

}



How to compile
==============

To compile with version v11.

$ microchip-compiler-v11

To compile with version v34.

$ microchip-compiler-v34

To compile in Contiki

$ make TARGET=odins-pic32


How to program
==============

To program with the application of [![MPLAB IPE](http://microchipdeveloper.com/ipe:launching-ipe-application)](http://microchipdeveloper.com/ipe:launching-ipe-application)

$ /opt/microchip/mplabx/v4.05/mplab_ipe/mplab_ipe

To program with commands line using the Pickit-3 kit.

$ java -jar /opt/microchip/mplabx/v4.05/mplab_ipe/ipecmd.jar -fRexlab_ecc_acelerated.hex -P32MX795F512L  -TPPK3 -Y -m

To program with commands line using the ICD-3 kit.

$ java -jar /opt/microchip/mplabx/v4.05/mplab_ipe/ipecmd.jar -fRexlab_ecc_acelerated.hex -P32MX795F512L  -TPICD3 -Y -m   

To program with commands line using general-configuration.

$ ${IPECMD} -f${target}.hex -P${MICROCHIP_MCU_MODEL} -TP$(KIT) -Y -m

How to debug
============

To debug with the application of [![CUTECOM](http://cutecom.sourceforge.net/)](http://cutecom.sourceforge.net/)

$ sudo cutecom

To debug with commands line, if the device is in ttyUSB0 and the UART is configured to 19200 baudrate.

$ ./contiki/tools/sky/serialdump-linux -b19200 /dev/ttyUSB0



