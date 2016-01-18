Drupal Social Share Privacy is packaged for download at http://drupal.org/project/socialshareprivacy.

CHANGELOG
*********

v 7.x-1.1
- added this README



INSTALLATION
************

Normally install this module into your drupal.
One more step is required, as the jQuery Plugin is not packaged in this module.

So download it from here: http://www.heise.de/extras/socialshareprivacy/
(choose the .zip or .tar.gz file)
http://www.heise.de/extras/socialshareprivacy/jquery.socialshareprivacy.zip
http://www.heise.de/extras/socialshareprivacy/jquery.socialshareprivacy.tar.gz

Then extract this file in the folder sites/all/libraries/ of your installtion.
You'll endup having a seperate folder named socialshareprivacy,
where (at least) these two files have to exists:
jquery.socialshareprivacy.js
jquery.socialshareprivacy.min.js
plus the another folder socialshareprivacy (again),  with the
socialshareprivacy.css
file and the images folder.

The first socialshareprivacy folder can be named as prefered,
this is setable in this module's configuration, here: admin/config/system/socialshareprivacy



CONFIGURATION
*************

Besides the library location there are other configuration options.
F.e. the apperance of a media (facebook, twitter, google).
The Perma Option means the checkboxes to make the deciscion to keep one media always on.

I recommend to go to the settingspage and hit at least save once ..

POSITIONING
***********

There is automatically a block created, wich can be used for positioning the HTML.