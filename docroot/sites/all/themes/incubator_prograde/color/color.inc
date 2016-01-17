<?php

/**
 * @file
 *
 */

$info = array();

$info['fields'] = array(
  'base' => t('Main color'),                                      // $color_base
  'link' => t('Links'),                                           // $color_link
  'text' => t('Text'),                                            // $color_text
  'title' => t('Titles'),                                        // $color_title
  'block' => t('Block borders'),                                // $color_border
  'highlight' => t('Text on Color'),                    // $color_text_highlight
  'bg' => t('Background'),                                          // $color_bg
  'secondary' => t('Secondary color'),                       // $color_secondary
);

$info['schemes']['default'] = array(
  'title' => t('Default grey'),
  'colors' => array(
    'base' => '#858585',
    'link' => '#1d50c3',
    'text' => '#343434',
    'title' => '#736254',
    'block' => '#c7c7c7',
    'highlight' => '#e5e5e5',
    'bg' => '#fcfcfc',
    'secondary' => '#dcd9b7',
  ),
);

$info['css'] = array('css/colors.css');

$info['copy'] = array('logo.png');

$info['preview_css'] = 'css/preview.css';
$info['preview_js'] = 'color/preview.js';
$info['preview_html'] = 'color/preview.html';

// Gradients
$info['gradients'] = array();

// Color areas to fill (x, y, width, height)
$info['fill'] = array();

$info['slices'] = array();

$info['base_image'] = 'color/base.png';

$info['blend_target'] = '#ffffff';

$info['schemes']['slateblue'] = array(
  'title' => t('Slate Blue'),
  'colors' => array(
    'base' => '#5a6f82',
    'link' => '#476887',
    'text' => '#343434',
    'title' => '#2f2f2f',
    'block' => '#cdd4da',
    'highlight' => '#fdfdfd',
    'bg' => '#fcfcfc',
    'secondary' => '#404e5b',
  ),
);


return $info;