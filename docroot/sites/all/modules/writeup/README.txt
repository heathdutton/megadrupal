
INTRODUCTION
============

This module provides Writeup filter integration for Drupal input formats.

 * Writeup has a superficial similarity with Markdown, and is designed to co-
   exist with HTML. It is also meant to be as human-readable as possible.
 * Writeup is vastly more sophisticated than Markdown or other similar markup
   styles, with variables, functions and a full-blown text processing language.
 * Most markup characters are programmable so that the syntax of Markdown,
   Textile or other systems can be emulated
 * For more information, see see the writeup.org ( writeup.org/ ) website.

CONFIGURATION
=============

1. Install the module
2. Download the Writeup binary from one of:
    o https://writeup.googlecode.com/svn/bin/writeup
    o http://writeup.sourceforge.net
3. Go to admin/writeup and set the path to the binary and to any definitions
   include files.
4. Create an input format using /admin/settings/filters.
    o By default all output from the Writeup filter is also run through
      Drupal’s filter_xss to sanitize it. It is strongly recommended that
      this setting be left checked unless there is some reason to allow scripts
      to be inserted by the user.
5. A custom help message may be created for each format. This enables help for
   special definitions contained in the definitions include file for that
   format.
6. Apply the format to input text, as required.

SUPPORT
=======

If you experience a problem with Writeup Filter or have a problem, file a
request or issue on the Writeup Filter queue at drupal.org/project/issues/
writeup. DO NOT POST IN THE FORUMS. Posting in the issue queues is a direct
line of communication with the module authors.

No guarantee is provided with this software, no matter how critical your
information, module authors are not responsible for damage caused by this
software or obligated in any way to correct problems you may experience.


SPONSORS
========

The Writeup Filter module is sponsored by Crazy Korean Cooking
( crazykoreancooking.com ).

Licensed under the GPL 2.0.
www.gnu.org/licenses/gpl-2.0.txt

