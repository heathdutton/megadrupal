About "Textualizer"
=======================

Textualizer is a fancy jQuery drupal module for achieving 
awesome typographic effects.

The module accepts any number of sentences (or words) and 
can rotate them smartly and beautifully.

Once a sentence is about to rotate with the other one, the 
letters that will be re-used are kept and move to their new 
locations with several effects.

Textualizer currently has the following effects

 1. fadeIn
 2. slideLeft
 3. slideTop
 4. Random

It is possible to define the duration that each item will be 
displayed and also the duration of the transitions.

    Textualizer’s main use is most likely promotional texts 
    where quirky transition is of use.

Dependencies
============
  jQuery Update - http://drupal.org/project/jquery_update

Installation
============

After you activated the module (at "admin/modules") you can put the block to
any region on your page (using "admin/structure/block").

Configuration
=============

The configuration is availabe after you added the block to your site. The
easiest way is to click the configure link next to the block. Alternatively
you can use this lengthy link:
"admin/structure/block/manage/textualizer/textualizer-block/configure"

You can configure 4 aspects of the block:

  1. Text Sentences

     Enter some sentences which will be displayed in block with text transition
     effect. Use a new line for each Sentences.

  2. Animation Style

     The Style of the Animation effects.
     (fadeIn, slideLeft, slideTop, Random)

  3. Sentence Display Duration 
      
     Time (ms) each sentence will remain on screen.

  4. CSS Style for the Textualizer block

     Enter the CSS property of the textualizer block. Adjust width, height and
     font-size values to fit to your theme.
     Width and Height CSS Property are required.
