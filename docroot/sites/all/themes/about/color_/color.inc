<?php

// Put the logo path into JavaScript for the live preview.
//drupal_add_js(array('color' => array('logo' => theme_get_setting('logo', 'about'))), 'setting');

// create a preview html file on the fly from php
$php_preview = DRUPAL_ROOT . '/' . drupal_get_path('theme', 'about') . '/color/preview.php';

if (module_exists('php')) {
  // holy moley! We better trust the site owner who uploaded this theme with a php preview file!
  //   ...however, this theme is intended for single user sites, so it's probably safe.
  $php_preview_contents = php_eval('<?php' . file_get_contents($php_preview) . '?>');
  file_put_contents(drupal_get_path('theme', 'about') . '/color/preview.html', $php_preview_contents);
}

$info = array(
  // Available colors and color labels used in theme.
  'fields' => array(
    'top' => t('Header top'),
    'bottom' => t('Header bottom'),
    'bg' => t('Main background'),
    'sidebar' => t('Sidebar background'),
    'sidebarborders' => t('Sidebar borders'),
    'footer' => t('Footer background'),
    'titleslogan' => t('Title and slogan'),
    'text' => t('Text color'),
    'link' => t('Link color'),
  ),
  // Pre-defined color schemes.
  'schemes' => array(
    'default' => array(
      'title' => t('Light on dark (default)'),
      'colors' => array(
        'top' => '#0779bf',
        'bottom' => '#48a9e4',
        'bg' => '#ffffff',
        'sidebar' => '#f6f6f2',
        'sidebarborders' => '#f9f9f9',
        'footer' => '#292929',
        'titleslogan' => '#fffeff',
        'text' => '#3b3b3b',
        'link' => '#0071B3',
      ),
    ),
  ),

  // CSS files (excluding @import) to rewrite with new color scheme.
  'css' => array(
    'css/colors.css',
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
