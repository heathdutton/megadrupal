A module automating the implementation of Unveil.js

To install:
Download the module, and the unveil js library from the git repository here:
https://github.com/luis-almeida/unveil/.
The unveil js libarary should be located at 
sites/*/libraries/unveil/jquery.unveil.min.js

To enable:
The unveil library is enabled by default, when you install the module.

To disable:
To disable unveil uncheck admin/config/development/performance, where 
a checkbox for enabling unveil.js is available under bandwidth options. 
This will disable unveil.js for the whole site. 

Exclude single image:
To exclude a single image, create a preprocess hook for it, where you 
put any business logic for identifying the image you want to exlude, 
and set $vars['unveil_exclude'] = TRUE. 
See: unveil.example.inc for an example of how to implement.

Unveiled distance:
The unveiled distance attribute is used to define how many pixels from 
the viewport, the image should be unveiled at. The default is 200, meaning 
that anything within 200 pixels of the viewport is unveiled.

To test:
To see how this works, open Chrome, then turn on Developer tools 
(view > developer > developer tools), and use the Network tab to see 
exactly when your images are being loaded. Scroll along the site and 
notice that your images are being loaded only when needed.
