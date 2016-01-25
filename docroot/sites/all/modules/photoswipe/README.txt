DESCRIPTION
===========

jQuery-based lightbox library offers very nice mobile browsing features (in
particular swiping to the next picture)!


INSTALLATION
============

1. Download and install the "Libraries API" module: uncompress the "libraries"
folder, copy it to your "modules" directory, and enable it.
http://drupal.org/project/libraries

2. Place the "photoswipe" folder in your "modules" directory (e.g.
sites/all/modules/photoswipe).

3. Install third party PhotoSwipe software
     Download PhotoSwipe 4.x source from PhotoSwipe website
     (e.g. https://github.com/dimsemenov/PhotoSwipe/archive/v4.0.5.zip)
     Unarchive it into your "libraries" directory (e.g. sites/all/libraries).
     You may need to create the "libraries" directory first.
     Rename it to "photoswipe" (lower case).
NB: Relying on libraries module to locate 'photoswipe' folder allows you to place
it in a site specific (e.g. sites/mysite/libraries) or default folder
(e.g. sites/all/libraries). Site-specific versions are selected preferentially.

4. Enable the PhotoSwipe module.


USAGE
=====

1. Multiple images in nodes
After adding an image field to any content type (e.g. 'article'), you can select
'PhotoSwipe: Preset1 to Preset2' as a display mode in Structure >> Content types
>> MyContentType in the tab 'Manage display'. All possible
combinations of image styles are proposed.

2. Multiple images in Views
To use photoswipe in views you must add a custom CSS class called
'photoswipe-gallery'.
Fields >> Content: Image >> Style settings >> check Customize field and label
wrapper HTML >> Wrapper HTML element (leave default) >> check Create a CSS class
>> add as CSS class 'photoswipe-gallery'.
It is NOT recommended to add the CSS class to Advanced >> Other >> CSS class in
a View since there are known issues with the ajax pager.

3. Single image in node
To load a single image in node you must add data-size="widthxheight"
(the exact size of the image) and the class="photoswipe" to display it properly.
e.g.
<a href="/images/test_img.png" class="photoswipe" data-size="640x400">
 <img src="/images/test_img.png" alt="Test Image" />
</a>
It might be needed to load photoswipe assets in case they are not already loaded
To do so just call
photoswipe_load_assets();
Or force them to load on all non-admin pages
variable_set('photoswipe_always_load_non_admin', TRUE);
