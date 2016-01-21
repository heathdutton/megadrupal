CONTENTS OF THIS FILE
---------------------
* Introduction
* Installation
* Configuration
* FAQ
* Examples
* Maintainers

INTRODUCTION
------------
jQuery custom scrollbar is a jQuery plugin made by Maciej Zubala

This module provides configuration for the plugin through Drupal's 
administration interface.

"jQuery Custom Scrollbar is a plugin that lets you add fully customizable 
scrollbars to your sites instead of browser's default scrollbars. When using 
the plugin you can apply any css styles you want to your scrollbars. 

Features include: vertical and horizontal scrollbars you can style your own 
way, scrolling by mouse dragging, mouse wheel, keyboard – just as you would 
with native browser scrollbar, touch scrolling on mobile devices 
(Android, iPhone and iPad), a couple predefined skins showing you how to style 
scrollbars, simple api that lets you scroll programmatically. "

Official jQuery plugin page: http://plugins.jquery.com/custom-scrollbar/

INSTALLATION
------------
Download the plugin from https://github.com/mzubala/jquery-custom-scrollbar,
unpack it then move it to sites/all/library/jquery-custom-scrollbar.

The module expect these files to work properly:
 * library/jquery-custom-scrollbar/jquery.custom-scrollbar.js
 * library/jquery-custom-scrollbar/jquery.custom-scrollbar.css

CONFIGURATION
-------------
Go to /admin/config/user-interface/jquery-custom-scrollbar and set pages on 
which the js will be loaded and choose selectors to apply scrollbar.

You can specify "*" to load custom scrollbar on all pages.

Optional: if you want a specific, different behavior, you can call:

  drupal_add_library('jquery_custom_scrollbar', 'jquery_custom_scrollbar');

the two lib files mentionned in the installation will be loaded on request.

FAQ
---

I used "body" as the css selector, the page goes blank, is it broken ?

IMPORTANT: width and height must be set on the container.
The plugin does not work on elements with dynamic heights,
for instance body most of the time.

To reset your variables set from the admin interface, call:
  drush vset jquery_custom_scrollbar_selectors FALSE
  drush vset jquery_custom_scrollbar_pages FALSE

EXAMPLES
--------
Examples can be found on your website relative path:
jquery-custom-scrollbar/drupal-integration-example

Or on the official website:
http://jquery-custom-scrollbar.rocketmind.pl/

MAINTAINERS
-----------
The module is maintained by B2F (ifzenelse.net)
