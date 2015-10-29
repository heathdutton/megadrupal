<?php
// $Id: color.inc,v 1.2 2011/02/04 14:35:58 jax Exp $

/**
 * @file
 * Settings for the color module.
 */

$info = array(
  // Available colors and color labels used in theme.
  'fields' => array(
    'base'   => t('Main background'),
    'link'   => t('Link color'),
    'top'    => t('Header top'),
    'bottom' => t('Header bottom'),
    'text'   => t('Text color'),
    'search' => t('Search background'),
    'button' => t('Button color'),
    'border' => t('Image border'),
    'menu'   => t('Menu tabs'),
    'logo'   => t('Logo'),
  ),
  // Pre-defined color schemes.
  'schemes' => array(
    'default' => array(
      'title' => t('Desk02 Gradiel (default)'),
      'colors' => array(
        'base'   => '#8a8a8a',
        'link'   => '#0072B3',
        'top'    => '#383838',
        'bottom' => '#5b5b5b',
        'text'   => '#505050',
        'search' => '#bbc7ce',
        'button' => '#f16700',
        'border' => '#f1f1f1',
        'menu'   => '#8a8a8a',
        'logo'   => '#1773cf',
      ),
    ),
    'lagoon' => array(
      'title' => t('Blue Lagoon'),
      'colors' => array(
        'base'   => '#1773cf',
        'link'   => '#0071B3',
        'top'    => '#0779bf',
        'bottom' => '#48a9e4',
        'text'   => '#3b3b3b',
        'search' => '#f6f6f2',
        'button' => '#187adc',
        'border' => '#292929',
        'menu'   => '#8fc7e8',
        'logo'   => '#1773cf',
      ),
    ),
    'firehouse' => array(
      'title' => t('Firehouse'),
      'colors' => array(
        'base'   => '#f65013',
        'link'   => '#d6121f',
        'top'    => '#cd2d2d',
        'bottom' => '#cf3535',
        'text'   => '#3b3b3b',
        'search' => '#ddba92',
        'button' => '#f16700',
        'border' => '#ededed',
        'menu'   => '#d0948b',
        'logo'   => '#e34308',
      ),
    ),
    'ice' => array(
      'title' => t('Ice'),
      'colors' => array(
        'base'   => '#d0d0d0',
        'link'   => '#019dbf',
        'top'    => '#95c6d6',
        'bottom' => '#b7d6e6',
        'text'   => '#4a4a4a',
        'search' => '#84ccdc',
        'button' => '#2dcdf0',
        'border' => '#24272c',
        'menu'   => '#9ac2d6',
        'logo'   => '#c6d7d6',
      ),
    ),
    'plum' => array(
      'title' => t('Plum'),
      'colors' => array(
        'base'   => '#aa4598',
        'link'   => '#9d408d',
        'top'    => '#4c1c58',
        'bottom' => '#593662',
        'text'   => '#301313',
        'search' => '#d3c2d6',
        'button' => '#e977d6',
        'border' => '#653375',
        'menu'   => '#ad7ba5',
        'logo'   => '#bc4bc3',
      ),
    ),
    'slate' => array(
      'title' => t('Slate'),
      'colors'   => array(
        'base'   => '#818181',
        'link'   => '#0073b6',
        'top'    => '#5a5a5a',
        'bottom' => '#353535',
        'text'   => '#3b3b3b',
        'search' => '#888888',
        'button' => '#f10000',
        'border' => '#bebebe',
        'menu'   => '#aaaaaa',
        'logo'   => '#909090',
      ),
    ),
    'olivia' => array(
      'title' => t('Olivia'),
      'colors' => array(
        'base'   => '#7db323',
        'link'   => '#6a9915',
        'top'    => '#b5d52a',
        'bottom' => '#7db323',
        'text'   => '#191a19',
        'search' => '#7db323',
        'button' => '#9fe51f',
        'border' => '#b8e564',
        'menu'   => '#88aa4a',
        'logo'   => '#3fb00f',
      ),
    ),
  ),

  // CSS files (excluding @import) to rewrite with new color scheme.
  'css' => array(
    'css/style.css',
    'css/maintenance-page.css',
  ),

  // Files to copy.
  'copy' => array(
    'logo.png',
  ),

  // Gradient definitions.
  'gradients' => array(
    array(
      // (x, y, width, height).
      'dimension' => array(800, 168, 40, 64),
      // Direction of gradient ('vertical' or 'horizontal').
      'direction' => 'vertical',
      // Keys of colors to use for the gradient.
      'colors' => array('top', 'bottom'),
    ),
  ),

  // Color areas to fill (x, y, width, height).
  'fill' => array(
    'base'   => array(0, 0, 880, 600),
    'search' => array(0, 271, 178, 24),
    'menu'   => array(0, 242, 161, 28),
    'logo'   => array(0, 338, 49, 57),
    'link'   => array(800, 500, 9, 9),
  ),

  // Coordinates of all the theme slices (x, y, width, height)
  // with their filename as used in the stylesheet.
  'slices' => array(
    'images/branding-header.png'             => array(0, 0, 880, 121),
    'images/content-bottomline.png'          => array(0, 122, 880, 45),
    'images/content-sidebar-bottomline.png'  => array(0, 168, 500, 45),
    'images/dotted-line.png'                 => array(0, 214, 15, 27),
    'images/top-line-top.png'                => array(16, 214, 15, 25),
    'images/top-line.png'                    => array(32, 214, 15, 25),
    'images/top-line-bottom.png'             => array(48, 214, 15, 25),
    'images/footer-line.png'                 => array(64, 214, 15, 5),
    'images/main-li-a.png'                   => array(0, 242, 25, 28),
    'images/main-li-a-span.png'              => array(26, 242, 135, 28),
    'images/main-li-active-a.png'            => array(162, 240, 25, 30),
    'images/main-li-active-a-span.png'       => array(188, 240, 139, 30),
    'images/searchbox.png'                   => array(0, 271, 178, 23),
    'images/gradient.png'                    => array(800, 168, 38, 106),
    'images/form-input-text.png'             => array(355, 214, 30, 23),
    'images/form-textarea.png'               => array(328, 214, 26, 126),
    'images/logo.png'                        => array(0, 338, 49, 57),
    'images/add.png'                         => array(800, 500, 9, 9),
  ),

  // Reference color used for blending. Matches the base.png's colors.
  'blend_target' => '#ffffff',

  // Preview files.
  'preview_image' => 'color/preview.png',
  'preview_css'   => 'color/preview.css',

  // Base file for image generation.
  'base_image' => 'color/base.png',
);
