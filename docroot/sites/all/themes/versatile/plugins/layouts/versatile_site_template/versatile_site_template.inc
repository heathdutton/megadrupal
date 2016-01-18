<?php
/**
 * @file
 * Plugin definition for Versatile site template layout.
 */

$plugin = array(
  'title' => t('Versatile site template'),
  'theme' => 'versatile_site_template',
  'icon' => 'versatile-site-template.png',
  'admin css' => 'versatile_site_template.css',
  'category' => 'Versatile',
  'regions' => array(
    'header' => t('Header'),
    'main' => t('Main'),
    'footer' => t('Footer'),
  ),
);