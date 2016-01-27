Module page: http://drupal.org/sandbox/luxpaparazzi/1301730

This project adds an organizational layer to drupal,
making it easier for managing large-scale websites as photo portals and
document management.

Starting with a physical directory structure managed by the module,
you may add nodes, files and images to different directories
and control fine-grained access-control lists.

Current issues:
 - When uploading multiple files with the muti-file-upload mask, nothing will
   actually be uploaded, when selecting 1 file it works as supposed.
   The DND-functionality works fine.
 - Deleting directories currently does not work.
 - "Directory based access control" and "Field to directory" 
   are actually untested.
  
*******************************************************************************
Installation:

* Enable the following modules:
    - "Directory based organisational layer"
    - "Directory based image"

* Optionally enable one of the slideshow sub-modules.

* Go to odir administration menus (admin/config/odir)
  for setting the different options, especially enter the local directory
  which you would like to use with the module. If it does not exist
  create it and check that the webserver has write permissions to it.
  (Permissions should be the same as for the files directory of drupal.)

  The default path is /tmp/odir_files and is probably ok, for evaluation
  and testing purposes on Unix-like machines.

* Optionally go to admin/odir/settings_odir_image for choosing a slideshow mode.

* Go to admin/structure/block enable the following blocks: 
    - "Subdirectories"
    - "Directory operations"

* If no odir access control module is installed, you must set permissions
  for "Directory based organisational layer" globally in
  admin/people/permissions.
  Those permissions can be overidden per directory by the odir_acl module.


What you can do from here:

* Create directories and subdirectories, you have 2 possiblities:
    - Use the functionality in the activated blocks
    - Create directories physically on disk as with FTP or a file browser.
* Add files into these directories, you have 3 possibilities:
    - Per multi-file-selection utility
    - Per Drag n Drop into the "Folder operations" block
      (tested only with Firefox)
    - Add files physically to disk as with FTP or a file browser.
* Images are displayed as clickable thumbnails, enabling shlideshows.
* Non-image files will be displayed in a block and may be downloaded.
