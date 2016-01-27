Yelp API 2.x for Drupal 7.x
----------------------------
The Yelp API module provides the nessisary functions and hooks to query the Yelp
API and return results.

Installation
------------
The Yelp API module can be installed like any other Drupal module -- place it
in the modules directory for your site and enable it on the 'admin/build/modules'
page.  To take full advantage of the Yelp API module, you should enable the
'Yelp Blocks' module as well.

Compatibility
-------------
Yelp API provides integration with the following modules:
- Tokens[1]
- addressfield [2] (must be patched to work [3])
- Features (through the Yelp Blocks module)

Yelp Images
-----------
- Per Yelps terms of use, whenever results from Yelp are displayed the Yelp logo 
must also be displayed. The Yelp API module provides all available Yelp logo images, 
and they can be found in the 'yelp-images' directory.

- To display a yelp image you should use the 'drupal_get_path()' function

## EXAMPLE
<img src="<?php echo drupal_get_path('module', 'yelp_api'); ?>/yelp-images/Powered_By_Yelp_Black.png" />

For Developers
--------------
Please read `API.txt` for more information about the concepts and integration
points in the Yelp API module.

Maintainer
-----------
- mikemiles86 (Mike Miles)

[1]: http://drupal.org/project/token
[2]: http://drupal.org/project/addressfield
[3]: http://drupal.org/node/970048#comment-5712492