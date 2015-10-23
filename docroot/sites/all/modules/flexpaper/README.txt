The Flexpaper module provides the formatter for File field which is used for
showing pdf files using Devaldi FlexPaper pdf reader.
 - http://flexpaper.devaldi.com

-----------------------------------------------------------------------------
VERSION INFO
-----------------------------------------------------------------------------
Version 2.x supports both Flexpaper Zine version and Flexpaper classic viewer
(including adaptive UI and GPL versions).

Old 1.x versions support GPL version only.

-----------------------------------------------------------------------------
ABOUT THIS MODULE
-----------------------------------------------------------------------------
This module provides field formatter for File fields. This formatter serves
for displaying PDF files in beautiful adaptive flipbook, using Devaldi Flexpaper
library. You can see demos here http://flexpaper.devaldi.com/demo.jsp

Commercial versions of Flexpaper viewer supports 3 ways of displaying: html5,
flash and html4. GPL version works only in flash mode. Adaptive UI maybe used
only with html5/html4 modes, so GPL package doesn't provide this opportunity.

HTML5 version requires nothing except pdf file itself. Flash mode uses specially
prepared .swf files to operate. And html4 needs .png files.
This module takes care of it and prepares all needed files using pdf2swf and
swfrender tools which are parts of Swftools utilities collection (see the installation
guide). In order to standardize the module work, we require Swftools to be
installed on your server even though you may need only html5 version,

-----------------------------------------------------------------------------
REQUIREMENTS
-----------------------------------------------------------------------------
1. Swftools should be installed on your server, and pdf2swf tool should be
available by command line.
2. To provide correct way to search the text in the split mode your server
should also have pdf2json tool installed and available by command line.
3. Your server should support exec() and shell_exec() to run pdf2swf and
pdf2json commands.

-----------------------------------------------------------------------------
INSTALLATION
-----------------------------------------------------------------------------
1. Install Swftools. See how to do it here
http://wiki.swftools.org/wiki/Installation.
2. Install pdf2json tool. See http://code.google.com/p/pdf2json.
3. Install and enable Libraries API module. https://drupal.org/project/libraries
4. Create flexpaper folder in the libraries directory. Download needed Flexpaper
webpackage version from your account download page (in case of commercial version)
or GPL version from here http://flexpaper.devaldi.com/download. There are many
folders in the package. You need only js, css, locale folders and FlexPaperViewer.swf file.
Copy it into this flexpaper folder.
5. Enable Flexpaper module.
6. Check admin/config/flexpaper and set proper paths to pdf2swf, swfrender, pdf2json
tools. You should also set up your license key in there (or may leave it empty in case of
GPL version)
7. (Optional) Check permissions page to allow users to generate SWF files on
first node view.
