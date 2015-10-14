About the Module
================

Splash Block module allows users the ability to create "splash" popups by using the Blocks UI. It creates a new region in any theme (called Splash), to which blocks can be added. This allows for admins to pick and choose which splash content shows up on which pages, using Drupal core's block visibility rules.

The time interval between consecutive splash displays, and the width of splash area, can be set on a block-by-block basis, using the Splash Block Settings fieldset on block edit forms.

Differs from similar modules in the following ways:
1. Allows admins to make use of the familiar and flexible blocks administration UI. This allows for beginners and site builders to create any number of splash blocks on the fly, and add them to pages according to Drupal's powerful block visibility rules.
2. Allows for any number of splash blocks to be used concurrently. Some similar modules allow for only 1 splash item to be in use on a site at a time.
3. Does not require an external lightbox/popup library or module, making the module simpler to get started with. An option to use such a library (like Colorbox or Lightbox2) may be added in the future, but will not be required.

Installation Instructions
=========================

1. Install module as usual (make sure Libraries module is installed and sites/*/libraries directory exists).
2. Install jstorage library:
  * If using Drush, the library will be automatically installed for you when enabling the module with drush pm-enable (en).
  * If using Drush, but not enabling with Drush, splash_block module creates the drush sb-get-jstorage (sbg) command, which will download and unzip the library for you.
  * Manual Installation:
    1. Command line:

      `cd /path/to/libraries`

      `mkdir jstorage`
   
      `cd jstorage`
   
      `curl https://raw.githubusercontent.com/andris9/jStorage/master/jstorage.min.js > jstorage.min.js`
      
    2. Manual: download and unpack the Library using the Download ZIP link at https://github.com/andris9/jStorage/tree/master. Unzip to sites/all/libraries (or wherever your libraries directory exists). Rename the unzipped directory to **jstorage**. The final result should be /path/to/libraries/**jstorage/jstorage.min.js**.