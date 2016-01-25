CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation
 * Roadmap


INTRODUCTION
------------

Current Maintainer: Yannick Leyendecker (http://drupal.org/user/531118)

Gallery Link provides a field formatter for image fields.
You can display images as gallery triggered by a link.


INSTALLATION
------------

1. Download fancybox 1.3.4 from fancybox.net [1]

2. Unzip the archive and put the fancybox folder inside into sites/all/libraries
   (so jquery.fancybox-1.3.4.pack.js is located at 
   sites/all/libraries/fancybox/jquery.fancybox-1.3.4.pack.js)

3. Add an image field to an entity and choose "Gallery Link" on the
   Manage Display site.

4. Set some options and save them.


Optional: When the Jquery Colorpicker module [2] is enabled you will get a
          nice colorpicker for choosing the colors.


[1] http://fancybox.googlecode.com/files/jquery.fancybox-1.3.4.zip
[2] http://drupal.org/project/jquery_colorpicker


ROADMAP
-------

1. Optimize fancybox options settings (dependencies)
