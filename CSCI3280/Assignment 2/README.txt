CSCI3280 Assignment 2

This assignment implemented the full version of the algorithm described in the specification,
X, Y is in -120 to 120, Z is greater than 0, including the focal length f greater than 0.

If the X or Y value is out of range, 
it will be assigned with the background black color, i.e. rgb = (0, 0, 0)

Please first compile the program with command

C:\> cl.exe viewSynthesis.cpp bmp.cpp

Then run the program with command

C:\> viewSynthesis.exe LF_Views [X] [Y] [Z] [f]

While 
[f] is optional (100 default if not specified)
-120 <= [X],[Y] <= 120 
[Z], [f] >= 0

*All the .bmp source file are located at the LF_Views folder*
**Due to the storage problem of double type variable, small error within 0.001 maybe occurred**
