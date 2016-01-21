<?php

$info = array(
  // Available colors and color labels used in theme.
  'fields' => array(
    'base' => t('Main background'),
    'link' => t('Link color'),
    'top' => t('Header top'),
    'bottom' => t('Header bottom'),
    'text' => t('Text color'),
  ),
  // Pre-defined color schemes.
  'schemes' => array(
    'default' => array(
      'title' => t('Glass (default)'),
      'colors' => array(
        'base' => '#b2c8d3',
        'link' => '#027ac6',
        'top' => '#313332',
        'bottom' => '#424446',
        'text' => '#494949',
      ),
    ),
    'ash' => array (
      'title' => 'Ash',
      'colors' => array (
        'base' => '#464849',
        'link' => '#2f416f',
        'top' => '#2a2b2d',
        'bottom' => '#5d6779',
        'text' => '#494949',
      ),
    ),
    'aquamarine' => array (
      'title' => 'Aquamarine',
      'colors' => array (
        'base' => '#55c0e2',
        'link' => '#000000',
        'top' => '#085360',
        'bottom' => '#007e94',
        'text' => '#696969',
      ),
    ),
    'belgian_chocolate' => array (
      'title' => 'Belgian Chocolate',
      'colors' => array (
        'base' => '#d5b048',
        'link' => '#6c420e',
        'top' => '#331900',
        'bottom' => '#971702',
        'text' => '#494949',
      ),
    ),
    'bluemarine' => array (
      'title' => 'Bluemarine',
      'colors' => array (
        'base' => '#3f3f3f',
        'link' => '#336699',
        'top' => '#6598cb',
        'bottom' => '#6598cb',
        'text' => '#000000',
      ),
    ),
    'citrus_blast' => array (
      'title' => 'Citrus Blast',
      'colors' => array (
        'base' => '#d0cb9a',
        'link' => '#917803',
        'top' => '#efde01',
        'bottom' => '#e6fb2d',
        'text' => '#494949',
      ),
    ),
    'cold_day' => array (
      'title' => 'Cold Day',
      'colors' => array (
        'base' => '#0f005c',
        'link' => '#434f8c',
        'top' => '#4d91ff',
        'bottom' => '#1a1575',
        'text' => '#000000',
      ),
    ),
    'greenbeam' => array (
      'title' => 'Greenbeam',
      'colors' => array (
        'base' => '#c9c497',
        'link' => '#0c7a00',
        'top' => '#03961e',
        'bottom' => '#7be000',
        'text' => '#494949',
      ),
    ),
    'mediterrano' => array (
      'title' => 'Mediterrano',
      'colors' => array (
        'base' => '#ffe23d',
        'link' => '#a9290a',
        'top' => '#fc6d1d',
        'bottom' => '#a30f42',
        'text' => '#494949',
      ),
    ),
    'mercury' => array (
      'title' => 'Mercury',
      'colors' => array (
        'base' => '#788597',
        'link' => '#3f728d',
        'top' => '#a9adbc',
        'bottom' => '#d4d4d4',
        'text' => '#707070',
      ),
    ),
    'nocturnal' => array (
      'title' => 'Nocturnal',
      'colors' => array (
        'base' => '#5b5fa9',
        'link' => '#5b5faa',
        'top' => '#0a2352',
        'bottom' => '#9fa8d5',
        'text' => '#494949',
      ),
    ),
    'olivia' => array (
      'title' => 'Olivia',
      'colors' => array (
        'base' => '#7db323',
        'link' => '#6a9915',
        'top' => '#b5d52a',
        'bottom' => '#7db323',
        'text' => '#191a19',
      ),
    ),
    'pink_plastic' => array (
      'title' => 'Pink Plastic',
      'colors' => array (
        'base' => '#12020b',
        'link' => '#1b1a13',
        'top' => '#f391c6',
        'bottom' => '#f41063',
        'text' => '#898080',
      ),
    ),
    'shiny_tomato' => array (
      'title' => 'Shiny Tomato',
      'colors' => array (
        'base' => '#b7a0ba',
        'link' => '#c70000',
        'top' => '#a1443a',
        'bottom' => '#f21107',
        'text' => '#515d52',
      ),
    ),
    'teal_top' => array (
      'title' => 'Teal Top',
      'colors' => array (
        'base' => '#18583d',
        'link' => '#1b5f42',
        'top' => '#34775a',
        'bottom' => '#52bf90',
        'text' => '#2d2d2d',
      ),
    ),
  ),

  // CSS files (excluding @import) to rewrite with new color scheme.
  'css' => array(
    'css/default.css',
    'css/form.css',
    'css/form.css',
    'css/layout.css',
    'css/misc.css',
    'css/navigation.css',
    'css/table.css',
    'css/typography.css',
  ),

  // Files to copy.
  'copy' => array(
    'logo.png',
  ),

  // Gradient definitions.
  'gradients' => array(
    array(
      // (x, y, width, height).
      'dimension' => array(0, 0, 900, 28),
      // Direction of gradient ('vertical' or 'horizontal').
      'direction' => 'vertical',
      // Keys of colors to use for the gradient.
      'colors' => array('top', 'bottom'),
    ),
  ),

  // Color areas to fill (x, y, width, height).
  'fill' => array(
    'base' => array(0, 29, 900, 472),
    'link' => array(97, 210, 371, 56),
  ),

  // Coordinates of all the theme slices (x, y, width, height)
  // with their filename as used in the stylesheet.
  'slices' => array(
    'images/header/header_bg.png'                      => array(40, 0, 1, 28),
    'images/header/gohome.png'                    => array(7, 6, 18, 21),
    'images/panel/panel_bg.png'                    => array(276, 0, 341, 25),
    'images/panel/panel_content_bg.png'                    => array(276, 0, 1, 1),
    'images/panel/panel_gfx.png'                    => array(458, 0, 160, 25),
    'images/panel/panel_open.png'                    => array(244, 4, 16, 16),
    'images/panel/panel_close.png'                    => array(260, 4, 16, 16),
    'images/header/navigation_bg.png'                    => array(10, 29, 1, 64),
    'images/tabs/tableft_big.png'                    => array(61, 31, 5, 62),
    'images/tabs/tabright_big.png'                    => array(65, 31, 360, 62),
    'images/tabs/tableft.png'                    => array(451, 45, 5, 48),
    'images/tabs/tabright.png'                    => array(455, 45, 360, 48),
    'images/tabs/tableft_small.png'                    => array(451, 143, 5, 26),
    'images/tabs/tabright_small.png'                    => array(455, 143, 360, 26),
    'images/buttons/blue-button.png'          => array(97, 236, 369, 80),
    'images/layout/tl.gif'                    => array(246, 133, 10, 10),
    'images/layout/tr.gif'                    => array(256, 133, 10, 10),
    'images/layout/bl.gif'                    => array(246, 143, 10, 10),
    'images/layout/br.gif'                    => array(256, 143, 10, 10),
    'images/layout/b.gif'                    => array(256, 143, 1, 10),
    'images/layout/t.gif'                    => array(256, 133, 1, 10),
    'images/layout/l.gif'                    => array(246, 143, 10, 1),
    'images/layout/r.gif'                    => array(256, 143, 10, 1),
    'images/layout/content_bg.gif'                    => array(255, 142, 1, 1),
    'screenshot.png'                       => array(0, 37, 400, 240),
  ),

  // Reference color used for blending. Matches the base.png's colors.
  'blend_target' => '#ffffff',

  // Preview files.
  'preview_css' => 'color/preview.css',
  'preview_image' => 'color/preview.png',

  // Base file for image generation.
  'base_image' => 'color/base.png',
);

