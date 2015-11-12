<?php

// Put the logo path into JavaScript for the live preview.
drupal_add_js(array('color' => array('logo' => theme_get_setting('logo', 'bluemarine'))), 'setting');

$info = array(
  // Available color areas and labels used in theme.
  'fields' => array(
    'header' => t('Header background'),
    'base' => t('Content'),
    'sidebar' => t('Sidebar background'),
    'footer' => t('Footer background'),
    'title' => t('Title color'),
    'text' => t('Text color'),
    'link' => t('Link color'),
  ),
  // Pre-defined color schemes.
  'schemes' => array(
    'default' => array(
      'title' => t('Bluemarine (default)'),
      'colors' => array(
        'header' => '#6699cc',
        'base' => '#ffffff',
        'sidebar' => '#dddddd',
        'footer' => '#eeeeee',
        'title' => '#777777',
        'text' => '#000000',
        'link' => '#336699',
      ),
    ),
    'firehouse' => array(
      'title' => t('Greensea'),
      'colors' => array(
        'header' => '#66cc99',
        'base' => '#ffffff',
        'sidebar' => '#dddddd',
        'footer' => '#eeeeee',
        'title' => '#777777',
        'text' => '#000000',
        'link' => '#339966',
      ),
    ),
    'ice' => array(
      'title' => t('Land, ho!'),
      'colors' => array(
        'header' => '#cc9966',
        'base' => '#ffffff',
        'sidebar' => '#dddddd',
        'footer' => '#eeeeee',
        'title' => '#777777',
        'text' => '#000000',
        'link' => '#996633',
      ),
    ),
    'plum' => array(
      'title' => t('Purple sea urchin'),
      'colors' => array(
        'header' => '#9966cc',
        'base' => '#ffffff',
        'sidebar' => '#dddddd',
        'footer' => '#eeeeee',
        'title' => '#777777',
        'text' => '#000000',
        'link' => '#663399',
      ),
    ),
    'slate' => array(
      'title' => t('Sea sick green'),
      'colors' => array(
        'header' => '#99cc66',
        'base' => '#ffffff',
        'sidebar' => '#dddddd',
        'footer' => '#eeeeee',
        'title' => '#777777',
        'text' => '#000000',
        'link' => '#669933',
      ),
    ),
  ),

  // CSS files (excluding @import) to rewrite with new color scheme.
  'css' => array(
    'style.css',
  ),

  // Files to copy.
  'copy' => array(
    'logo.png',
  ),

  // Gradient definitions.
  'gradients' => array(),

  // Color areas to fill (x, y, width, height).
  'fill' => array(),

  // Coordinates of all the theme slices (x, y, width, height)
  // with their filename as used in the stylesheet.
  'slices' => array(),

  // Reference color used for blending. Matches the base.png's colors.
  'blend_target' => '#ffffff',

  // Preview files.
  'preview_css' => 'color/preview.css',
  'preview_js' => 'color/preview.js',
  'preview_html' => 'color/preview.html',

  // Base file for image generation.
  'base_image' => 'color/base.png',
);
