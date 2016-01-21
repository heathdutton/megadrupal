Views jQFX: ImageFlow
----------------------------
This module is a Views jQFX addon that integrates the ImageFlow plugin with views.
The Views jQFX module is a dependency.

Installation
----------------------------
1) If you have not already done so, get and install the Views jQFX module from the drupal project page.
2) Place this module in your modules directory and enable it at admin/modules.
   It will appear in the views section.
3) Download the ImageFlow plugin and place it in your sites/all/libraries directory.
   To get the plugin to go the project page (http://imageflow.finnrudolph.de/).
   The final directory structure should look like:
     'sites/all/libraries/ImageFlow/imageflow.js'
     'sites/all/libraries/ImageFlow/imageflow.packed.js'
     'sites/all/libraries/ImageFlow/imageflow.css'
     'sites/all/libraries/ImageFlow/imageflow.packed.css'
     'sites/all/libraries/ImageFlow/style.css'
     'sites/all/libraries/ImageFlow/img/'
     etc...
   Open the imageflow.css file and remove the first line that reads '@charset "utf-8";'. This line will break CSS aggregation.
4) Create or edit your content type.
   Captions are created from the alt and title tag attributes of the images in the content type you wish to display.
   If you want to display captions you must have the ability to add these attributes to you image fields.
   To enable image attributes navigate to: admin --> structure --> content types
   Select 'manage fields' in the content type(s) that will be displayed in the ImageFlow (or create a new content type).
   Edit the image field that you want to display and be sure that the 'alt' and 'title' boxes are checked.
   This will provide a textfield for adding information to uploaded images when creating or editing a node.
   If this is a new content type an image field will need to be created for it.
5) Create the nodes that you wish to display under: admin --> content --> add
6) Create a node view. If you are new to views you may want to find a tutorial for it. There are many out there.
   Only the images in the node view will be displayed in ImageFlow.
   Add the the image fields that you wish to display under the 'fields' section.
   Add your filters, arguments, etc for the content.
   Create a new block or page display for your view.
   Blocks can be added to page regions in: admin --> structure --> blocks
   Pages must have a path (ie 'imageflow').
7) In the settings of your view, click on the link next to the 'Style' label (usually will say 'unformatted').
   Choose the style jQFX and hit the update button.
   This will give you a drop down menu from which to choose your jQFX Settings.
   For the ImageFlow display, select 'ImageFlow' under the 'FX Style'.
   The menu will provide your imageflow display options.

The Full ImageFlow Options List. A thorough list of examples for these option can be seen at http://finnrudolph.de/ImageFlow/Examples
=====================================================================================================================================
Name               Type         Default      Description
-------------------------------------------------------------------------------------------------------------------------------------								
animationSpeed     Number       50           Animation speed in ms 	
aspectRatio        Number       1.964        Aspect ratio of the ImageFlow container (width divided by height)
buttons            Boolean      false        Toggle navigation buttons 								
captions           Boolean      true         Disables / enables the captions 							
circular           Boolean      false        Toggle circular rotation 								
glideToStartID     Boolean      true         Toggle glide animation to start ID 				
imageCursor        String       'default'    Cursor type for the images 				
ImageFlowID        String       'imageflow'  Unique id of the ImageFlow container 			
imageFocusM        Number       1.0          Multiplicator for the focussed image size in percent 			
imageFocusMax      Number       4            Maximum number of images on each side of the focussed one
imagePath          String       ''           Path to the images relative to the reflect_.php script 	
imageScaling       Boolean      true         Toggle image scaling 								
imagesHeight       Number       0.67         Height of the images div container in percent 			
imagesM            Number       1.0          Multiplicator for all images in percent 			
onClick            Function     function() { document.location = this.url; }  Function that is called onclick of the focussed image 
opacity            Boolean      false        Disables / enables image opacity 						
opacityArray       Array of Numbers  [10,8,6,4,2]  Image opacity range - first value is for the focussed image (0 = 100% opacity, 10 = 0% opacity)
percentLandscape   Number       118          Scale landscape format 								
percentOther       Number       100          Scale portrait and square format 				
preloadImages      Boolean      true         Disables / enables the loading bar and image preloading 	
reflections        Boolean      true         Disables / enables the reflections 				
reflectionGET      String       ''           Passes variables via the GET method to the reflect2.php and reflect3.php script
reflectionP        Number       0.5          Reflection height in % of the source image (1.0 = 100%, 0 = 0%) 
reflectionPNG      Boolean      false        Switches from the reflect2.php to the reflect3.php for PNG transparency support
reflectPath        String       ''           Path to the reflect_.php script 	
scrollbarP         Number       0.6          Width of the scrollbar in percent 	
slider             Boolean      true         Disables / enables the scrollbar 				
sliderCursor       String       'e-resize'   Cursor type for the slider 						
sliderWidth        Number       14           Width of the slider in px 	
slideshow          Bool         false        Toggle slideshow 									
slideshowSpeed     Number       1500         Time between slides in ms 				
slideshowAutoplay  Bool         false        Toggle automatic slideshow play on startup 			
startID            Number       1            Glide to this image number on startup 	
startAnimation     Boolean      false        Animate images moving in from the right on startup 	
xStep              Number       150          Step width on the x-axis in px 		



These variables can be passed to the reflect script via reflectionGET
=========================================================================

Reflect 2 options for jpg images
-------------------------------------------------------------------------
img         required  The source image to reflect
height      optional  Height of the reflection (% or pixel value)
bgc         optional  Background colour to fade into, default = #000000
fade_start  optional  Start the alpha fade from whch value? (% value)
fade_end    optional  End the alpha fade from whch value? (% value)
jpeg        optional  Output will be JPEG at 'param' quality (default 80)
cache       optional  Save reflection image to the cache? (boolean)

Reflect 3 options for png images
-------------------------------------------------------------------------
img         required  The source image to reflect
height      optional  Height of the reflection (% or pixel value)
fade_start  optional  Start the alpha fade from whch value? (% value)
fade_end    optional  End the alpha fade from whch value? (% value)
cache       optional  Save reflection image to the cache? (boolean)
tint        optional  Tint the reflection with this colour (hex)

Please post any comments, questions, bugs or feature requests in the issues queue of the Views jQFX: ImageFlow project page on Drupal
