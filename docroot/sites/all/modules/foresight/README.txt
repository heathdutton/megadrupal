Foresight
---------

Provides a field formatter to display image fields using the foresight.js
library. Core image effects are able to adjust scale dimensions to account for
device pixel ratio and requested image size.

This means that:
* High resolution images can be served to Retina devices and other devices with
  high pixel ratios.
* Images are dynamically requested at alternate sizes to fit responsive
  layouts. The browser doesn't need to download a large image just to scale it
  down in the layout. The image is requested and generated at the exact size
  required.

For more information on Foresight visit
https://github.com/adamdbradley/foresight.js

Installation
------------

* Download the Foresight JavaScript library from
  https://github.com/adamdbradley/foresight.js/archive/master.zip
* Extract the archive and rename the folder to 'foresight'
* Copy this to your libraries directory e.g. sites/all/libraries

Usage
-----

Select the 'Foresight Image' formatter to display an image field in your content
type's display settings or in a view.

This module affects the behaviour of the following filters:

* image_resize
* image_scale
* image_scale_and_crop

The dimensions you specify for these effects should be for the default size for
a pixel ratio of 1 (non-retina screens). These will automatically be scaled up
to create the variants requested by Foresight.

Alternatives
------------

Responsive images and styles
http://drupal.org/project/resp_img

* Uses cookies and server side logic, rather than dynamic loading client side.
* Picks best fit from a series of preset sizes based on window size rather than
  generating image to exact requested size.

Adaptive Image
http://drupal.org/project/adaptive_image

Another server side / cookie solution based purely on screen size.

Client-side adaptive image
http://drupal.org/project/cs_adaptive_image

Applies alternate image styles based on fixed breakpoints rather than scaling an
existing style.

Borealis
http://drupal.org/project/borealis

Performs a similar function in dynamically replacing responsive and retina
images.

Main differences with Foresight Images:
* Foresight Images offers the option to test bandwidth and only display hi-res
  images on devices with a fast enough connection.
* Applies to all instances of a style, whereas Foresight Images provides a field
  formatter that can be used only where required.

Retina Images
http://drupal.org/project/retina_images

Simple solution to serve Retina resolution images to all devices.

Credits
-------

Module by Graham Bates <hello@grahambates.com>

Foresight library Copyright (c) 2012 Adam Bradley
https://github.com/adamdbradley/foresight.js
