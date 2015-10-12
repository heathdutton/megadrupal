<?php

/**
 * @file
 * Makes layers a colorable theme.
 */

$info = array();

// Define the possible replaceable items and their labels.
$info['fields'] = array(
  'base' => t('Primary Color'),
  'secondary' => t('Secondary Color'),
  'blend' => t('Blend links'),
  'text' => t('Text Color'),
  'background' => t('Background color'),
  'link' => t('Link Color'),
);

// Color schemes for the site.
$info['schemes']['default'] = array(
  'title' => t('Default'),
  'colors' => array(
    'base' => '#1abc9c',
    'secondary' => '#16a085',
    'blend' => '#18af92',
    'text' => '#1f2126',
    'background' => '#fcfcfc',
    'link' => '#ffffff',
  ),
);
// Color schemes for the site.
$info['schemes']['emerald'] = array(
  'title' => t('Emerald'),
  'colors' => array(
    'base' => '#2ecc71',
    'secondary' => '#27ae60',
    'blend' => '#2bbe69',
    'text' => '#1f2126',
    'background' => '#fcfcfc',
    'link' => '#ffffff',
  ),
);
// Color schemes for the site.
$info['schemes']['peterriver'] = array(
  'title' => t('Peter River'),
  'colors' => array(
    'base' => '#3498db',
    'secondary' => '#2980b9',
    'blend' => '#2f8dcc',
    'text' => '#1f2126',
    'background' => '#fcfcfc',
    'link' => '#ffffff',
  ),
);
// Color schemes for the site.
$info['schemes']['amethyst'] = array(
  'title' => t('Amethyst'),
  'colors' => array(
    'base' => '#9b59b6',
    'secondary' => '#8e44ad',
    'blend' => '#954fb2',
    'text' => '#1f2126',
    'background' => '#fcfcfc',
    'link' => '#ffffff',
  ),
);
// Color schemes for the site.
$info['schemes']['wetasphalt'] = array(
  'title' => t('Wet Asphalt'),
  'colors' => array(
    'base' => '#34495e',
    'secondary' => '#2c3e50',
    'blend' => '#304458',
    'text' => '#1f2126',
    'background' => '#fcfcfc',
    'link' => '#ffffff',
  ),
);
// Color schemes for the site.
$info['schemes']['sunflower'] = array(
  'title' => t('Sun Flower'),
  'colors' => array(
    'base' => '#f1c40f',
    'secondary' => '#f39c12',
    'blend' => '#f2b210',
    'text' => '#1f2126',
    'background' => '#fcfcfc',
    'link' => '#ffffff',
  ),
);
// Color schemes for the site.
$info['schemes']['carrot'] = array(
  'title' => t('Carrot'),
  'colors' => array(
    'base' => '#e67e22',
    'secondary' => '#d35400',
    'blend' => '#dd6b13',
    'text' => '#1f2126',
    'background' => '#fcfcfc',
    'link' => '#ffffff',
  ),
);
// Color schemes for the site.
$info['schemes']['alizarin'] = array(
  'title' => t('Alizarin'),
  'colors' => array(
    'base' => '#e74c3c',
    'secondary' => '#c0392b',
    'blend' => '#d54334',
    'text' => '#1f2126',
    'background' => '#fcfcfc',
    'link' => '#ffffff',
  ),
);
// Color schemes for the site.
$info['schemes']['concrete'] = array(
  'title' => t('Concrete'),
  'colors' => array(
    'base' => '#95a5a6',
    'secondary' => '#7f8c8d',
    'blend' => '#8b9a9b',
    'text' => '#1f2126',
    'background' => '#fcfcfc',
    'link' => '#ffffff',
  ),
);
// Define the CSS file(s) that we want the Color module to use as a base.
$info['css'] = array('css/color.css');

/**
 * Default settings for the advanced stuff.
 *
 * No need to edit these if you just want to play around with the colors.
 * Color wants these, otherwise it's not going to play.
 *
 * We dive deeper into these in the Part 2. Advanced Color settings.
 */

// Files we want to copy along with the CSS files, let's define these later.
$info['copy'] = array();

// Files used in the scheme preview.
$info['preview_css'] = 'color/preview.css';
$info['preview_js'] = 'color/preview.js';
$info['preview_html'] = 'color/preview.html';

// Gradients.
$info['gradients'] = array();

// Color areas to fill (x, y, width, height).
$info['fill'] = array();
$info['blend_target'] = '#1abc9c';
// Coordinates of all the theme slices (x, y, width, height).
// with their filename as used in the stylesheet.
$info['slices'] = array();

// Base file for image generation.
$info['base_image'] = 'color/base.png';
