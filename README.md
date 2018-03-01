**IMPORTANT:**<br />
This repository represents Version 1.0 of the Bunny.exe and will not be updated any more.
Instead there will be a Visual-C++ application for Windows, which will receive new features.

The link to the new repository will be published at the begining of this readme file.<br />
<br />
<br />


**README:**
The Game of Life, or so,
'simulates' a bunny population

Download [Release/Bunnys.exe](Release/Bunnys.exe)
or go into [/Bunnys](Bunnys) and have fun<br /><br />

Press ```k```to initiate a mass murder on half of the population.<br />
Press ```w``` to accelerate and ```s``` to deccelerate the simulation (Windows only).

Use of Arguments (not case sensitive):
```Bunny.exe -x <int> -n <uint> -y <int> --loG```<br />
<br />
The values in brackets, are representing the default values.<br />

```(-)-help/h```&ensp;&emsp;display this argument list<br />
```-x (100)```&ensp;&emsp;&emsp;horizontal gridsize<br />
```-y (60)```&emsp;&emsp;&emsp;vertical gridsize<br />
```-bm (1000)```&ensp;&emsp;bunny max population size m<br />
```-bn (15)```&ensp;&emsp;&emsp;bunny start amount n<br />
```-inf (80)```&emsp;&emsp;infection probability for healthy bunnies<br />
```-fn (5)```&emsp;&emsp;&emsp;food amount at the beginning n<br />
```-fd (5317)```&ensp;&emsp;food duration, of the source<br />

```-slp (1000)```&emsp;time between to cylces in ms<br />
```--save```&ensp;&emsp;&emsp;&emsp;save the following game to a file<br />
```-save <PATH>```&ensp;specify this file<br />
```--load```&ensp;&emsp;&emsp;&emsp;load the saved game (--load is priorised over --save)<br />
```-load <PATH>```&ensp;specify this file<br />

```--log```&ensp;&ensp;&emsp;&emsp;&emsp;more detailed log<br />
```--nolog```&ensp;&ensp;&emsp;&emsp;no log (--log is priorised over --nolog)<br />
<br />
For the save and load function watch the upcoming documentation at [SaveDoc.md](SaveDoc.md)<br /><br />
The order and amount of arguments is not important.
Not feeded arguments will be replaced by default values.<br />
If an arguments is feeded twice, the last one will be taken into account.<br />
<br />
DISCLAIMER: all names of the bunnies are unrelated to any real person, and are not chosen by possible associations with real people or other groups/organisations.
