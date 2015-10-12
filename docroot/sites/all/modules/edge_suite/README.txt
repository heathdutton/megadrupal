------------------------
Edge Compositions
------------------------
 by Timm Jansen (timm.jansen@gmail.com) (@timmjansen)
 ("ti2m", http://drupal.org/user/73587)


Notes
--------------
 For detailed information and resources check
 - the project page (http://drupal.org/project/edge_suite)
 - Edge Docks (http://edgedocks.com/edge-suite)

 EXPERIMENTAL!!!

 Since Edge Animate itself is pretty new this module is yet not stable and
 should therefore be used with caution and NOT in production.


Description
-----------
 Create animated and interactive content with Adobe Edge and upload these
 compositions to your Drupal website.


Install
-----------
 Install as any common module.
 - Download the module
 - Unpack it
 - Upload it to e.g. sites/all/modules
 - Enable the module in Drupal
 - Go to permissions and assign 'Administer Edge Suite' to the
   appropriate roles. See 'Security'.


Security
-----------
 This module comes with certain security implications: A user with the given
 'Administer Edge Suite compositions' role can upload archives/OAM files to your
 server. These archives will get unpacked and all images, gifs and Javascript
 files will be copied to a separate project folder. The archive itself will be
 copied to a special source folder within the 'private' system folder, which is
 not publicly accessible.
 This is being done so the edge composition can be rebuild from source in case
 of corruption or an update of this module.
 With uploading edge compositions the user is able to deploy random Javascript
 files on your site through the block system (which is also possible when using
 the 'Full HTML Text format' permission).
 Conclusion: ONLY GIVE THIS ROLE TO TRUSTED USERS!


Basic Usage
-----------
 - Create a composition with Edge Animate
 - Publish it with option "Animate Deployment Package", an OAM file will be
   generated
 - Upload the OAM file under:
   "Structure->Edge Suite" (admin/structure/edge_suite)
 - The composition is now being exposed through a block and you can place it on
   the page

   For more advanced usage check the project page


What it does
------------
 For now the module takes care of adjusting all the paths and uses shared
 libraries for all the compositions. More features are already implemented,
 for the actual feature check the project page.


Roadmap
----------
 Trello roadmap: https://trello.com/board/edge-suite/507488990084aba17a421fc7
 Module page: http://drupal.org/project/edge_suite


Issues & Features
----------
In case you are running into problems or have specific feature request,
please post them at http://drupal.org/project/issues/edge_suite
