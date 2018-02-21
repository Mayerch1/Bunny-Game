# Bunny-Game
The Game of Life, or so,
'simulates' a bunny population

Download [Release/Bunnys.exe](Release/Bunnys.exe)
or go into [/Bunnys](Bunnys) and have fun<br /><br />

Press ```k```to initiate a mass murder on half of the population (Windows only).

Use of Arguments (not case sensitive):
```Bunny.exe -x <int> -n <uint> -y <int> --loG```<br />
<br />

```(-)-help/h```&emsp;&emsp;display this argument list<br />
```-x```&emsp;&emsp;&emsp;&emsp;&emsp;horizontal gridsize<br />
```-y```&emsp;&emsp;&emsp;&emsp;&emsp;vertical gridsize<br />
```-n```&emsp;&emsp;&emsp;&emsp;&emsp;max population size n<br />
```-inf```&emsp;&emsp;&emsp;&emsp;infection probability for healthy bunnies<br />
```-f```&emsp;&emsp;&emsp;&emsp;&emsp;amount of food sources in the grid<br />
```-s```&emsp;&emsp;&emsp;&emsp;&emsp;number of bunnies at beginning<br />
```-slp```&emsp;&emsp;&emsp;&emsp;time between to cylces in ms<br />
```--save```&ensp;&emsp;&emsp;save the following game to a file<br />
```--load```&ensp;&emsp;&emsp;load the saved game (--load is priorised over --save)<br />

```--log```&ensp;&emsp;&emsp;&emsp;more detailed log<br />
```--nolog```&ensp;&emsp;&emsp;no log (--log is priorised over --nolog)<br />
<br />
For the save and load function watch the upcoming documentation at [SaveDoc.md](SaveDoc.md)<br />
The order and amount of arguments is not important.
Not feeded arguments will be replaced by default values.<br />
If an arguments is feeded twice, the last one will be taken into account.
