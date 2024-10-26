# Readme

I added my name to keep this from conflicting with the one Robert is working on.

## Folder overview:

/build: reserved for the build output.
/deps: this folder is for the c/c++ dependencies.  Currently this folder is on .gitignore until I am certain about the final plan on how to incorporate zlib into the project.
/nodeApp: This is a folder for nodeJS apps.  With node installed, this can be configured by typing 'npm install'.  Currently there is a minizlib file, but I believe I will try to use the built in node zlib library instead and remove it later.  There is a public folder where I will put some standalone html drafts.  However, I do not intend to install express to host these files as the purpose of using node is to quickly build working code and that's the tool I'm good at.
/src: here's for the main software source folder.


## Okay, sh*** first draft done!

I'm just getting this out there, and I would say this code is only in draft mode.  It's also written as one big blob in the main.cpp file, which I would like to clean up, but...I digress.

## Building instructions...sort of:
Look at .gitignore and you will see that I made a subfolder in the /src directory called minizip.  This is where I installed the minizip library, which is a dependency.

Steps:
1) Download minizip, build it and stick it in a subfolder of src.
2) Fork the repository if you have to make changes to the makefile or other things.
3) Read the makefile: I wrote a single line to use clang, and the relative path to minizip/build/libminizip.a is right there.  Not much to read yet...
4) You're going to have to create a build folder on your own.  /build from the root of the repository.  I'll do the makefile magic to create the folder another day.
5) Run make, and the executable 'denigma' should pop out in the build folder.


## SOME WARNINGS
Read the code! be sure you know what it does, because unknown bugs might nuke files.  Please be careful with your files! I'll describe here what it's supposed to do:

1) make a subfolder matching the name of the musx file you have.  I.E. myJankyScore.musx will create a subfolder myJankyScore/
2) Dumps the zip contents of the file into that folder.  Should include:
	- META-INF/container.xml
	- presets/10001.preset
	- mimetype
	- NotationMetadata.xml
	- score.dat
3) It will make a temporary score.gz file in that subfolder, but will delete it before the process is done.
4) Then it does the amazing magic of decompressing and decrypting score.dat into score.enigmaxml.

## Usage
simply run denigma, on shell you would use "./denigma" with one argument, the path to the musx file.  For example

```
./denigma myJankyScore.musx

## or

./denigma ../my/path/to/myJankyScore.musx
```

It **should** put the subfolder right next to the musx file.

## Future plans and help
Down the road I would like to create recursive features, but I need to study up on safe practices with file trees.  Also, I could use some help with code review, checking or bugs and PORTABILITY

I am building this on MacOS 10.15.  I could use some help getting it running on windows.  Please reach out if you can help.  I would like to make it a one and done ./configure, make, make install setup, and all that.  But for now, this is where we are.  Welcome any suggestions on further reading for this.

Compatibility with MUSX versions is important too.  I'm not sure how consistent the contents of a musx file is across versions and all that.  Any input would be great.

Decoding the origional MUS files would also be a dream...but that one is way up in the stars.

## Why did we do this!
Well, J.S. Bach had been dead for longer than the life of Finale before Mozart discovered his work.  Had Mozart found a usb stick in the drawer with these unopenable .musx files, music history would be very different.  The purpose of denigma is to insure that as many .mus and .musx files from the millions of composers who have used Finale for the last 30 years are accessible, even in somewhat cryptic form, to future generations.

denigma automates the steps to decode the 'enigmaxml' data as we call it, which while not supported by any software other than finale (for now) still is stored in text based readable language.  My hope is that this tool will allow music historians of the future to discover forgotten archives of music from this century that did not make it on to a printed paper.