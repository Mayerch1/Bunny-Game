The Game of Life, or so,
'simulates' a bunny population

Download [Release/Bunnys.exe](Release/Bunnys.exe)
or go into [/Bunnys](Bunnys) and have fun<br /><br />

Press ```k```to initiate a mass murder on half of the population (Windows only).

Use of Arguments (not case sensitive):
```Bunny.exe -x <int> -n <uint> -y <int> --loG```<br />
<br />

```(-)-help/h```&ensp;&emsp;display this argument list<br />
```-x```&ensp;&emsp;&emsp;&emsp;&emsp;&emsp;horizontal gridsize<br />
```-y```&ensp;&emsp;&emsp;&emsp;&emsp;&emsp;vertical gridsize<br />
```-n```&ensp;&emsp;&emsp;&emsp;&emsp;&emsp;max population size n<br />
```-inf```&ensp;&emsp;&emsp;&emsp;&emsp;infection probability for healthy bunnies<br />
```-f```&ensp;&emsp;&emsp;&emsp;&emsp;&emsp;amount of food sources in the grid<br />
```-fd```&emsp;&emsp;&emsp;&emsp;&emsp;specify the lifetime of a foodsource<br />
```-s```&ensp;&emsp;&emsp;&emsp;&emsp;&emsp;number of bunnies at beginning<br />
```-slp```&ensp;&emsp;&emsp;&emsp;&emsp;time between to cylces in ms<br />
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