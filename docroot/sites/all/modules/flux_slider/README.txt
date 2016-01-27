********************************************************************
                     D R U P A L    M O D U L E
********************************************************************
Name: Flux Slider
Author: Ray Nimmo <ray at junglecreative dot com>
Drupal: 7
********************************************************************

DESCRIPTION:

Module for integrating the Flux Slider JavaScript library and controlling its
settings through an administration interface.

The Flux Library must be installed at /sites/all/libraries/flux-slider/flux.js

The module creates a block which can be positioned within the main blocks
configuration page.

Currently only supporting 4 images to be used within the slider along with 4
captions to appear with the transition. The images are currently served from
the modules /images/ directory but the image path can be changed to anything
that is within the public files folders.

Available transitions:
Note that some of the advanced 3D CSS transitions are not supported by
Internet Explorer prior to version 10.

2D: bars, blinds, blocks, blocks2, concentric, dissolve,
slide, warp, zip
3D: bars3D, blinds3D, cube, tiles3D, turn
Available Controls
Slider control features available through the administration panel are;

transition type
transition autoplay
transition delay
transition pagination controls display
transition controls display
transition captions
transition width
transition height
Dependencies
Drupal 7 Dependencies:
Libraries API
Block (included with Drupal core)

Drupal 6 Dependencies:
Libraries API
Block (included with Drupal core)
jQuery Multi
A copy of jQuery later than version 1.4.4
********************************************************************

FLUX LIBRARY INSTALLATION
To download the Flux Library go to
https://github.com/joelambert/Flux-Slider

Or get it directly from:
http://www.joelambert.co.uk/flux/js/flux.js
or:
http://www.joelambert.co.uk/flux/js/flux.min.js

For further information see http://www.joelambert.co.uk/flux and
http://blog.joelambert.co.uk/2011/05/05/ -
 - flux-slider-css3-animation-based-image-transitions/

Current version as of 29-12-2012 is Flux Slider v1.4.4
Copyright 2011, Joe Lambert.
Free to use under the MIT license.
http://www.opensource.org/licenses/mit-license.php

********************************************************************

DRUPAL 7 MODULE INSTALLATION
1. You must install the Libraries API before enabling this module.

2. Download the Flux Slider library from one of the locations shown above and
upload it to /sites/all/libraries/flux-slider/flux.js
(If the Flux Slider library is not at this location an error message will
appear telling you to consult the README.txt file)

3. Place the entire Flux Slider directory into your Drupal modules directory 
(normally sites/default/modules).

4. Enable the module by navigating to:

Admin > Modules

5. If you want anyone besides the administrative user to be able to configure
the Flux Slider (usually a bad idea), they must be given the 
"administer flux slider" access permission:

Admin > People > Permissions

When the module is enabled and the user has the "administer flux slider"
permission, a "Flux Slider" menu should appear under
Admin > Structure in the menu system.

********************************************************************

Future plans;
Add user upload for adding/replacing images
Add support for more images to be contained within the slider


********************************************************************