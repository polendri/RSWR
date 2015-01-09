Rainbow Stalin's Wild Ride
==========================

This repository is no longer being maintained.

![RSWR screenshot](screenshots/screenshot.png)

It's a very simplistic game written under a timeframe for a user interfaces
course at UWaterloo, and its borderline-inappropriate theme is inspired by a
a popular internet meme you can witness [here](http://psapin.github.io/).

I keep this thing around since it holds some personal charm to me. What follows
is the information and instructions I had to hand in when submitting it for the
course.

<div>
/------------------------------------\</br>
| RAINBOW STALIN'S WILD RIDE         |</br>
|                                    |</br>
| Author:     Paul Hendry (pshendry) |</br>
| Term:       Winter 2013            |</br>
| Course:     CS 349                 |</br>
| Assignment: 1                      |</br>
\------------------------------------/</br>
</br>
</br>
------------------</br>
TABLE OF CONTENTS:</br>
------------------</br>
</br>
1. How to build and run</br>
2. How to play</br>
3. Enhancements</br>
4. Miscellaneous notes</br>
</br>
</br>
-----------------------</br>
1. HOW TO BUILD AND RUN</br>
-----------------------</br>
</br>
Run "make" in the main directory of the project, and it will be built (if</br>
necessary) and run automatically. (Some deprecation warnings will appear, just</br>
ignore them.)</br>
</br>
</br>
--------------</br>
2. HOW TO PLAY</br>
--------------</br>
</br>
* Click "START" to begin the game. </br>
* Use the arrow keys to control the plane (the dropping of propaganda papers</br>
  is done automatically)</br>
* The goal is to drop a propaganda paper on each of the peasants encountered</br>
  (shown as black rectangles). Once brainwashed with propaganda, they will</br>
  change colour to red.</br>
* The game can be paused by clicking "QUIT" or pressing "f" in-game. This</br>
  returns you to the main screen, where you may click "RESUME" to continue the</br>
  game.</br>
* Score is recorded in the top-right corner of the screen. You obtain +1 score</br>
  for each peasant brainwashed.</br>
* The game ends when either an unbrainwashed peasant is missed (they disappear</br>
  on the left side of the screen) or the plane crashes into the ground.</br>
* FOR TAs: the "z" and "x" keys can be used to slow down and speed up scrolling</br>
  speed respectively. If you are having trouble playing the game, just hold</br>
  down "z" for awhile.</br>
</br>
</br>
---------------</br>
3. ENHANCEMENTS</br>
---------------</br>
</br>
* Resizing: The game is resizeable at all points during its execution: not</br>
    only the splash screen, but the game and defeat screens also.</br>
</br>
* Buttons: Several different buttons appear on various screens, which behave</br>
    like conventional UI buttons. They appear to visually be pressed down when</br>
    clicked (this seems not to occur for fast clicks so try holding a click</br>
    slightly to verify). Additionally, their behaviour is not activated until</br>
    the click is released, and only if the click is released on top of the</br>
    button.</br>
</br>
* Colour: Many colours are used.</br>
</br>
* Projectiles: A high number of projectiles exist simultaneously; additionally,</br>
    there is a random component to the velocity of each projectile, which can</br>
    be observed by flying up high.</br>
</br>
* Simple controls: The menus can be navigated using only button clicks, and the</br>
    plane is controlled using only the arrow keys. Sufficient challenge is</br>
    achieved without requiring an additional "shoot" button.</br>
</br>
* Keyboard shortcuts: The first letter of each menu button can also be pressed</br>
    on the keyboard to activate that button; as such, advanced users can </br>
    navigate the menus quickly without using the mouse.</br>
</br>
* Defeat screen: An additional screen is displayed when the player loses the</br>
    game.</br>
</br>
* Score: The player's score is recorded in the top-right corner of the screen</br>
    in-game, and final score is displayed on the defeat screen.</br>
</br>
* Artwork: Bitmaps are used on menu screens to display more elaborate and</br>
    appealing visuals (and buttons with text). Bitmaps are not used in-game</br>
    (except for the quit button for consistency) since they cannot be resized,</br>
    and I wanted the game screen to be resizeable.</br>
</br>
</br>
----------------------</br>
4. Miscellaneous notes</br>
----------------------</br>
</br>
* With regards to flicker: In testing my game, I encountered no noticeable</br>
  flicker when running it on my very underpowered laptop. I did notice an</br>
  occasional flicker when running it in the VM on the same laptop, but I</br>
  attribute this to the crappiness of my laptop combined with VM-related slowdowns</br>
  (since just using the VM at all is slow). All this to say that I chose not to</br>
  double-buffer because I saw no need in my own testing; so if things are a bit</br>
  flickery when you run it, please don't take off marks!</br>
</div>
