
PageFlip: Book, Magazine, Comic Viewer module - http://drupal.org/project/pageflip
==================================================================================

DESCRIPTION
-----------
PageFlip simulates "flipping through the pages" of books, magazines, comics, flyers, or other "book"-like content.

PageFlip includes two viewers: PageFlip MegaZine3 Viewer and PageFlip HTML/JavaScript Viewer. The former uses the open-source Flash-based MegaZine3 page flipping engine while the latter is a custom viewer written with jQuery.

Content is created using three content types:

* PageFlip Page: Represents a page and allows uploading two resolutions of each page.
* PageFlip Chapter: Represents a chapter or section and acts as a container for PageFlip Pages.
* PageFlip Book: Represents one full document (e.g. a comic book). Defines the front cover page, inside-front-cover pages, and chapters that comprise the book.

REQUIREMENTS
------------
* Drupal 6.x
* CCK
* ImageField

INSTALLATION
------------
1.  To install the module copy the 'pageflip' folder to your
    sites/all/modules directory.

2.  Install the necessary libraries.

For the MegaZine3 Viewer, download and extract the library
from http://www.megazine3.de/ into the sites/all/libraries directory
in a subfolder called 'mz3'.

The path to the Flash file should be:

sites/all/libraries/mz3/megazine/megazine.swf

For the HTML Viewer, download and extract the required jQuery
libraries:

1. jQuery Cycle Plugin - http://malsup.com/jquery/cycle/
   Download it and copy jquery.cycle.all.min.js so that it resides here:
   sites/all/libraries/jquery.cycle/jquery.cycle.all.min.js

2. jQuery hashchange event plugin - http://benalman.com/projects/jquery-hashchange-plugin/
   Download it from that page and copy jquery.ba-hashchange.min.js so that it resides here:
   sites/all/libraries/jquery.ba-hashchange/jquery.ba-hashchange.min.js

3. Go to admin/build/modules. Enable the main module and at least one of the
   viewers. It is found in the Others section.

Read more about installing modules at http://drupal.org/node/70151

UPGRADING
---------
Any updates should be automatic. Just remember to run update.php!

FEATURES
--------
General features:

* Pages are arranged so that chapters always start on the right-hand side.
* If you don't want or need the chapter functionality, you can simply ignore
  leave the "Chapters" field of your PageFlip Book blank and insert all your
  pages (PageFlip Page nodes) into the "Opening Pages" field.

The HTML/JavaScript viewer

* fades between page pairs when "flipping" pages--not quite as fancy as the
  MegaZine viewer, but more fun than a page refresh
* has a drop-down "Front cover/chapter 1/chapter 2..." selector that allows
  quickly navigating through the book being viewed
* optionally integrates with ShareThis (provides a place to paste your
  ShareThis widget code on its settings page)
* optionally integrates the vote_up_down module for book voting

The MegaZine3 Flash viewer

* provides a setting to customize options sent to MegaZine in the XML <book> tag
* provides a setting to customize options in each XML <page> tag
* allows overriding XML generation on any page with completely customized XML

HOWTO / FAQ
-----------

1 - Creating a book.

TODO

AUTHORS
-------
adamdicarlo - http://drupal.org/user/100783.

The following users have contributed to this module:

Alan D. - http://drupal.org/user/198838.

REFERENCES
----------
[1] http://drupal.org/project/pageflip
[2] http://www.megazine3.de/
[3] http://www.megazine3.de/doc/MegaZine3

