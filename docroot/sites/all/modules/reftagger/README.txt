Module: RefTagger
Author: Nathan Rambeck <www.rambeck.com>


Description
===========
Add the Logos RefTagger javascript to your site and allows configuration in the admin


Installation
============
* Copy the 'reftagger' module directory in to your Drupal
sites/all/modules directory as usual.


Usage
=====
RefTagger will work out-of-the-box by just enabling the module.

You can configure RefTagger for your site by visiting the site configuration
page for RefTagger. This will allow you to choose a default Bible version,
change Libronix settings, add exclusion tags and classes, and override the CSS
for the RefTagger tooltip box.

A block is provided that will allow your site visitors to change the default
Bible version and Libronix settings to their own preferences. By editing the
block settings you can change the theme of this user control panel. To implement
your own custom styling of the control panel, copy the contents of the file
RefTaggerControlPanel.css into your theme stylesheet for editing. Then choose
the 'Custom' theme option in the block settings.
