<?php
// Put the logo path into JavaScript for the live preview.
// global $theme_key;
// drupal_add_js(array('color' => array('logo' => theme_get_setting('logo', $theme_key))), 'setting');


// Sample of $info
// Notice that you can transform the field into fieldset if you separate the array key by using # character


$info = array(
  // Sample of $info
// Notice that you can transform the field into fieldset if you separate the array key by using # character
  'fields' => array(
    // Body & Required, without this color module will throw notice error
    'base' => t('body background'),
    'page' => t('Page background'),

    // Links & Required, without this color module will throw notice error
    'text' => t('Text'),
    'link' => t('Link'),
    'active' => t('Link active'),
    'hover' => t('Link hover'),

    // Sample where we wrap the element in a fieldset
    'sample#one' => t('Sample one'),
    'sample#two' => t('Sample two'),
    'sample#three' => t('Sample three'),
  ),
  // Pre-defined color schemes.
  'schemes' => array(
    'default' => array(
      'title' => t('My cool color (default)'),
      'colors' => array(
        // body
        'base' => '#eeeeee',
        'page' => '#ffffff',

        // Links
        'text' => '#585858',
        'link' => '#2964bf',
        'active' => '#34689c',
        'hover' => '#3f7dbb',

        // Sample
        'sample#one' => '#f5f5f5',
        'sample#two' => '#f5f5f5',
        'sample#three' => '#f5f5f5',
 			),
    ),
  ),

  'copy' => array(),

  // cSS files (excluding @import) to rewrite with new color scheme.
  'css' => array(
    'css/colors.css',
  ),

  // Gradient definitions.
  'gradients' => array(),

  // color areas to fill (x, y, width, height).
  'fill' => array(),

  // coordinates of all the theme slices (x, y, width, height)
  // with their filename as used in the stylesheet.
  'slices' => array(),

  // Reference color used for blending. Matches the base.png's colors.
  'blend_target' => '#ffffff',

  // Preview files.
  'preview_css' => 'color/preview.css',
  'preview_js' => 'color/preview.js',
  'preview_html' => 'color/preview.html',

  // base file for image generation.
  'base_image' => 'color/base.png',
);

