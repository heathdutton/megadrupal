CONTENTS OF THIS FILE
---------------------
   
 * Introduction
 * Requirements
 * Installation
 * Configuration
 * FAQ
 * Maintainers


Introduction
=============
This is a lightweight solution for responsive blocks based on the 
MediaCheck library.
(http://sparkbox.github.io/mediaCheck/) 

This module is intended to aid Developer's and Designer's when 
working on Responsive design.

P.S : Should not be used in a site with lot of blocks.

Requirements
=============
Module functionality is implemented by using MediaCheck JS library 
Library Repository on Github : https://github.com/sparkbox/mediaCheck


Installation
=============
As noted, this module does not include the actual mediaCheck library. 
This should be downloaded or cloned from one of the links above and 
placed in 

 - sites/all/libraries/mediacheck

or somewhere the Libraries API (if present) can find it, eg

 - sites/default/libraries/mediacheck
 - sites/example.com/libraries/mediacheck
 
Once the module is installed and enabled, browse to the Status Report 
page (admin/reports/status) and confirm that the library is found. The 
PHP file should have a pathname that is similar to 

 - sites/all/libraries/mediacheck/mediaCheck.js
 
If you think everything is installed correctly, you may need to clear 
the Drupal caches (admin/config/development/performance). 

Configuration
=============
This module provies an interface to Developer's/ Designer's to select
and confirm if block should be 
displayed for particular device.

[site-url]/admin/config/services/responsive-blocks

Resolutoin for Desktop, Tablet and Mobile can be configured here.


FAQ
====
1. This module makes use of JS plugin . No server side component is required to
   identity the Device.
2. This module doesn't really identifies the Device , but works for different
   Resolutions
 
Maintainers
============
SwapS
saurabh-chugh
