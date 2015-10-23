<?php
/**
 * @file   moscone.inc
 * @date   Tue Dec 18 9:50:00 2012
 * @brief  The panels layout plugin for the moscone panels layout.
 *
 *
 */

// Plugin definition
$plugin = array(
  'title' => t('Moscone'),
  'icon' => 'moscone.png',
  'category' => t('Panopoly'),
  'theme' => 'moscone',
  'css' => 'moscone.css',
  'regions' => array(
    'header' => t('Header'),
    'sidebar' => t('Content Sidebar'),
    'content_main' => t('Content'),
    'footer' => t('Footer'),
  ),
);
