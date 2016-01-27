The Pirobox Limit module extends the Pirobox module with limiting
and access functionalities.

REQUIREMENTS
------------
Pirobox module, version 7.x-1.0-beta2+

RECOMMENDED
-----------
ImageCache Actions module (drupal.org/project/imagecache_actions).
FileField Paths module (drupal.org/project/filefield_paths).

INSTALLATION
------------
1. Copy the extracted Pirobox Limit folder to modules directory
   or install the module with the UI at admin/modules/install.
2. At admin/build/modules enable the Pirobox Limit module.

ADMINISTRATION
--------------
1. Administer the Pirobox Limit module permissions at admin/people/permissions.
2. If you are use different image styles to display limited images

     - Enable the 'ImageCache Actions' module.

     - Add different image styles at admin/config/media/image-styles
       (See http://drupal.org/node/1388056)

     The different image styles needed on step 4.

3. Configure Pirobox image fields.

   At admin/structure/types/manage/[content type]/fields add a image field.

     Use the tab 'Manage display' and change the format of a image field
     to 'Pirobox'.
     (See http://drupal.org/node/1388036)

     Use the edit icon on the right side of a Pirobox image field.
     (See http://drupal.org/node/1388044)

       - Configure the options 'Gallery limitation'
         and 'Gallery limitation bypass'.
         See LIMITATIONS below.

4. Configure the 'Pirobox Limit' module global settings
   at admin/config/media/pirobox/pirobox-limit.
   (See http://drupal.org/node/1387996)

LIMITATIONS
-----------
'Gallery limitation' and 'Gallery image random'
at the same time is not possible.

The module functionality cannot be used in context with
the entity type taxonomy term.

FILE NAME ANONYMIZING
---------------------
It's a good idea to anonymize the filenames of uploaded images in limited
galleries. This prevents the visitors guess the file name to use them directly
in the browser.

How anonymize image names

  1. Enable the 'FileField Paths' module (drupal.org/project/filefield_paths).
  2. Configure a Pirobox image field at
     admin/structure/types/manage/[content type]/fields/[image field]
     and use the fields 'Image path settings' -> 'Replacement patterns'.

     Use one of the 'Random' options to activate the filename anonymizing.
     (See http://drupal.org/node/1391250)
