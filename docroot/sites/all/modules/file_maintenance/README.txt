
File maintenance
============

Requires:          Drupal 7.x
Project page:      http://drupal.org/sandbox/jox/1215500
Author/maintainer: Jonas Petersen <jox axe mindfloaters dude com>


-- This file is incomplete yet. --


Overview
--------
File maintenance is a tool (for Drupal 7) to administer managed and 
unmanaged files. Directories can be browsed and files examined. 
Differences between the database and the filesystem will be pointed out. 
Operations such as copying, moving, deleting, (batch) renaming and 
modifying files and directories are possible (not yet).


Features
--------
     * Uses Ajax and jQuery for a user-friendly UI, aiming to provide 
       optimum usability.
     * Shows disk space usage and number of files. Both for single 
       folders and recursively including subfolders.
     * Highlights conflicts in the directory tree and file list. 
       Conflicts are files that exists in the database but not in the 
       filesystem or vice versa.
     * Certain directories can be defined as temporary directories. 
       These are for generated files that might not be registered in
       file_managed.
     * The visibility of the columns in the file list can toggled.
     * ...


Planned Features
----------------
     * Find duplicates and optionally merge uses.
     * Operations on files and directories (copying, moving, deleting, 
       (batch) renaming, ...).
     * ...


Usage
-----
File maintenance provides a file browser UI with a directory tree and a file 
list panel. Both directory and file list are scrollable. The bar between 
them can be dragged to resize the panels.

Files and directories are color-coded:

     * Green: existing in database and filesystem.
     * Blue: existing only in filesystem.
     * Red: existing only in database.

Icons indicate problems in the directory tree.


Development Status
------------------
File maintenance is being developed for Drupal 7. It is still work in 
progress, but has reached a state where it can be tested and even be 
useful. No operations can be performed yet but the file browser UI is 
functional and informative.


Known issues
------------
There is a few.

     * There is display problems in Internet Explorer.
     * ...

