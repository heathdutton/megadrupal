***********************************************************************
             Slideshow Creator Thumbnail Pager Feature
***********************************************************************

THIS FEATURE IS CURRENTLY EXPERIMENTAL
--------------------------------------

The software AFAICT works fine, but the design of the feature might
change if I receive good suggestions for improvement. For example,
can it be simpler or more easily extensible? If you use it, for now,
please be willing to alter your slideshow code if the feature's
interface is altered.


WHAT DOES IT DO?
----------------

It puts a thumbnail pager on your slideshow similar to this:

http://jquery.malsup.com/cycle/pager2.html


HOW TO DO IT
------------

Include a pager request in the options of your slideshow:

ssc_pager='thum'

By default the thumbnails appear above the slides, but this can be
changed with the ssc_pager_pos option:

ssc_pager_pos='before'  (the default - pager above the slides)
or
ssc_pager_pos='after'   (for the pager below the slides)

The size of the thumbnail images can be altered with the 'ssc_thumx'
and 'ssc_thumy' options, for example:

ssc_thumy=80,ssc_thumx=100

The default values are ssc_thumx=50, ssc_thumy=40. Values are in
pixels, but other units may be used if the argument is quoted:

ssc_thumx='40pt', ssc_thumy='25mm'

Here is an example of a complete slideshow, using a 'dir' element to
provide the slides:

[slideshow: 2, height=340, width=356,timeout=0,speed=300,
 layout=none, order=bottom, ssc_pager='thum', ssc_pager_pos='after',
 ssc_thumy=60,ssc_thumx='80px',
 dir= | tests | | | The nice\, friendly title | The [explanatory\] caption ]


HOW IT WORKS
------------

The thumbnail pager is selected by including the option

 ssc_pager='thum'

and if you examine the slideshow_creator.js file, you will see that
it contains a function called ssc_pagerfn_thum. Suspicious!

To write your own pager, all you have to do is choose a name for it
(for example "snazzy") and write your own javascript function called
ssc_pagerfn_snazzy, using ssc_pagerfn_thum as a guide. Then try it
in a slideshow by including: ssc_pager='snazzy'

Your function can use the existing parameters ssc_thumx, ssc_thumy,
and ssc_pager_pos, and you can also invent any new ones you need. As
long as parameter names start with "ssc_", they will automatically
be passed through to your function, where you can use them to
control your feature however you like. You can also set defaults for
your new parameters in the Slideshow Creator admin "Default Tags"
string. You don't have to do anything to 'activate' your newly
minted options - just make sure they start with "ssc_".

The jquery.malsup.com site might be helpful.

If you come up with something good, please contribute it and get
credit for helping to develop Drupal!
