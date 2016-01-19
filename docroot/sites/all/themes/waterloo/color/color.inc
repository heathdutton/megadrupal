<?php

/**
 * @file
 * Color module integration.
 */

/**
 * Put the logo path into JavaScript for the live preview.
 */
drupal_add_js(array('color' => array('logo' => theme_get_setting('logo', 'waterloo'))), 'setting');

$info = array(
  // Available colours and colour labels used in theme.
  'fields' => array(
    'main' => t('Main colour'),
    'text' => t('Text colour'),
    'link' => t('Link colour'),
  ),
  // Pre-defined color schemes.
  'schemes' => array(
    'default' => array(
      'title' => t('Regal red (default)'),
      'colors' => array(
        'main' => '#8b0d0d',
        'text' => '#151515',
        'link' => '#8b0d0e',
      ),
    ),
    'blue' => array(
      'title' => t('Ocean blue'),
      'colors' => array(
        'main' => '#154780',
        'text' => '#151515',
        'link' => '#154780',
      ),
    ),
    'green' => array(
      'title' => t('Battlefield green'),
      'colors' => array(
        'main' => '#37531f',
        'text' => '#151515',
        'link' => '#37531f',
      ),
    ),
    'purple' => array(
      'title' => t('Royal purple'),
      'colors' => array(
        'main' => '#462963',
        'text' => '#151515',
        'link' => '#462963',
      ),
    ),
  ),

  // CSS files (excluding @import) to rewrite with new colour scheme.
  'css' => array(
    'css/colours.css',
  ),

  // Files to copy.
  'copy' => array(
    'logo.png',
  ),

 // Gradient definitions.
  'gradients' => array(),

  // Colour areas to fill (x, y, width, height).
  'fill' => array(),

  // Coordinates of all the theme slices (x, y, width, height)
  // with their filename as used in the stylesheet.
  'slices' => array(),

  // Reference colour used for blending. Matches the base.png's colours.
  'blend_target' => '#ffffff',

  // Preview files.
  'preview_css' => 'color/preview.css',
  'preview_js' => 'color/preview.js',
  'preview_html' => 'color/preview.html',

  // Base file for image generation.
  'base_image' => 'color/base.png',
);
