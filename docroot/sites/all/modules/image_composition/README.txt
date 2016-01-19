# Magazine Layout for CCK Imagefield
================================

*An alternative renderer for displaying imagefield images attached to nodes*

Wrapper around the "magazine layout" algorithm described by
Harvey Kane http://www.alistapart.com/articles/magazinelayout/
Also at http://www.ragepank.com/magazine-layouts/


Distributed with a one-off LGPL PHP class.
This is not an active 'Library' of any type and was published 
in an online magazine as a proof-of-concept experiment. 
Therefore the full code is distributed here rather than as 
a separate downloadable dependency.
 
2012-02
This is not yet very Drupally - and may be unsafe around private files.
Also, has no caching. TODO! compare with imagecrop and similar.

Can be refactored to add more intelligence, but right now it does actually just work!
 
@author Harvey Kane http://www.alistapart.com/articles/magazinelayout/
@author dman http://coders.co.nz/
@version 2012-02

## Setup
-------------------------

Choose "Magazine Layout" in the "Display fields" administration for your content type.
That's it.


## Limitations
-------------------------

See the original article for background.
  http://www.ragepank.com/magazine-layouts/

Currently it works best with up to 8 images.

No UI is yet provided to adjust parameters such as width or padding, though they 
are easily tweaked directly in the code. (for now)

2012-02 NO CACHING! 
Early code resizes images on the fly.
This needs to be updated before using it for more than a demo.

Desired: 
* More Drupally theming. 
* Link and lightbox integration.
* Beyond CCK imagefield to real image-as-nodes gallery building - so: Views integration.

