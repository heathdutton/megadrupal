================
  INSTALLATION
================

- Download the smiley-slider package:
  https://github.com/dglittle/smiley-slider/tarball/master

- Unpack it and rename the folder to "smiley-slider"

- Place the folder in the module's directory so that the image is at e.g.
  sites/all/modules/smileyslider/smiley-slider/smiley-slider.png


=========
  USAGE
=========

This is an API module; it just provides a new form element type for use in
other modules. Just set '#type' => 'smileyslider' and optionally set the
#range to a positive integer (it defaults to 10) and the value of the
element when submitted will be an integer between 0 and the #range,
indicating the user's happiness. For example:

  $form['smiley'] = array(
    '#type' => 'smileyslider', // required
    '#range' => 100, // defaults to 10
    '#title' => t('Happiness'), // only shows for users with JS disabled
    '#required' => TRUE,
    '#default_value' => rand(0, 100), // random amount of happiness
  );

Because it only provides a new form element type, this module has no effect on
your site by itself. Another module has to use the form element type.


==========
  AUTHOR
==========

This module was written by Isaac Sukin (IceCreamYou).
https://drupal.org/user/201425

The Smiley Slider script was written by dglittle at oDesk.
https://github.com/dglittle

The Drupal project is located at https://drupal.org/project/smileyslider

The Smiley Slider script is located at
https://github.com/dglittle/smiley-slider
