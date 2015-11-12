********************************************************************
                J Q U E R Y   B O O K L E T   M O D U L E
********************************************************************
Original Author: Varun Mishra
Current Maintainers: Varun Mishra
Email: varunmishra2006@gmail.com

********************************************************************
DESCRIPTION:

   jQuery Booklet module allows you to create small booklets of pages
   using jquery booklet plugin. The booklet pages can be nodes of any 
   any content type. You can select which nodes will be used as booklet 
   pages. You can also select the view mode of nodes to be used in booklet.
   It allows you to upload and use front cover and back cover for your
   booklet. The format of front and back cover can be png, jpg, jpeg and gif.
   It provide an easy interface to manage display order of the pages. 
   It also allows you to enter url alias for the booklet if path module is
   installed. This module also provide views integration. This module has 
   dependency on jquery_update and libraries module. 

FEATURES:

   1) Views Integration.
   2) Url alias support

********************************************************************

INSTALLATION:

1. Place the entire jquery_booklet directory into sites modules directory
  (eg sites/all/modules).

2. Enable this module by navigating to:

     Administration > Modules

3) This module have dependency on jquery_update and libraries module.

4) Download jQuery Booklet Library from http://builtbywill.com/code/booklet/ . 
   Copy booklet directory from jquery.booklet.1.x.x and paste it 
   in the sites/all/libraries directory and rename (jquery.booklet.1.x.x.css or 
   jquery.booklet.latest.css) to jquery.booklet.css, 
   (jquery.booklet.1.x.x.min.js or jquery.booklet.latest.min.js) to 
   jquery.booklet.min.js and jquery.easing.1.x.js to jquery.easing.js in the 
   booklet directory. The plugins files should be available at a path like 
   sites/all/libraries/booklet/jquery.booklet.css, 
   sites/all/libraries/booklet/jquery.booklet.min.js and 
   sites/all/libraries/booklet/jquery.easing.js       

5) Please read the step by step instructions as an example to use this
   module below:-

a) Install the module. 

b) Go to admin/people/permissions#module-jquery_booklet page and set which users
   you would like to have the permission to view and administer booklets.

c) Go to admin/config/development/jquery_update and select jquery version 
   greater than or equal to 1.7 and save it.

d) Go to admin/structure/jquery-booklets. Click on settings tab 
   (admin/structure/jquery-booklets/settings) .  Select the content type which 
   you want to be use as booklet pages. Select view mode of the node. Save the 
   configuration. 

e) Go to admin/structure/jquery-booklets. Click on Add Booklets link. Add new 
   booklet. 

f) You can go to http://builtbywill.com/code/booklet/ to learn about advance 
   settings options used in Advance Setting box.
