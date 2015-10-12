DESCRIPTION
===========

Fit Text is a jQuery plugin for font-sizes flexible.

It's useful if you are using a responsive theme (such as Omega), 
and want the text size to scale.

FitText makes font-sizes flexible. Use this plugin on 
your responsive design, to achieve scalable headlines 
that fill the width of the parent element.

CONFIGURATION
=============

* Go to the configuration screen at 
Administration » Configuration » Media » FitVids.

Selector
--------

* The default selector is "#site-name". Enter your own 
jQuery selectors. Use a new line for each selector.

Compressor
----------

If your text is resizing poorly, you'll want to turn tweak 
up/down "The Compressor". It works a little like a guitar amp. 
The default is 1.

$("#responsive_headline").fitText(1.2); // Turn the compressor up
(text shrinks more aggressively)
$("#responsive_headline").fitText(0.8); // Turn the compressor down
(text shrinks less aggressively)

This will hopefully give you a level of "control" that might 
not be pixel perfect, but scales smoothly & nicely.


FURTHER INFO
============

There is an explanatory blog post at 
https://github.com/davatron5000/FitText.js

