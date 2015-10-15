<?php

/**
 * @file
 * Color module settings and presets for Parish Theme.
 */

// Put the logo path into JavaScript for the live preview.
drupal_add_js(array('color' => array('logo' => theme_get_setting('logo', 'parish_theme'))), 'setting');

$info = array(
  // Define the possible replaceable items and their labels.
  'fields' => array(
    'bg' => t('Main background'),
    'text' => t('Text color'),
    'link' => t('Link color'),
    'headings' => t('Headings'),
    'highlight' => t('Menu highlight'),
    'hover' => t('Link hover color'),
  ),

  // Define user-selectable color schemes.
  'schemes' => array(
    // Default - Cool Blue theme.
    'default' => array(
      'title' => t('Cool Blue'),
      'colors' => array(
        'bg' => '#ffffff',
        'text' => '#333333',
        'link' => '#237da3',
        'headings' => '#000000',
        'highlight' => '#3ca0cb',
        'hover' => '#2e8eb8',
      ),
    ),
    // Regal Red theme.
    'regal_red' => array(
      'title' => t('Regal Red'),
      'colors' => array(
        'bg' => '#ffffff',
        'text' => '#333333',
        'link' => '#ae0f13',
        'headings' => '#000000',
        'highlight' => '#9f282b',
        'hover' => '#eb141a',
      ),
    ),
    // Regal Red theme.
    'minimal_grey' => array(
      'title' => t('Minimal Grey'),
      'colors' => array(
        'bg' => '#ffffff',
        'text' => '#646262',
        'link' => '#2a3f83',
        'headings' => '#545454',
        'highlight' => '#5c5c5c',
        'hover' => '#435ba8',
      ),
    ),
  ),

  // Define the CSS file(s) that we want the Color module to use as a base.
  'css' => array(
    'css/colors.css',
  ),

  // Files we want to copy along with the CSS files.
  'copy' => array(
    'logo.png',
  ),

  // Files used in the preview.
  'preview_css' => 'color/preview.css',
  'preview_js' => 'color/preview.js',
  'preview_html' => 'color/preview.html',

  // Gradient definitions.
  'gradients' => array(),

  // Color areas to fill (x, y, width, height).
  'fill' => array(),

  // Coordinates of all the theme slices (x, y, width, height) with their
  // filename as used in the stylesheet.
  'slices' => array(),

  // Base file for image generation.
  'base_image' => 'color/base.png',
);
