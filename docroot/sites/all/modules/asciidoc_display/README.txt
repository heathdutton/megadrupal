INTRODUCTION
------------

This project contains scripts and a Drupal module that will allow you to use the
Asciidoc documentation formatting system to generate documentation, and then
display it in a Drupal site. It also supports on-line source editing with
preview, and downloading either an updated source file or a unified diff.

You will need to install this module like you would any other Drupal module. But
in order for it to work, depending on which pieces of the functionality you want
to use, there are additional steps. See below.


REQUIREMENTS - GENERAL
----------------------

You will need to have the multi-byte extension enabled in PHP, which is most
likely true anyway.


ASCIIDOC OUTPUT BUILD SCRIPTS
-----------------------------

To use the module to display an AsciiDoc book, you will need to generate some
XHTML output from AsciiDoc source files. For that, you will need several
open-source tools:
- AsciiDoc: http://asciidoc.org/INSTALL.html
- DocBook: http://docbook.org or http://www.dpawson.co.uk/docbook/tools.html


On a Linux machine, you can use one of these commands to install the tools:
  apt-get install asciidoc docbook
  yum install asciidoc docbook

Once you have the tools installed, the "mkoutput.sh" script in the
"sample_scripts" directory is a starting point for making XHTML output suitable
for display in this module. Note that this module does rely on some
customizations in the "bare.xsl" style sheet in order to display the XHTML
output files correctly.

The DocBook XSL stylesheet files can be downloaded from
  http://sourceforge.net/projects/docbook/
[Look around to make sure you are downloading the style sheet files
(docbook-xsl) and not the documentation.] In that download, the "xhtml"
directory has the xsl stylesheets that the xmlto command uses by default to make
its output from the DocBook source. The bare.xsl stylesheet included in this
project contains some overrides; if you are unhappy with the output, you may
want to do more. You might also look at the slightly more refined scripts that
the User Guide project uses, which also make e-book and PDF output:
  https://www.drupal.org/project/user_guide


MODULE CONFIGURATION
--------------------

Once you have the module installed and some bare XHTML output generated from the
scripts, visit the configuration page at
  admin/config/development/asciidoc
and set up a "book". Configuration notes:
- Choose a display URL for each book.
- Point the book to the directory where the HTML output has been generated
  from the scripts. If the book is translated, output from the individual
  translations should be below this in directories 'en', 'de', 'es', etc.
  corresponding to the standard ISO language codes. You can also have one
  level of image directory below the main output directory, with an arbitrary
  name.
- Project information is for informational purposes, to point viewers and
  editors to the project where downloads and source might exist (it just makes
  a link).
- A File an Issue link can also be provided for viewers.
- If you want to enable on-line editing, provide a source directory, which can
  be read-only (and see the section below about additional downloads for
  editing). Like the output directory, this is organized by languages if the
  book is translated. The side-by-side translation editing feature will only
  work if the source file names are the same in each of these subdirectories.

You'll also need to set up View and Edit permissions.

Note: This module only supports file names for source and output files and image
directories that contain upper- and lower-case letters, numbers, and - _ and
. characters. No other characters are allowed.


CACHE
-----

This module makes use of the Drupal cache for efficiency. It should be
invalidating the data when it needs to, but if you ever find that your site
is not behaving as you think it should, use the clear cache button on the
main admin page of this module, or clear the generic Drupal cache.


BLOCKS
------

This module comes with several blocks:
* The "book list" block lists all of your defined books, and can be
  displayed anywhere.
* The "navigation" and "source information" blocks are specific to
  each book, and are only visible on book pages. Navigation is a table of
  contents, and Source information shows information from your book
  configuration, and allows appropriate users to edit the source files.
* The "languages" block is only visible when viewing a translated book, and
  it lists all available translations in a list. It doesn't integrate
  with the Drupal UI translation system.


COPYRIGHT
---------

Your AsciiDoc output can include a copyright notice, which will be displayed at
the bottom of every generated page if it exists.

To make this happen, add lines like this to the AsciiDoc source of your book (it
must include the phrase "Copyright notice" (case insensitive) and it must be a
DocBook pass-through remark):

pass:[<remark>Copyright notice: Copyright 2015 by the individual contributors;
see <xref linkend="copyright" /> for details. Licensed under <ulink
url="https://creativecommons.org/licenses/by-sa/2.0/">CC BY-SA
2.0</ulink></remark>]

This sample will link users to the copyright page to see the details, as well
as make an outside link to the license web site.

You'll need to look at the HTML output files and find the file that contains
this notice; put this file name into the book configuration.


INSTALLATION - EDITING
----------------------

If you want to use the AsciiDoc editor and preview functionality, you will need
to download some additional libraries and files. If you just want to display
AsciiDoc output in your site, you do not need these downloads.

Note that the editor does not affect the Drupal site at all, or edit the source
files directly. It just reads in the source files and lets the user download the
result as a patch or replacement text file. It also has an option to edit
two languages side-by-side, if the book is translated.

NOTE: Some of the downloads, when extracted, have incorrect file
permissions. Make sure all files can be read by the web user!

markItUp!
http://markitup.jaysalvat.com/home/
Download and install so that you have a file
sites/all/libraries/markitup/markitup/jquery.markitup.js
as well as the rest of the files in the download.

AsciiDoctor-Markitup images
https://github.com/lordofthejars/asciidoctor-markitup
(See also:
http://www.lordofthejars.com/2013/07/asciidoc-editor-with-markitup.html )
Download the images from the markitup/sets/asciidoc/images directory and
place them in
sites/all/libraries/asciidoctor_images

You'll also need to copy the js/asciidoc_display.edit.css file and the
js/images/monospace.png file from the AsciiDoc Display project into the
asciidoctor_images folder. Sorry, I couldn't figure out a good way to make this
work otherwise.

AsciiDoctor JS
https://github.com/asciidoctor/asciidoctor.js
(See also: http://asciidoctor.org/ )
Download and install so that you have a file
asciidoctor/dist/ascidoctor-all.min.js
as well as the rest of the files in that directory of the download.


INSTALLATION - DIFF FILES
-------------------------

If you want users who edit files to be able to download patches, you will need
to install the PECL xdiff extension for PHP on the web server where your Drupal
site is hosted. However, since the module only generates generic patch files,
not Git diff files, for most projects it is probably better for them just to
download the updated text file and make a patch using Git.

To install PECL xdiff, see:
  http://php.net/manual/en/install.pecl.php
  https://pecl.php.net/package/xdiff

On a Linux machine, if you already have PECL, in theory you should be able to
type:
  pecl install xdiff
to install it. You will also need to have the "libxdiff" Linux package installed
first. You can learn how This does not seem to be provided by Ubuntu, and I
don't know about other distributions. Try an internet search, or the libxdiff
instructions here:
  http://www.wikidot.org/doc:debian-quickstart
I could not get this to work on Ubuntu, however -- the PECL part failed with
compilation errors.

So, the diff making feature of this module is untested. Use at your own risk!
