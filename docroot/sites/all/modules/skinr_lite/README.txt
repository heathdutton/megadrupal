Skinr Lite Module
-----------------

This module is an extremely pared-down version of the Skinr module.

If you are familiar with the Skinr API, then this module implements the Skinr
API version 2--only for skin plugins available to the html theme hook and only
if they are enabled by default for the current theme--as theme settings.

If you are not familiar with the Skinr API, this module will enable you to use  
some skins that your themer has defined simply by adjusting the theme settings.

If you are a theme designer working on a theme that could be used across many 
sites, the Skinr API will help you to create a set of options that will show up
as css classes when a page is viewed.  For simple use cases, this module can
translate the site-wide options into theme settings.  More complex use cases
which would require the full Skinr module include, among others: skinning 
individual blocks or views, changing the skin with logic from context or panels,
and adding custom css classes to any element.


Installation and Configuration
------------------------------

Install the module as usual, then visit the settings page for the Skinr-enabled
theme in order to choose your skin options.


Theme Development
-----------------

The full Skinr API allows for skins to be defined by modules, by themes in a
single include file, or by themes as a series of plugins.  If the full Skinr
module is somewhere in your code base--whether or not it is enabled--you will be
able to see qualifying skins from all those sources as well.  If only Skinr Lite
is in your code base, you will only be able to see skins that have been defined 
by the theme using the single include file method.

See http://skinr.org/ for documentation on how to define skins for your theme.


Caveats
-------

Using this module alongside the full Skinr module seems likely to cause some
confusion.  There's no upgrade path either, although manually copying the few
settings that most themes will implement in this way should take little time.
