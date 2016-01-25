<?php
/**
 * @file   taylor.inc
 * @date   Tue Dec 18 9:15:00 2012
 * @brief  The panels layout plugin for the taylor panels layout.
 *
 *
 */

// Plugin definition
$plugin = array(
  'title' => t('Taylor'),
  'icon' => 'taylor.png',
  'category' => t('Panopoly'),
  'theme' => 'taylor',
  'css' => 'taylor.css',
  'regions' => array(
    'header' => t('Header'),
    'half' => t('Half Column'),
    'quarter1' => t('Quarter 1 Column'),
    'quarter2' => t('Quarter 2 Column'),
    'footer' => t('Footer'),
  ),
);
