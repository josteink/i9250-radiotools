# I9250 Radiotools

A few, very simple portable tools written by Jostein Kjønigsen to simplify
radio firmware-handling for the GSM Galaxy Nexus I9250 smartphone.

Code is known to work on Linux & Windows. Chances are it should work on OS X
and *BSD based OSes as well as well as arm-based devices.

## Disclaimer

Implementations are based on community-knowledge and I claim no credit for discovering these
nor responsibility based on usage of this code.

This code has been submitted to Github to avoid abandonware-syndrome. Further development is
not expected.

## Building it / dependencies

No dependencies apart from a basic C-compiler and standard libraries should be needed to build these tools.

On Debian-based Linux-distros the following should be enough:

    # root required.
    $ sudo apt-get install build-essential
    # this will take some time

Building the tools should be a straight forward task, without any need for autotools:
	
	# no root required for the remaining work.
    $ gcc radiotrim.c -o radiotrim
    $ gcc i9250radiograbber.c -o i9250radiograbber
    # quickly done
    $ _

To *use* these tools, you need to have the [Android SDK](https://developer.android.com/sdk/index.html)
installed and have its command-line tools in your shell path. Specifically you
need adb up and working.

The phone which you want to extract the radio-firmware from must also be *rooted*,
otherwise the tool wont be able to access the actual firmware itself.

## Checking that adb works

If you want to grab a radio off a device, ensure that you have adb working and ready, 
as the grabber depends on this. Adb needs to be in your path.

Connect you phone via USB and execute the following:

    $ adb devices
    List of devices attached 
    02343B0C11018018        device

    $ _

Reported serial doesn't matter. What's important is that your phone shows up. If it doesn't google
the internet to figure how to make adb work in your environment.

## I9250 radiograbber

With all that out of the way, you should be ready to rip your radio.

In a console window, enter the directory where you unpacked the stuff, and grab your radio.
On my machine it looks like this:

    $ ./i9250radiograbber radio.img
    
    Samsung Galaxy Nexus I9250 radio-image grabber.
    Grabbing started...
    49153+0 records in
    49153+0 records out
    12583168 bytes transferred in 12.724 secs (988931 bytes/sec)
    3042 KB/s (12583168 bytes in 4.038s)
    Radio grabbed successfully.
    Detected baseband name: SC04DOMLA1
    
    $ _

The resulting file, radio.img, is your ripped and trimmed radio, in fastboot format. Voila!

## Radiotrim

If you have a radio firmware-image which you cannot flash with fastboot, because it is reported
to be too large you can trim your image prior to flashing it.

This is just as easy as grabbing an image. In a console, just specify the source and the
destination file and you are pretty much done.

    $ ./radiotrim radio.img trimmed.img
    
    Samsung Galaxy Nexus I9250 radio-image trimmer.
    radio.img opened successfully.
    Detected baseband name: SC04DOMLA1
    Creating trimmed copy trimmed.img (12583168 bytes).
    
    $ _

To package it in a CWM-zip, just take any of the CWM packages found online and replace the radio.img file contained
with yours trimmed copy.

## Manually grabbing a image

If for some reason the util above doesn't work, you can manually grab an image and then trim it later.
With the phone hooked up via USB, do the following:

    $ adb shell 
    # == inside adb shell ==
    $ su
    # dd bs=256 count=49153 if=/dev/block/platform/omap/omap_hsmmc.0/by-name/radio of=/sdcard/radio.img
    49153+0 records in
    49153+0 records out
    12583168 bytes transferred in 11.365 secs (1107185 bytes/sec)
    # exit
    $ exit
    # == adb shell exited ==
    $ adb pull /sdcard/radio.img .
    # done

Note: This image has been extracted with its correct length. It doesn't need to be trimmed.

You can use the trimming to verify that it has been extracted correctly though, as this will output its recognized baseband name.
