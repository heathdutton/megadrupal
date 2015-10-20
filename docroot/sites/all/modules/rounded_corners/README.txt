
CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Dependencies
 * Installation
 * Round corners and images
 * Authors

 INTRODUCTION
-------------

Rounded Corners is a wrapper module around the jQuery Corner plugin
available at http://www.malsup.com/jquery/corner/.

This module also provides an API function that allows adding JavaScript
corner() commands via PHP.

For example, copy this code snippet to your custom module, just change
"foo" to your module's name.

  /**
   * Implements hook_init().
   *
   * Add round corners on the top of the message area.
   */
  function foo_corners_init() {
    // Add an example message.
    drupal_set_message(t('This message should have the top with rounded corners.'));

    $commands = array();
    $commands[] = array('selector' => '.messages');
    // Add the rounded corners.
    rounded_corners_add_corners($commands);
  }

DEPENDENCIES
------------------------

This module uses the 1.x branch of Libraries API. 

INSTALLATION
------------------------

1. Download and install Libraries 1.x from http://drupal.org/project/libraries.
2. Extract the contents of these projects into /sites/all/modules.
3. Download the jQuery Corner plugin from http://www.malsup.com/jquery/corner/.
4. Place the jquery.corner.js into /sites/all/libraries/corner.
5. Enable the module.

ROUND CORNERS AND IMAGES
------------------------

Round corners can be used on images, by selecting the wrapping div and not the
<img> itself and by settings the "image wrapper" property to TRUE. for example,
consider the following HTML:

  <div class="foo">
    <img src="bar.jpg" width="10" height="10">
  </div>

And the PHP code:

  $commands = array();
  $commands[] = array(
    // Select the wrapping DIV.
    'selector' => '.foo',
    // Let the module know this is a wrapping div of an image.
    'image wrapper' => TRUE,
  );
  rounded_corners_add_corners($commands);

Setting the "image wrapper" property to TRUE will insure two things:
1) The wrapping div height and width will be adjusted according to the image
   dimensions.
2) The round corners plugin will not try to use the browser's native support for
   round corners, thus will insure the image corners are properly "hidden".

AUTHORS
-------
- Yuval Hager 'yhager', author of the 6.x-1.x branch <http://drupal.org/user/71425>
- Amitai Burstein 'Amitaibu', author of the 6.x-2.x branch <http://drupal.org/user/57511>
- Gabor Seljan 'sgabe', author of the 7.x-3.x branch <http://drupal.org/user/232117>
