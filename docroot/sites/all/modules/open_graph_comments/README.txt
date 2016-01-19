
*******************************************************
    README.txt for Open Graph Comments module
*******************************************************

Contents:
=========
1. ABOUT
2. REQUIREMENTS
3. INSTALLATION

1. ABOUT
===========

Open Graph Comments module allows to show Open Graph meta tags 
(http://opengraphprotocol.org/) of first link available in comment. Its same 
kind of functionality used by facebook and twitter to show title(with link), 
image and description of particular external page if its link is added in any 
status or tweet.
Open Graph Comments module get link from comment body and fetches open graph 
meta tags information to show og:title, og:link, og:image and og:description 
attached to comment body.
It also allows to enable or disable to display of open graph meta tags 
information for particular content type. It can be done from comment settings 
in content type edit page.
Open graph comments uses drupal_http_request() function to get data of 
particular page and then finds open graph tags (og:title, og:link, og:image 
and og:description) in its header. Once found it caches each comments open 
graph data so that drupal_http_request() function is not called again.

2. REQUIREMENTS
================

* Core comment module.

3. INSTALLATION
================

* Install as usual, 
  see https://drupal.org/documentation/install/modules-themes/modules-7 for 
  further information.
* Enable open graphs in comments for particular content type from comment 
  settings in content type edit form. This setting can be different for each 
  content type.
