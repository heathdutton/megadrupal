<?php
/**
 * Implements settings for Color module integration.
 */

$info = array(

  // Available colors and color labels used in theme.
  'fields' => array(
    'base' => t('Background color'),
    'link' => t('Link color'),
    'top' => t('Header top'),
    'bottom' => t('Header bottom'),
    'text' => t('Text color'),
    'footer' => t('Footer Background'),
    'titleslogan' => t('Title and slogan'),
    'linksbg' => t('Links wrapper Background'),
  ),

  // Pre-defined color schemes.
  'schemes' => array(
    'default' => array(
      'title' => t('Fusion (Default)'),
      'colors' => array(
        'base' => '#292929',
        'link' => '#DFB77C',
        'top' => '#ffffff',
        'bottom' => '#f3f3f3',
        'text' => '#F0F0F0',
        'footer' => '#000000',
        'titleslogan' => '#525252',
        'linksbg' => '#444',
      ),
    ),
    'Blue Lagoon' => array(
      'title' => t('Blue Lagoon'),
      'colors' => array(
        'base' => '#ffffff',
        'link' => '#0073b6',
        'top' => '#2385c2',
        'bottom' => '#5ab5ee',
        'text' => '3b3b3b',
        'footer' => '#333333',
        'titleslogan' => '#fffeff',
        'linksbg' => '#2a99de',
      ),
    ),
    'Plum' => array(
      'title' => t('Plum'),
      'colors' => array(
        'base' => '#f6eecc',
        'link' => '#6b2454',
        'top' => '#461453',
        'bottom' => '#6b2454',
        'text' => '#2c2333',
        'footer' => '#030303',
        'titleslogan' => '#fffeff',
        'linksbg' => '#873e3e',
      ),
    ),
    'Fresh' => array(
      'title' => t('Fresh'),
      'colors' => array(
        'base' => '#f6ffff',
        'link' => '#087095',
        'top' => '#78b91a',
        'bottom' => '#5b8b15',
        'text' => '#585858',
        'footer' => '#191a19',
        'titleslogan' => '#fffeff',
        'linksbg' => '#8aa02a',
      ),
    ),
    'adobe' => array(
      'title' => t('adobe'),
      'colors' => array(
        'base' => '#fff4da',
        'link' => '#ee4c1c',
        'top' => '#dbb987',
        'bottom' => '#ceb995',
        'text' => '#494949',
        'footer' => '#60655d',
        'titleslogan' => '#8e7043',
        'linksbg' => '#bd9355',
      ),
    ),
  ),

  // Images to copy over.
  'copy' => array(
    'logo.png',
  ),

  // CSS files (excluding @import) to rewrite with new color scheme.
  'css' => array(
    'css/fusion-acquia-slate-style.css',
  ),

  // Gradient definitions.
  'gradients' => array(
    array(
      // (x, y, width, height).
      'dimension' => array(0, 38, 760, 121),
      // Direction of gradient ('vertical' or 'horizontal').
      'direction' => 'vertical',
      // Keys of colors to use for the gradient.
      'colors' => array('top','footer','div', 'bottom'),
    ),
  ),

  // Color areas to fill (x, y, width, height).
  'fill' => array(
    'base' => array(0, 0, 768, 568),
  ),

  // Coordinates of all the theme slices (x, y, width, height)
  // with their filename as used in the stylesheet.
  'slices' => array(
    'images/base.png' => array(0, 0, 780, 280),
  ),

  // Reference color used for blending. Matches the base.png's colors.
  'blend_target' => '#000000',

  // Preview files.
  'preview_image' => 'color/preview.png',
  'preview_css' => 'color/preview.css',

  // Base file for image generation.
  'base_image' => 'color/base.png',
);
