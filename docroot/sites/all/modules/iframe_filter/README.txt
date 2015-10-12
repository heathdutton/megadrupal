$Id 

-- SUMMARY --

Iframe Filter allows to embed content into an iframe in a simple way, you
just need to add this filter to an input format and the content you creat
using that format will be displayed inside an iframe created dynamically.

This module is perfect for embedding external content or widgets.

Project page:
  http://drupal.org/project/iframe_filter
Bug reports, feature suggestions and latest developments:
  http://drupal.org/project/issues/iframe_filter


-- REQUIREMENTS --

* none


-- INSTALLATION --

* Install as usual, see http://drupal.org/node/70151 for further information.


-- USAGE --

  1. Go to input format page and create or edit an input format.
  2. In the list of filters choose "iframe filter".
  3. Make sure that this filter is the latest one to be executed, for doing
     this click "configure" on the input format and then go to "rearrange".
  4. Move the iframe filter to the latest position and click save.
  5. Create the block or content that you want to be displayed as an iframe
     choosing the input format you defined previously.
  6- Enter the height and width of the iframe in an HTML comment in this way:
    <!-- =wxh --> or <!-- width="w" height="w" --> replace "h" y "w" with the
	correct width and height.
  7- Save the content.


-- TROUBLESHOOTING --

* If the module doesn't find a valid width or height the content will not be
  placed inside an iframe, make sure that the syntax is correct.


-- CREDITS --

This project is maintained by Javier Reartes (javierreartes)

Originally created by Nathan Haug (quicksketch)
