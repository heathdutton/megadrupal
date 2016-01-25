<?php
  //color schemes
  $info = array(

  // Available colors and color labels used in theme.
  'fields' => array(
    'base' => t('Base color'),
    'link' => t('Link color'),
    'text' => t('Text color'),
    'background' => t('Background color'),
  ),

  'schemes' => array(
    'default' => array(
      'title' => t('Blue (Default)'),
      'colors' => array(
        'base' => '#004D7D',
        'link' => '#004D7D',
        'text' => '#454545',
        'background' => '#ebebeb',
      ),
    ),

    'scheme2' => array(
      'title' => t('Color 2'),
      'colors' => array(
        'base' => '#00017d',
        'link' => '#00017d',
        'text' => '#454545',
        'background' => '#ebebeb',
      ),
    ),

    'scheme3' => array(
      'title' => t('Color 3'),
      'colors' => array(
        'base' => '#48007d',
        'link' => '#48007d',
        'text' => '#454545',
        'background' => '#ebebeb',
      ),
    ),

    'scheme4' => array(
      'title' => t('Color 4'),
      'colors' => array(
        'base' => '#7d0065',
        'link' => '#7d0065',
        'text' => '#454545',
        'background' => '#ebebeb',
      ),
    ),

    'scheme5' => array(
      'title' => t('Color 5'),
      'colors' => array(
        'base' => '#7d001a',
        'link' => '#7d001a',
        'text' => '#454545',
        'background' => '#ebebeb',
      ),
    ),

    'scheme6' => array(
      'title' => t('Color 6'),
      'colors' => array(
        'base' => '#7d3000',
        'link' => '#7d3000',
        'text' => '#454545',
        'background' => '#ebebeb',
      ),
    ),

    'scheme7' => array(
      'title' => t('Color 7'),
      'colors' => array(
        'base' => '#7d7b00',
        'link' => '#7d7b00',
        'text' => '#454545',
        'background' => '#ebebeb',
      ),
    ),

    'scheme8' => array(
      'title' => t('Color 8'),
      'colors' => array(
        'base' => '#337d00',
        'link' => '#337d00',
        'text' => '#454545',
        'background' => '#ebebeb',
      ),
    ),

    'scheme9' => array(
      'title' => t('Color 9'),
      'colors' => array(
        'base' => '#007d17',
        'link' => '#007d17',
        'text' => '#454545',
        'background' => '#ebebeb',
      ),
    ),

    'scheme10' => array(
      'title' => t('Color 10'),
      'colors' => array(
        'base' => '#007d62',
        'link' => '#007d62',
        'text' => '#454545',
        'background' => '#ebebeb',
      ),
    ),
  ),

  // Images to copy over
  'copy' => array(
    'logo.png',
  ),

  // CSS files (excluding @import) to rewrite with new color scheme.
  'css' => array(
    'color/color_module.css',
  ),

  // Color areas to fill (x, y, width, height)
  'fill' => array(
  ),

  // Gradient definitions.
  'gradients' => array(
  ),

  // Coordinates of all the theme slices (x, y, width, height)
  // with their filename as used in the stylesheet.
  'slices' => array(
  ),

  // color preview css
  'preview_css' => '',

  // Base file for image generation
  'base_image' => 'logo.png',

  //	blend target
	'blend_target' => '#ffffff'
);
?>