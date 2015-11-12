<?php
/**
 * @file
 * burr_flipped.inc
 */


// Plugin definition.
$plugin = array(
  'title' => t('Burr Flipped'),
  'icon' => 'burr-flipped.png',
  'category' => t('Layouts BackEnd'),
  'theme' => 'burr_flipped',
  'css' => 'burr-flipped.css',
  'regions' => array(
    'sidebar' => t('Sidebar'),
    'contentmain' => t('Content'),
  ),
);
