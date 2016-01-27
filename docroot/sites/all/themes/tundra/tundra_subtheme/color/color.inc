<?php
  //color schemes
  $info = array(

  // Available colors and color labels used in theme.
  'fields' => array(
    'base' => t('Base color'),
    'link' => t('Link color'),
    'text' => t('Text color'),
  ),

  'schemes' => array(
    'default' => array(
      'title' => t('Green (Default)'),
      'colors' => array(
        'base' => '#88cb00',
        'link' => '#666666',
        'text' => '#444444',
      ),
    ),
  ),

  // Images to copy over
  'copy' => array(
    'logo.png',
  ),

  // Color areas to fill (x, y, width, height)
  'fill' => array(
    'base' => array(0, 0, 1488, 1057),
  ),

  // Gradient definitions.
  'gradients' => array(
    array(
      // (x, y, width, height).
      'dimension' => array(0, 0, 0, 0),
      // Direction of gradient ('vertical' or 'horizontal').
      'direction' => 'vertical',
      // Keys of colors to use for the gradient.
      'colors' => array('base', 'link'),
    ),
  ),

  // Coordinates of all the theme slices (x, y, width, height)
  // with their filename as used in the stylesheet.
  'slices' => array(
  ),

  // color preview css
  'preview_css' => '',

  // Base file for image generation
  'base_image' => 'color/base.png',

  //  blend target
  'blend_target' => '#ffffff'
);