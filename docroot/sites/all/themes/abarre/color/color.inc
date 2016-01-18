<?php
// $Id:$

$info = array(

  // Available colors and color labels used in theme.
  'fields' => array(
    'base' => t('Base color'),
    'link' => t('Link color'),
    'top' => t('Header top'),
    'bottom' => t('Header bottom'),
    'text' => t('Text color'),
  ),
  // Pre-defined color schemes
  'schemes' => array(
    'default' => array(
      'title' => t('Athens (Default)'),
      'colors' => array(
        'base' => '#e0dbc2',
        'link' => '#D63E03',
        'top' => '#e0dbc2',
        'bottom' => '#e0dbc2',
        'text' => '#545454',
      ),
    ),
    'monday' => array(
      'title' => t('Monday'),
      'colors' => array(
        'base' => '#bce0f6',
        'link' => '#0074cd',
        'top' => '#C7C2A9',
        'bottom' => '#C7C2A9',
        'text' => '#4e5c64',
      ),
    ),
    'white_and_gold' => array(
      'title' => t('White and Gold'),
      'colors' => array(
        'base' => '#f3f3f3',
        'link' => '#9f8741',
        'top' => '#C7C2A9',
        'bottom' => '#C7C2A9',
        'text' => '#6e6e6e',
      ),
    ),
    'drupal_blue' => array(
      'title' => t('Drupal Blue'),
      'colors' => array(
        'base' => '#65bcf1',
        'link' => '#014e88',
        'top' => '#C7C2A9',
        'bottom' => '#C7C2A9',
        'text' => '#6b6b6b',
      ),
    ),
    'woody' => array(
      'title' => t('Woody'),
      'colors' => array(
        'base' => '#baab7d',
        'link' => '#840000',
        'top' => '#C7C2A9',
        'bottom' => '#C7C2A9',
        'text' => '#6b6b6b',
      ),
    ),
    'vancouver' => array(
      'title' => t('Vancouver'),
      'colors' => array(
        'base' => '#c3f8ab',
        'link' => '#0074cd',
        'top' => '#C7C2A9',
        'bottom' => '#C7C2A9',
        'text' => '#71605b',
      ),
    ),
    'golden_boy' => array(
      'title' => t('Golden Boy'),
      'colors' => array(
        'base' => '#f5ca00',
        'link' => '#c0480c',
        'top' => '#C7C2A9',
        'bottom' => '#C7C2A9',
        'text' => '#3b3b3b',
      ),
    ),
    'aufumy' => array(
      'title' => t('RCS - Aufumy'),
      'colors' => array(
        'base' => '#7db798',
        'link' => '#877123',
        'top' => '#C7C2A9',
        'bottom' => '#C7C2A9',
        'text' => '#193930',
      ),
    ),
    'katbailey' => array(
      'title' => t('RCS - Katbailey'),
      'colors' => array(
        'base' => '#06757a',
        'link' => '#f27a0d',
        'top' => '#C7C2A9',
        'bottom' => '#C7C2A9',
        'text' => '#064499',
      ),
    ),
  ),


  // Images to copy over.
  'copy' => array(
    'images/comment.gif',
    'images/delete.gif',
    'images/edit.gif',
    'images/reply.gif',
    'images/shadow-bottom.gif',
    'logo.png',
  ),

  // CSS files (excluding @import) to rewrite with new color scheme.
  'css' => array(
    'css/style.css',
  ),
  
  // Gradient definitions.
  'gradients' => array(
    array(
      // (x, y, width, height).
      'dimension' => array(0, 38, 760, 121),
      // Direction of gradient ('vertical' or 'horizontal').
      'direction' => 'vertical',
      // Keys of colors to use for the gradient.
      'colors' => array('top', 'bottom'),
    ),
  ),

  // Color areas to fill (x, y, width, height)
  'fill' => array(
    'base' => array(0, 0, 760, 568),
    'link' => array(107, 533, 41, 23),
  ),

  // Coordinates of all the theme slices (x, y, width, height)
  // with their filename as used in the stylesheet.
  'slices' => array(
    'images/bg.gif'                       => array(0, 0, 630, 35),
    'images/side-li-active.gif'           => array(278, 219, 13, 7),
    'images/side-li.gif'                  => array(278, 235, 13, 7),
    'images/primary.gif'                  => array(276, 273, 15, 6),
    'images/primary-active.gif'           => array(276, 285, 15, 6),
    'images/header-side.gif'              => array(0, 0, 15, 15),
    'images/screenshot.png'               => array(0, 0, 400, 240),
    'images/footer-corner.gif'            => array(236, 522, 29, 29),
    'images/footer-corner2.gif'           => array(187, 522, 29, 29),
    'images/block.gif'                    => array(0, 434, 260, 18),
    'images/bullet.gif'                   => array(242, 386, 4, 4),
  ),

  // Reference color used for blending. Matches the base.png's colors.
  'blend_target' => '#ffffff',

  // Preview files
  'preview_image' => 'color/preview.png',
  'preview_css' => 'color/preview.css',

  // Base file for image generation
  'base_image' => 'color/base.png',
);
