<?php

/**
 * @file
 * Dominion theme example file.
 */

/**
 * This is an example implementation of dominion.inc (build for Garland).
 *
 * This file must be named "dominion.inc" and located in the theme directory.
 */

$info = array(
  'options' => array(
    array(
      'title' => 'Header color',
      'search_string' => '#edf5fa',
      'default_value' => '#edf5fa',
      'type' => DOMINION_THEME_COLOR,
      'required' => TRUE,
    ),
    array(
      'title' => 'Link color',
      'search_string' => '#027AC6',
      'default_value' => '#027AC6',
      'type' => DOMINION_THEME_COLOR,
      'required' => TRUE,
    ),
    array(
      'title' => 'Link hover color',
      'search_string' => '#0062A0',
      'default_value' => '#0062A0',
      'type' => DOMINION_THEME_COLOR,
      'required' => TRUE,
    ),
    array(
      'title' => 'Background image',
      'search_string' => 'images/bg-content.png',
      'type' => DOMINION_THEME_IMAGE,
      'required' => TRUE,
    ),
    array(
      'title' => 'Font',
      'search_string' => 'Verdana, sans-serif',
      'type' => DOMINION_THEME_LIST,
      'required' => TRUE,
      'options' => array(
        'Verdana, sans-serif' => t('Verdana'),
        'Arial, sans-serif' => t('Arial'),
        '"Times New Roman", serif' => t('Times New Roman'),
        'monospace' => t('Monospace'),
      ),
    ),
    array(
      'title' => 'Additional CSS',
      'search_string' => 'extra.css',
      'type' => DOMINION_THEME_FILE,
      'required' => FALSE,
      'append_to' => 'style.css',
      'description' => t('Contents of this file will be appended to the stylesheet.'),
    ),
  ),

  'alpha_image' => array(
    'search_string' => 'transparent_header.png',
    'alpha' => 70,
    'color' => 0, // Number of configuration option
  ),

  // CSS files (excluding @import) to rewrite with new color scheme.
  'css' => array(
    'style.css',
  ),
);