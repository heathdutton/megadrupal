Overview:
--------
The module provides like button for one of the most popular Russian social
networks VKontakte.ru (vk.com).

Rather than having to manually copy/paste the button code from vk.com for each
piece of content you (or your users) create, this module will automatically add
that code to the end of each chosen node type. You are able to add a Like button
which likes a given URL (static, e.g. your homepage) or/and put a dynamic Like
button on your page to like the page you are actually visiting.

The module provides similar functionality to Facebook Like Button module for
Facebook, which was used as an example. The module allows site administrators to
customize all the options for the button (size, style, text) provided by vk.com
at the moment. Drupal 7 only is supported for now.

Features:
---------
The Vkontakte like button module:

* Adds a like button on your site (static)
* Add more like buttons to your site (dynamic)
* Control the position of the buttons
* Integrates with display suite.
* Overrideable output through overriding 'vklikebutton' theme.


Installation:
------------
Install this module in the usual way (drop the file into your site's contributed
modules directory).

Configuration:
-------------
Create a standalone app at http://vk.com/editapp?act=create and copy its app ID 
to respective field in "Configuration" -> "VK Like Button" menu.

Go to sub-menus of "Configuration" -> "VK Like Button" to find all the 
configuration options.



Display Suite Integration:
-------------------------
When using display suite to display the node's content the normal VKontakte like
button configuration settings for the dynamic VKontakte like button become
somewhat irrelevant.
