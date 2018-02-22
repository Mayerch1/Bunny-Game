In the current state, you can't specify the output and input name.<br />
If you want to change that, simply copy the file and rename it, until the feature is pushed.<br />
<br />
The saved file is named ```game01.save```, doesn't allow Windows ```CL LF``` endings and is structured as followed:<br />
<br />
The first line is a simple warning to be careful with this file.<br />
<br />
The following line includes important argument, as they were in the saved run.<br />
```{<int>,<int>,<int>,<int>,<int>,};``` is filled with ```{gridX,gridY,BunnyCount,foodCount,max_hunger};```, terminated with ```;``` and a simple new line. The last ```,``` is mandatory.<br />
```max_hunger``` is not available via the arguments, the value will be assumed if the correlating values are feeded. It is anyway only interesting, if you want to disable the feeding system.<br />
<br />
The next piece of information represents the food sources. There must be exactly the same amount of sources/lines as defined with ```foodCount```above.<br />
```{<int>,<int>,}```simply represents the coordinates ```{x,y}``` of one food source. At the end, is again one simple newLine and the last food source is followed by one additional ```LF```newLine.
<br />
The last and usually biggest part represents every single bunny, which was alive when the game was saved.
The number of Bunnies/lines must be the same as defined with ```BunnyCount``` above.<br />
```{<int> <int> <int> <string> <int> <int> <int> <int>}``` shows one Bunny with the values ```{sex color age Name isMutant daySinceLastEaten coordinate.x coordinate.y }```, this time separated with blancs.<br />
The bunnies to each other are again seperated with one simple newLine ```LF```.
<br /><br />
At the end of the file is no additional newLine.
Common comments markups like ```//``` or ```--``` are NOT supported.
<br /><br /><br />
This is like a working saved game could look:<br/>
```
!!Modify at your own risk, wrong values could lead to pointer access violation. Be careful with hidden nl and blancs!!
{100,60,5,4,3,};
{63,4,}
{86,45,}
{84,1,}
{1,22,}

{0 2 10 Werner_von_Braun 0 1 68 4 }
{1 2 7 Kimberly 0 1 63 3 }
{0 2 3 Lamarck 0 1 68 6 }
{0 1 9 The_Hoff 0 1 65 4 }
{0 2 8 Sir_Oppenheimer 0 1 62 6 }
```
