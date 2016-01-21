<?php
/**
 * @file
 * Plugin definition for Versatile site template layout.
 */

$plugin = array(
  'title' => t('Versatile site template'),
  'theme' => 'versatile_html5_site_template',
  'icon' => 'versatile-html5-site-template.png',
  'admin css' => 'versatile_html5_site_template.css',
  'category' => 'Versatile (HTML5)',
  'regions' => array(
    'header' => t('Header'),
    'main' => t('Main'),
    'footer' => t('Footer'),
  ),
);