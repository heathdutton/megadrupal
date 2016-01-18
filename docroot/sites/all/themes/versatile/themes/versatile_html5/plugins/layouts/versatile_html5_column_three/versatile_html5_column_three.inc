<?php
/**
 * @file
 * Plugin definition for Versatile three column layout.
 */

$plugin = array(
  'title' => t('Versatile three columns'),
  'theme' => 'versatile_html5_column_three',
  'icon' => 'versatile-html5-column-three.png',
  'admin css' => 'versatile_html5_column_three.css',
  'category' => 'Versatile (HTML5)',
  'regions' => array(
    'main_alpha' => t('Main alpha'),
    'column_first_alpha' => t('First column alpha'),
    'column_second_alpha' => t('Second column alpha'),
    'column_third_alpha' => t('Third column alpha'),
    'main_beta' => t('Main beta'),
    'column_first_beta' => t('First column beta'),
    'column_second_beta' => t('Second column beta'),
    'column_third_beta' => t('Third column beta'),
  ),
);