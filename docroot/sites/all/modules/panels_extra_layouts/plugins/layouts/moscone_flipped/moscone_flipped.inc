<?php
/**
 * @file   moscone_flipped.inc
 * @date   Tue Dec 18 9:50:00 2012
 * @brief  The panels layout plugin for the moscone flipped panels layout.
 *
 *
 */

// Plugin definition
$plugin = array(
  'title' => t('Moscone Flipped'),
  'icon' => 'moscone-flipped.png',
  'category' => t('Panopoly'),
  'theme' => 'moscone_flipped',
  'css' => 'moscone-flipped.css',
  'regions' => array(
    'header' => t('Header'),
    'sidebar' => t('Content Sidebar'),
    'content_main' => t('Content'),
    'footer' => t('Footer'),
  ),
);
