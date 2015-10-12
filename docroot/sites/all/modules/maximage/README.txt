CONTENTS OF THIS FILE
---------------------
 * Introduction
 * Requirements
 * Installation
 * Configuration
 * How to use it
 * Troubleshooting
 * FAQ
 * Maintainers

INTRODUCTION
------------
MaxImage 2.0 is a powerful jQuery plugin written by Aaron Vanderzwan.
This simple Drupal module makes a new content type 'Maximage' that allows
inserting images and assign them to pages.
The contents of type Maximage are selected to compose the Maximage slideshow.
You can also change the path to Javascript and css libraries and
set the MaxImage's customization.

The injected code has the following pattern:
<div id="maximage">
<div>
[your 'image' Maximage content']
</div>
<div class="maximage-text">
[your 'body' Maximage content']
</div>
</div>

REQUIREMENTS
------------
This module requires the following modules:
 * jQuery Dollar https://drupal.org/project/jquery_dollar
 * Image_field_caption https://drupal.org/project/image_field_caption
 * Libraries https://drupal.org/project/libraries

INSTALLATION
------------
 * Install as you would normally install a contributed drupal module. See:
  https://drupal.org/documentation/install/modules-themes/modules-7
  for further information.
 * Download:
   jquery.maximage.js
   [http://blog.aaronvanderzwan.com/2012/07/maximage-2-0/#download]
 
   jquery.cycle.all.js
   [http://jquery.malsup.com/cycle/download.html]

   Notice, you need only the jquery.maximage.js file
   and jquery.cycle.js, not the entire folder.

   Put them into a 'maximage' and 'cycle' folder under 
   your libraries path:
   sites/all/libraries directory or
   profiles/[yourprofilename]/libraries or
   sites/example.com/libraries if you have a multi-site installation

   see https://drupal.org/node/1440066 for more details.

CONFIGURATION
-------------
 * go to admin/config/maximage and check/change the default path
   for css and js plugins. Change them to the correct path
   (relative to Drupal root) where you eventually moved css and libraries
 * set the permissions and give access to the selected roles

HOW TO USE IT
-------------
Go to Add content and create a new Maximage item.
Add images and text captions.
In "Path" add the pages (on multiple lines) where the Maximage has to be shown.
<front> and metachars * allowed.

 ** CUSTOMIZATION OF Maximage jQuery **
 If you need customize the Maximage jQuery function use the textarea 
"Maximage customization" where you can put the jQuery code: i.e.:

 $('#maximage').maximage({
   cycleOptions: {
     fx: 'fade',
     speed: 1000,
     timeout: 10000
   },
 });

Notice. Don't wrap the code with jQuery $(function(){ ... });

 ** CUSTOMIZATION OF HTML CODE **
 If you need to add some HTML code for the customization of Maximage you can
 use the "Maximage HTML customization" like this:

 <div class="curtains">
  <span class="top"></span>
  <span class="right"></span>
  <span class="bottom"></span>
  <span class="left"></span>
 </div>

MULTI-LANGUAGE
-------------
Maximage also support multi-language.


TROUBLESHOOTING
---------------
* Please refer to https://drupal.org/sandbox/briganzia/2225575

FAQ
---
Q: If I have to customize the Maximage view (i.e. insert arrows, thumbnail,
   etc..., have I to change the HTML template?
A: Not necessarily. This module provide a way
   to add fine customization Javascript and HTML through Maximage item.
   See more examples on http://www.aaronvanderzwan.com/maximage/

Q: How can I have a text for each slideshow image?
A: Add a "caption" to image. The standard display position is bottom-right. 
   Change ii through css.

Q: How can I have only one text for the slideshow?
A: Add a text into the "body" field of Maximage. The standard display for this
   text is top-right. Change it as needed through css.

Q: Why I can't see background images?
A: Check if your theme has a layer hiding background. 
   Check the access maximage permissions under people/permissions.
   Check the maximage content path.

Q: Can maximage run with multi-language?
A: Yes. If you add multi-language settings to maximage content-type you
   can display the maximage backgrounds depending on the language
   you've selected.

MAINTAINERS
-----------
* Please refer to https://drupal.org/sandbox/briganzia/2225575 
  - briganzia@gmail.com
