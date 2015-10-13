About
=====
Integrates the OKVideo library into Drupal.

About OKVideo
----------------

Library available at https://github.com/okfocus/okvideo

OKVideo is a jQuery plugin that allows for YouTube or Vimeo videos to be used
as full-screen backgrounds on webpages. OKVideo aims to be customizable while
making some basic decisions about how the plugin should control video. When
using OKVideo, all videos will be served from Vimeo or YouTube based on a
number of variables like browser, device, bandwidth, etc.

Installation
============

Dependencies
------------

- [Libraries API 2.x](http://drupal.org/project/libraries)
- [OKVideo Library](https://github.com/okfocus/okvideo)

Tasks
-----

1. Download the OKVideo library from https://github.
com/okfocus/okvideo/archive/master.zip
2. Unzip the file and rename the folder to "okvideo" (pay attention to the
case of the letters)
3. Put the folder in a libraries directory
    - Ex: sites/all/libraries
4. The following files are required (last file is required for javascript 
debugging)
    - src/okvideo.min.js
    - src/okvideo.js
5. Ensure you have a valid path similar to this one for all files
    - Ex: sites/all/libraries/okvideo/okvideo.min.js


Usage
======

1. Go to the OKVideo admin page (/admin/config/media/okvideo).
2. Enter the URL or the ID of the Vimeo or YouTube video. The OKVideo
plugin will automatically detect which one is being used.
3. Configure the settings and set your visibility paths.
4. For the approved paths, a video will be applied to the background.

Option Sets
-----------
More info to come later.

Debugging
---------

You can toggle the development version of the library in the administrative
settings page. This will load the unminified version of the library. Uncheck
this when moving to a production site to load the smaller minified version.

External Links
==============

- [Wiki Documentation for OKVideo](https://github.com/okfocus/okvideo)
