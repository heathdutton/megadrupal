-- SUMMARY --

Responsive images and styles (resp_img) This module solves the problems with images
and responsive themes, it allows you to define multiple image style suffixes and their
corresponding maximum width.

For a full description of the module, visit the project page:
http://drupal.org/project/resp_img

To submit bug reports and feature suggestions, or to track changes:
http://drupal.org/project/issues/resp_img

-- REQUIREMENTS --

none

-- INSTALLATION --

* Install as usual, see http://drupal.org/node/70151 for further information.

-- TUTORIAL --

Install a responsive theme like omega and create the following image styles at:
/admin/config/media/image-styles

thumbnail_mobile: width 100px;
thumbnail_narrow: width 150px;
thumbnail_normal: width 220px;
thumbnail_wide: width 300px;
goto admin/config/media/resp_img and add the following

add a suffix: _narrow - 500
add a suffix: _normal - 720
add a suffix: _wide - 900
goto admin/config/media/resp_img/settings and enter _mobile for the default suffix

When using in combination with views choose your _mobile image style for all images

-- KNOWN PROBLEMS --

If you enable 'Force resize' the module tries to resize everything, this works for plain images,
but not for field_slideshow and some other jQuery slideshow like plugins.
If you use barracuda/octopus (http://drupal.org/project/barracuda) you have to change the cache
key, add $cookie_respimg to the fastcgi_cache_key directive inside nginx_advanced_include.conf

